/*
 * Copyright 2018 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mgos_apds9960_internal.h"

struct mgos_apds9960 *mgos_apds9960_create(struct mgos_i2c *i2c, uint8_t i2caddr) {
  struct mgos_apds9960 *sensor = NULL;
  uint8_t id = 0;

  if (!i2c) {
    return NULL;
  }

  sensor = calloc(1, sizeof(struct mgos_apds9960));
  if (!sensor) {
    return NULL;
  }

  memset(sensor, 0, sizeof(struct mgos_apds9960));
  sensor->i2caddr             = i2caddr;
  sensor->i2c                 = i2c;
  sensor->gesture_ud_delta_   = 0;
  sensor->gesture_lr_delta_   = 0;
  sensor->gesture_ud_count_   = 0;
  sensor->gesture_lr_count_   = 0;
  sensor->gesture_near_count_ = 0;
  sensor->gesture_far_count_  = 0;
  sensor->gesture_state_      = 0;
  sensor->gesture_motion_     = APDS9960_DIR_NONE;
  sensor->light_handler       = NULL;
  sensor->proximity_handler   = NULL;
  sensor->gesture_handler     = NULL;
  mgos_apds9960_resetGestureParameters(sensor);

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ID, &id)) {
    LOG(LL_ERROR, ("Cannot read from device at I2C 0x%02x", sensor->i2caddr));
    free(sensor);
    return false;
  }
  if (!(id == APDS9960_ID_1 || id == APDS9960_ID_2)) {
    LOG(LL_ERROR, ("Device at I2C 0x%02x does not identify as an APDS9960", sensor->i2caddr));
    free(sensor);
    return false;
  }

  if (!mgos_apds9960_init(sensor)) {
    LOG(LL_ERROR, ("Could not initialize APDS9960 at I2C 0x%02x", sensor->i2caddr));
    free(sensor);
    return false;
  }

  // Install interrupt handler
  if (mgos_sys_config_get_apds9960_irq_pin() > 0) {
    mgos_gpio_set_mode(mgos_sys_config_get_apds9960_irq_pin(), MGOS_GPIO_MODE_INPUT);
    mgos_gpio_set_pull(mgos_sys_config_get_apds9960_irq_pin(), MGOS_GPIO_PULL_UP);
    mgos_gpio_set_int_handler(mgos_sys_config_get_apds9960_irq_pin(), MGOS_GPIO_INT_EDGE_NEG, mgos_apds9960_irq, NULL);
    mgos_gpio_enable_int(mgos_sys_config_get_apds9960_irq_pin());
  }

  LOG(LL_INFO, ("APDS9960 initialized at I2C 0x%02x", sensor->i2caddr));
  return sensor;
}

void mgos_apds9960_destroy(struct mgos_apds9960 **sensor) {
  if (!*sensor) {
    return;
  }
  mgos_apds9960_disable(*sensor);

  free(*sensor);
  *sensor = NULL;
  return;
}

bool mgos_apds9960_read_light(struct mgos_apds9960 *sensor, uint16_t *c, uint16_t *r, uint16_t *g, uint16_t *b) {
  if (!sensor) {
    return false;
  }

  if (c) {
    if (!mgos_apds9960_read_ambient_light(sensor, c)) {
      return false;
    }
  }
  if (r) {
    if (!mgos_apds9960_read_red_light(sensor, r)) {
      return false;
    }
  }
  if (g) {
    if (!mgos_apds9960_read_green_light(sensor, g)) {
      return false;
    }
  }
  if (b) {
    if (!mgos_apds9960_read_blue_light(sensor, b)) {
      return false;
    }
  }
  return true;
}

bool mgos_apds9960_read_proximity(struct mgos_apds9960 *sensor, uint8_t *val) {
  if (!sensor || !val) {
    return false;
  }
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_PDATA, val)) {
    return false;
  }
  return true;
}

bool mgos_apds9960_is_gesture_available(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GSTATUS, &val)) {
    return APDS9960_ERROR;
  }

  val &= APDS9960_GVALID;

  return val == 1;
}

int mgos_apds9960_read_gesture(struct mgos_apds9960 *sensor) {
  uint8_t fifo_level = 0;
  int     bytes_read = 0;
  uint8_t fifo_data[128];
  uint8_t gstatus;
  int     motion;
  int     i;
  uint8_t mode;

  if (!sensor) {
    return -1;
  }

  if (!mgos_apds9960_is_gesture_available(sensor)) {
    return APDS9960_DIR_NONE;
  }
  if (!mgos_apds9960_get_mode(sensor, &mode)) {
    return APDS9960_DIR_NONE;
  }
  if ((mode & 0b01000001) == 0) {
    return APDS9960_DIR_NONE;
  }
  // Keep looping as long as gesture data is valid
  while (1) {
    // Wait some time to collect next batch of FIFO data
    mgos_msleep(APDS9960_FIFO_PAUSE_TIME);

    // Get the contents of the STATUS register. Is data still valid?
    if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GSTATUS, &gstatus)) {
      return APDS9960_ERROR;
    }

    // If we have valid data, read in FIFO
    if ((gstatus & APDS9960_GVALID) == APDS9960_GVALID) {
      // Read the current FIFO level
      if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GFLVL, &fifo_level)) {
        return APDS9960_ERROR;
      }

#if DEBUG
      LOG(LL_DEBUG, ("FIFO Level: %d", fifo_level));

      /*
       * Serial.print("FIFO Level: ");
       * Serial.println(fifo_level);
       */
