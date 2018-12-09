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
  sensor->i2caddr           = i2caddr;
  sensor->i2c               = i2c;
  sensor->light_handler     = NULL;
  sensor->proximity_handler = NULL;
  sensor->gesture_handler   = NULL;
  mgos_apds9960_reset_gesture_data(sensor);

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
    mgos_gpio_set_int_handler(mgos_sys_config_get_apds9960_irq_pin(), MGOS_GPIO_INT_EDGE_NEG, mgos_apds9960_irq, sensor);
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
    return false;
  }

  val &= APDS9960_GVALID;

  return val == 1;
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

bool mgos_apds9960_set_callback_proximity(struct mgos_apds9960 *sensor, uint8_t low_threshold, uint8_t high_threshold, mgos_apds9960_proximity_event_t handler) {
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

bool mgos_apds9960_set_callback_gesture(struct mgos_apds9960 *sensor, mgos_apds9960_gesture_event_t handler) {
  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_enable_gesture_sensor(sensor)) {
    return false;
  }

  /*
   * if (!mgos_apds9960_set_gesture_enter_threshold(sensor, enter_threshold)) {
   * return false;
   * }
   * if (!mgos_apds9960_set_gesture_exit_threshold(sensor, exit_threshold)) {
   * return false;
   * }
   * if (!mgos_apds9960_set_gesture_wait_time(sensor, wait_time)) {
   * return false;
   * }
   */
  if (!mgos_apds9960_set_gesture_int_enable(sensor, true)) {
    return false;
  }

  sensor->gesture_handler = handler;
  return true;
}

void mgos_apds9960_reset_gesture_data(struct mgos_apds9960 *sensor) {
  uint8_t fifo[128];
  uint8_t bytes_read;

  if (!sensor) {
    return;
  }
  sensor->up_cnt    = 0;
  sensor->down_cnt  = 0;
  sensor->left_cnt  = 0;
  sensor->right_cnt = 0;
  while (mgos_apds9960_is_gesture_available(sensor)) {
    mgos_apds9960_get_gesture_fifo(sensor, fifo, &bytes_read);
    if (bytes_read > 0) {
      LOG(LL_INFO, ("Flushed %d bytes from Gesture FIFO", bytes_read));
    }
  }
  return;
}

bool mgos_apds9960_read_gesture(struct mgos_apds9960 *sensor, enum mgos_apds9960_direction_t *direction) {
  uint8_t fifo[128];
  uint8_t bytes_read;
  double  start           = 0;
  int     up_down_diff    = 0;
  int     left_right_diff = 0;
  enum mgos_apds9960_direction_t gestureReceived;

  if (!sensor || !direction) {
    return false;
  }

  start = mg_time();
  while (mgos_apds9960_is_gesture_available(sensor)) {
    double now = 0;
    gestureReceived = APDS9960_DIR_NONE;

    mgos_msleep(50);
    if (!mgos_apds9960_get_gesture_fifo(sensor, fifo, &bytes_read)) {
      LOG(LL_ERROR, ("Could not read Gesture FIFO"));
      return false;
    }

    LOG(LL_INFO, ("Read %u bytes from Gesture FIFO", bytes_read));
    for (int i = 0; i < bytes_read / 4; i++) {
      LOG(LL_INFO, ("U=%u D=%u L=%u R=%u", fifo[i*4+0], fifo[i*4+1], fifo[i*4+2], fifo[i*4+3]));
      if (abs((int)fifo[i * 4 + 0] - (int)fifo[i * 4 + 1]) > 13) {
        up_down_diff += (int)fifo[i * 4 + 0] - (int)fifo[i * 4 + 1];
      }
      if (abs((int)fifo[i * 4 + 2] - (int)fifo[i * 4 + 3]) > 13) {
        left_right_diff += (int)fifo[i * 4 + 2] - (int)fifo[i * 4 + 3];
      }
    }
    LOG(LL_INFO, ("up_down_diff=%d left_right_diff=%d", up_down_diff, left_right_diff));

    if (up_down_diff < 0) {
      if (sensor->down_cnt > 0) {
        gestureReceived = APDS9960_DIR_UP;
      } else {
        sensor->up_cnt++;
      }
    } else if (up_down_diff > 0) {
      if (sensor->up_cnt > 0) {
        gestureReceived = APDS9960_DIR_DOWN;
      } else {
        sensor->down_cnt++;
      }
    }

    if (left_right_diff < 0) {
      if (sensor->right_cnt > 0) {
        gestureReceived = APDS9960_DIR_LEFT;
      } else {
        sensor->left_cnt++;
      }
    } else if (left_right_diff > 0) {
      if (sensor->left_cnt > 0) {
        gestureReceived = APDS9960_DIR_RIGHT;
      } else {
        sensor->right_cnt++;
      }
    }

    if (gestureReceived != APDS9960_DIR_NONE) {
      mgos_apds9960_reset_gesture_data(sensor);
      *direction = gestureReceived;
      return true;
    }

    now = mg_time();
    LOG(LL_INFO, ("start=%.4f now=%.4f", start, now));
    if (now - start > (0.300)) {
      LOG(LL_INFO, ("timeout"));
      mgos_apds9960_reset_gesture_data(sensor);
      *direction = APDS9960_DIR_NONE;
      return false;
    }
  }
  LOG(LL_WARN, ("No Gesture FIFO data available"));
  return false;
}

void mgos_apds9960_irq(int pin, void *arg) {
  struct mgos_apds9960 *sensor = (struct mgos_apds9960 *)arg;
  bool light_firing            = false;
  bool proximity_firing        = false;
  bool gesture_firing          = false;

  if (!arg) {
    LOG(LL_ERROR, ("Interrupt fired for APDS9960, but no sensor to poll"));
  }

  mgos_apds9960_get_light_int(sensor, &light_firing);
  mgos_apds9960_get_proximity_int(sensor, &proximity_firing);
  mgos_apds9960_get_gesture_int(sensor, &gesture_firing);
  LOG(LL_INFO, ("Interrupt fired for APDS9960: light=%u proximity=%u gesture=%u", light_firing, proximity_firing, gesture_firing));

  if (light_firing && sensor->light_handler) {
    uint16_t clear = 0, red = 0, green = 0, blue = 0;
    mgos_apds9960_read_light(sensor, &clear, &red, &green, &blue);
    sensor->light_handler(clear, red, green, blue);
  }
  if (proximity_firing && sensor->proximity_handler) {
    uint8_t proximity;
    if (mgos_apds9960_read_proximity(sensor, &proximity)) {
      sensor->proximity_handler(proximity);
    }
  }
  if (gesture_firing && sensor->gesture_handler) {
    enum mgos_apds9960_direction_t direction = APDS9960_DIR_NONE;
    if (!mgos_apds9960_read_gesture(sensor, &direction)) {
      LOG(LL_WARN, ("Could not read gesture"));
    } else {
      sensor->gesture_handler(direction);
    }
  }

  mgos_apds9960_clear_int(sensor);
  (void)pin;
}