#endif

      // If there's stuff in the FIFO, read it into our data block
      if (fifo_level > 0) {
        bytes_read = mgos_apds9960_wireReadDataBlock(sensor, APDS9960_GFIFO_U,
                                                     (uint8_t *)fifo_data,
                                                     (fifo_level * 4));
        if (bytes_read == -1) {
          return APDS9960_ERROR;
        }
#if DEBUG
        LOG(LL_DEBUG, ("FIFO Dump:"));
        for (i = 0; i < bytes_read; i++) {
          LOG(LL_DEBUG, ("%d", fifo_data[i]));
        }

        /*
         * Serial.print("FIFO Dump: ");
         * for ( i = 0; i < bytes_read; i++ ) {
         *  Serial.print(fifo_data[i]);
         *  Serial.print(" ");
         * }
         * Serial.println();
         */
#endif

        // If at least 1 set of data, sort the data into U/D/L/R
        if (bytes_read >= 4) {
          for (i = 0; i < bytes_read; i += 4) {
            sensor->gesture_data_.u_data[sensor->gesture_data_.index] = fifo_data[i + 0];
            sensor->gesture_data_.d_data[sensor->gesture_data_.index] = fifo_data[i + 1];
            sensor->gesture_data_.l_data[sensor->gesture_data_.index] = fifo_data[i + 2];
            sensor->gesture_data_.r_data[sensor->gesture_data_.index] = fifo_data[i + 3];
            sensor->gesture_data_.index++;
            sensor->gesture_data_.total_gestures++;
          }

#if DEBUG
          LOG(LL_DEBUG, ("Up Data:"));
          for (i = 0; i < sensor->gesture_data_.total_gestures; i++) {
            LOG(LL_DEBUG, ("%d", sensor->gesture_data_.u_data[i]));
          }

          /*
           * Serial.print("Up Data: ");
           * for ( i = 0; i < sensor->gesture_data_.total_gestures; i++ ) {
           *  Serial.print(sensor->gesture_data_.u_data[i]);
           *  Serial.print(" ");
           * }
           * Serial.println();
           */
#endif

          // Filter and process gesture data. Decode near/far state
          if (mgos_apds9960_processGestureData(sensor)) {
            if (mgos_apds9960_decodeGesture(sensor)) {
              // ***TODO: U-Turn Gestures
#if DEBUG
              //Serial.println(gesture_motion_);
#endif
            }
          }

          // Reset data
          sensor->gesture_data_.index          = 0;
          sensor->gesture_data_.total_gestures = 0;
        }
      }
    } else {
      // Determine best guessed gesture and clean up
      mgos_msleep(APDS9960_FIFO_PAUSE_TIME);
      mgos_apds9960_decodeGesture(sensor);
      motion = sensor->gesture_motion_;
#if DEBUG
      LOG(LL_DEBUG, ("END: %d", sensor->gesture_motion_));

      /*
       * Serial.print("END: ");
       * Serial.println(sensor->gesture_motion_);
       */
#endif
      mgos_apds9960_resetGestureParameters(sensor);
      return motion;
    }
  }
  return -1;
}

bool mgos_apds9960_set_callback_light(struct mgos_apds9960 *sensor, uint16_t low_threshold, uint16_t high_threshold, mgos_apds9960_light_event_t handler) {
  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_enable_light_sensor(sensor)) {
    return false;
  }
  if (!mgos_apds9960_set_light_int_low_threshold(sensor, low_threshold)) {
    return false;
  }
  if (!mgos_apds9960_set_light_int_high_threshold(sensor, high_threshold)) {
    return false;
  }
  if (!mgos_apds9960_set_light_int_enable(sensor, true)) {
    return false;
  }

  sensor->light_handler = handler;
  return true;
}

bool mgos_apds9960_set_callback_proximity(struct mgos_apds9960 *sensor, uint16_t low_threshold, uint16_t high_threshold, mgos_apds9960_proximity_event_t handler) {
  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_enable_proximity_sensor(sensor)) {
    return false;
  }
  if (!mgos_apds9960_set_proximity_int_low_threshold(sensor, low_threshold)) {
    return false;
  }
  if (!mgos_apds9960_set_proximity_int_high_threshold(sensor, high_threshold)) {
    return false;
  }
  if (!mgos_apds9960_set_proximity_int_enable(sensor, true)) {
    return false;
  }

  sensor->proximity_handler = handler;
  return true;
}

bool mgos_apds9960_set_callback_gesture(struct mgos_apds9960 *sensor, uint16_t enter_threshold, uint16_t exit_threshold, uint8_t wait_time, mgos_apds9960_gesture_event_t handler) {
  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_enable_gesture_sensor(sensor)) {
    return false;
  }
  if (!mgos_apds9960_set_gesture_enter_threshold(sensor, enter_threshold)) {
    return false;
  }
  if (!mgos_apds9960_set_gesture_exit_threshold(sensor, exit_threshold)) {
    return false;
  }
  if (!mgos_apds9960_set_gesture_wait_time(sensor, wait_time)) {
    return false;
  }
  if (!mgos_apds9960_set_gesture_int_enable(sensor, true)) {
    return false;
  }

  sensor->gesture_handler = handler;
  return true;
}

void mgos_apds9960_irq(int pin, void *arg) {
  LOG(LL_INFO, ("Interrupt fired for APDS9960"));
  (void)pin;
  (void)arg;
}
