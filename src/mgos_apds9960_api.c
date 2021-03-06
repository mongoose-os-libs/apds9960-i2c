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

/* Start sparkfun import */
bool mgos_apds9960_get_led_boost(struct mgos_apds9960 *sensor, uint8_t *boost) {
  if (!sensor || !boost) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONFIG2, boost)) {
    return false;
  }

  *boost = (*boost >> 4) & 0b00000011;

  return true;
}

bool mgos_apds9960_set_led_boost(struct mgos_apds9960 *sensor, uint8_t boost) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONFIG2, &val)) {
    return false;
  }

  boost &= 0b00000011;
  boost  = boost << 4;
  val   &= 0b11001111;
  val   |= boost;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONFIG2, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_proximity_gain_comp_enable(struct mgos_apds9960 *sensor, bool *enabled) {
  uint8_t val;

  if (!sensor || !enabled) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONFIG3, &val)) {
    return false;
  }

  *enabled = (val >> 5) & 0b00000001;

  return true;
}

bool mgos_apds9960_set_proximity_gain_comp_enable(struct mgos_apds9960 *sensor, bool enable) {
  uint8_t val;
  uint8_t enable_val;

  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONFIG3, &val)) {
    return false;
  }

  enable_val = enable << 5;
  val       &= 0b11011111;
  val       |= enable_val;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONFIG3, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_proximity_photomask(struct mgos_apds9960 *sensor, uint8_t *mask) {
  if (!sensor || !mask) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONFIG3, mask)) {
    return false;
  }

  *mask &= 0b00001111;
  return true;
}

bool mgos_apds9960_set_proximity_photomask(struct mgos_apds9960 *sensor, uint8_t mask) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONFIG3, &val)) {
    return false;
  }

  mask &= 0b00001111;
  val  &= 0b11110000;
  val  |= mask;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONFIG3, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_gesture_enter_threshold(struct mgos_apds9960 *sensor, uint8_t *threshold) {
  if (!sensor || !threshold) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GPENTH, threshold)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_set_gesture_enter_threshold(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GPENTH, threshold)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_gesture_exit_threshold(struct mgos_apds9960 *sensor, uint8_t *threshold) {
  if (!sensor || !threshold) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GEXTH, threshold)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_set_gesture_exit_threshold(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GEXTH, threshold)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_gesture_wait_time(struct mgos_apds9960 *sensor, uint8_t *time) {
  if (!sensor || !time) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF2, time)) {
    return false;
  }

  *time &= 0b00000111;
  return true;
}

bool mgos_apds9960_set_gesture_wait_time(struct mgos_apds9960 *sensor, uint8_t time) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF2, &val)) {
    return false;
  }

  time &= 0b00000111;
  val  &= 0b11111000;
  val  |= time;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GCONF2, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_gesture_mode(struct mgos_apds9960 *sensor, uint8_t *mode) {
  if (!sensor || !mode) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF4, mode)) {
    return false;
  }

  *mode &= 0b00000001;
  return true;
}

bool mgos_apds9960_set_gesture_mode(struct mgos_apds9960 *sensor, uint8_t mode) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF4, &val)) {
    return false;
  }

  mode &= 0b00000001;
  val  &= 0b11111110;
  val  |= mode;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GCONF4, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_init(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return false;
  }

  // Reset device first: power down and disable all features
  if (!mgos_apds9960_disable(sensor)) {
    return false;
  }

  // Set default values for ambient light and proximity registers
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_ATIME, APDS9960_DEFAULT_ATIME)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_WTIME, APDS9960_DEFAULT_WTIME)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_PPULSE, APDS9960_DEFAULT_PROX_PPULSE)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_POFFSET_UR, APDS9960_DEFAULT_POFFSET_UR)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_POFFSET_DL, APDS9960_DEFAULT_POFFSET_DL)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONFIG1, APDS9960_DEFAULT_CONFIG1)) {
    return false;
  }
  if (!mgos_apds9960_set_led_drive(sensor, APDS9960_DEFAULT_LDRIVE)) {
    return false;
  }
  if (!mgos_apds9960_set_proximity_gain(sensor, APDS9960_DEFAULT_PGAIN)) {
    return false;
  }
  if (!mgos_apds9960_set_light_gain(sensor, APDS9960_DEFAULT_AGAIN)) {
    return false;
  }
  if (!mgos_apds9960_set_proximity_int_low_threshold(sensor, APDS9960_DEFAULT_PILT)) {
    return false;
  }
  if (!mgos_apds9960_set_proximity_int_high_threshold(sensor, APDS9960_DEFAULT_PIHT)) {
    return false;
  }
  if (!mgos_apds9960_set_light_int_low_threshold(sensor, APDS9960_DEFAULT_AILT)) {
    return false;
  }
  if (!mgos_apds9960_set_light_int_high_threshold(sensor, APDS9960_DEFAULT_AIHT)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_PERS, APDS9960_DEFAULT_PERS)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONFIG2, APDS9960_DEFAULT_CONFIG2)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONFIG3, APDS9960_DEFAULT_CONFIG3)) {
    return false;
  }

  // Set default values for gesture sense registers
  if (!mgos_apds9960_set_gesture_enter_threshold(sensor, APDS9960_DEFAULT_GPENTH)) {
    return false;
  }
  if (!mgos_apds9960_set_gesture_exit_threshold(sensor, APDS9960_DEFAULT_GEXTH)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GCONF1, APDS9960_DEFAULT_GCONF1)) {
    return false;
  }
  if (!mgos_apds9960_set_gesture_gain(sensor, APDS9960_DEFAULT_GGAIN)) {
    return false;
  }
  if (!mgos_apds9960_set_gesture_led_drive(sensor, APDS9960_DEFAULT_GLDRIVE)) {
    return false;
  }
  if (!mgos_apds9960_set_gesture_wait_time(sensor, APDS9960_DEFAULT_GWTIME)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GOFFSET_U, APDS9960_DEFAULT_GOFFSET)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GOFFSET_D, APDS9960_DEFAULT_GOFFSET)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GOFFSET_L, APDS9960_DEFAULT_GOFFSET)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GOFFSET_R, APDS9960_DEFAULT_GOFFSET)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GPULSE, APDS9960_DEFAULT_GPULSE)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GCONF3, APDS9960_DEFAULT_GCONF3)) {
    return false;
  }
  if (!mgos_apds9960_set_gesture_int_enable(sensor, APDS9960_DEFAULT_GIEN)) {
    return false;
  }

  // Turn on the sensor power
  return mgos_apds9960_enable(sensor);
}

bool mgos_apds9960_enable(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return false;
  }
  return mgos_apds9960_set_mode(sensor, APDS9960_POWER, APDS9960_ON);
}

bool mgos_apds9960_disable(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return false;
  }

  // Set ENABLE register to 0 (disable all features)
  if (!mgos_apds9960_set_mode(sensor, APDS9960_ALL, APDS9960_OFF)) {
    return false;
  }

  return mgos_apds9960_set_mode(sensor, APDS9960_POWER, APDS9960_OFF);
}

bool mgos_apds9960_get_mode(struct mgos_apds9960 *sensor, uint8_t *mode) {
  if (!sensor || !mode) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ENABLE, mode)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_set_mode(struct mgos_apds9960 *sensor, uint8_t mode, uint8_t enable) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_get_mode(sensor, &val)) {
    return false;
  }

  enable = enable & 0x01;
  if (mode <= 6) {
    if (enable) {
      val |= (1 << mode);
    } else {
      val &= ~(1 << mode);
    }
  } else if (mode == APDS9960_ALL) {
    if (enable) {
      val = 0x7F;
    } else {
      val = 0x00;
    }
  }

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_ENABLE, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_enable_light_sensor(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_set_light_gain(sensor, APDS9960_DEFAULT_AGAIN)) {
    return false;
  }
  if (!mgos_apds9960_enable(sensor)) {
    return false;
  }
  if (!mgos_apds9960_set_mode(sensor, APDS9960_AMBIENT_LIGHT, 1)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_disable_light_sensor(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return false;
  }
  if (!mgos_apds9960_set_light_int_enable(sensor, false)) {
    return false;
  }
  if (!mgos_apds9960_set_mode(sensor, APDS9960_AMBIENT_LIGHT, 0)) {
    return false;
  }
  return true;
}

bool mgos_apds9960_enable_proximity_sensor(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_set_proximity_gain(sensor, APDS9960_DEFAULT_PGAIN)) {
    return false;
  }
  if (!mgos_apds9960_set_led_drive(sensor, APDS9960_DEFAULT_LDRIVE)) {
    return false;
  }
  if (!mgos_apds9960_enable(sensor)) {
    return false;
  }
  if (!mgos_apds9960_set_mode(sensor, APDS9960_PROXIMITY, 1)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_disable_proximity_sensor(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return false;
  }
  if (!mgos_apds9960_set_proximity_int_enable(sensor, 0)) {
    return false;
  }
  if (!mgos_apds9960_set_mode(sensor, APDS9960_PROXIMITY, 0)) {
    return false;
  }
  return true;
}

bool mgos_apds9960_enable_gesture_sensor(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return false;
  }

  /* Enable gesture mode
   * Set ENABLE to 0 (power off)
   * Set WTIME to 0xFF
   * Set AUX to LED_BOOST_300
   * Enable PON, WEN, PEN, GEN in ENABLE
   */
  mgos_apds9960_reset_gesture_data(sensor);
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_WTIME, 0xFF)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_PPULSE, APDS9960_DEFAULT_GESTURE_PPULSE)) {
    return false;
  }
  if (!mgos_apds9960_set_led_boost(sensor, APDS9960_LED_BOOST_300)) {
    return false;
  }
  if (!mgos_apds9960_set_gesture_mode(sensor, 1)) {
    return false;
  }
  if (!mgos_apds9960_enable(sensor)) {
    return false;
  }
  if (!mgos_apds9960_set_mode(sensor, APDS9960_WAIT, 1)) {
    return false;
  }
  if (!mgos_apds9960_set_mode(sensor, APDS9960_PROXIMITY, 1)) {
    return false;
  }
  if (!mgos_apds9960_set_mode(sensor, APDS9960_GESTURE, 1)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_disable_gesture_sensor(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return false;
  }
  mgos_apds9960_reset_gesture_data(sensor);
  if (!mgos_apds9960_set_gesture_int_enable(sensor, 0)) {
    return false;
  }
  if (!mgos_apds9960_set_gesture_mode(sensor, 0)) {
    return false;
  }
  if (!mgos_apds9960_set_mode(sensor, APDS9960_GESTURE, 0)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_led_drive(struct mgos_apds9960 *sensor, uint8_t *drive) {
  if (!sensor || !drive) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONTROL, drive)) {
    return false;
  }

  *drive = (*drive >> 6) & 0b00000011;
  return true;
}

bool mgos_apds9960_set_led_drive(struct mgos_apds9960 *sensor, uint8_t drive) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONTROL, &val)) {
    return false;
  }

  drive &= 0b00000011;
  drive  = drive << 6;
  val   &= 0b00111111;
  val   |= drive;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONTROL, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_gesture_led_drive(struct mgos_apds9960 *sensor, bool *enabled) {
  uint8_t val;

  if (!sensor || !enabled) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF2, &val)) {
    return false;
  }

  *enabled = (val >> 3) & 0b00000011;
  return true;
}

bool mgos_apds9960_set_gesture_led_drive(struct mgos_apds9960 *sensor, bool enable) {
  uint8_t val;
  uint8_t enable_val;

  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF2, &val)) {
    return false;
  }

  enable_val = enable << 3;
  val       &= 0b11100111;
  val       |= enable_val;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GCONF2, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_light_gain(struct mgos_apds9960 *sensor, uint8_t *gain) {
  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONTROL, gain)) {
    return false;
  }

  *gain &= 0b00000011;
  return true;
}

bool mgos_apds9960_set_light_gain(struct mgos_apds9960 *sensor, uint8_t gain) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONTROL, &val)) {
    return false;
  }

  gain &= 0b00000011;
  val  &= 0b11111100;
  val  |= gain;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONTROL, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_proximity_gain(struct mgos_apds9960 *sensor, uint8_t *gain) {
  if (!sensor || !gain) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONTROL, gain)) {
    return false;
  }

  *gain = (*gain >> 2) & 0b00000011;
  return true;
}

bool mgos_apds9960_set_proximity_gain(struct mgos_apds9960 *sensor, uint8_t gain) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONTROL, &val)) {
    return false;
  }

  gain &= 0b00000011;
  gain  = gain << 2;
  val  &= 0b11110011;
  val  |= gain;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONTROL, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_gesture_gain(struct mgos_apds9960 *sensor, uint8_t *gain) {
  if (!sensor || !gain) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF2, gain)) {
    return false;
  }

  *gain = (*gain >> 5) & 0b00000011;
  return true;
}

bool mgos_apds9960_set_gesture_gain(struct mgos_apds9960 *sensor, uint8_t gain) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF2, &val)) {
    return false;
  }

  gain &= 0b00000011;
  gain  = gain << 5;
  val  &= 0b10011111;
  val  |= gain;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GCONF2, val)) {
    return false;
  }
  return true;
}

bool mgos_apds9960_get_light_int_low_threshold(struct mgos_apds9960 *sensor, uint16_t *threshold) {
  uint8_t val;

  if (!sensor || !threshold) {
    return false;
  }
  *threshold = 0;

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_AILTL, &val)) {
    return false;
  }
  *threshold = val;

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_AILTH, &val)) {
    return false;
  }
  *threshold += ((uint16_t)val << 8);

  return true;
}

bool mgos_apds9960_set_light_int_low_threshold(struct mgos_apds9960 *sensor, uint16_t threshold) {
  uint8_t val_low;
  uint8_t val_high;

  if (!sensor) {
    return false;
  }

  // Break 16-bit threshold into 2 8-bit values
  val_low  = threshold & 0x00FF;
  val_high = (threshold & 0xFF00) >> 8;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_AILTL, val_low)) {
    return false;
  }

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_AILTH, val_high)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_light_int_high_threshold(struct mgos_apds9960 *sensor, uint16_t *threshold) {
  uint8_t val;

  if (!sensor || !threshold) {
    return false;
  }

  *threshold = 0;

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_AIHTL, &val)) {
    return false;
  }
  *threshold = val;

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_AIHTH, &val)) {
    return false;
  }
  *threshold += ((uint16_t)val << 8);

  return true;
}

bool mgos_apds9960_set_light_int_high_threshold(struct mgos_apds9960 *sensor, uint16_t threshold) {
  uint8_t val_low;
  uint8_t val_high;

  if (!sensor) {
    return false;
  }

  // Break 16-bit threshold into 2 8-bit values
  val_low  = threshold & 0x00FF;
  val_high = (threshold & 0xFF00) >> 8;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_AIHTL, val_low)) {
    return false;
  }

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_AIHTH, val_high)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_proximity_int_low_threshold(struct mgos_apds9960 *sensor, uint8_t *threshold) {
  if (!sensor || !threshold) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_PILT, threshold)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_set_proximity_int_low_threshold(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_PILT, threshold)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_proximity_int_high_threshold(struct mgos_apds9960 *sensor, uint8_t *threshold) {
  if (!sensor || !threshold) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_PIHT, threshold)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_set_proximity_int_high_threshold(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_PIHT, threshold)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_light_int_enable(struct mgos_apds9960 *sensor, bool *enabled) {
  uint8_t val;

  if (!sensor || !enabled) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ENABLE, &val)) {
    return false;
  }

  *enabled = (val >> 4) & 0b00000001;
  return true;
}

bool mgos_apds9960_set_light_int_enable(struct mgos_apds9960 *sensor, bool enable) {
  uint8_t val;
  uint8_t enable_val;

  if (!sensor) {
    return false;
  }
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ENABLE, &val)) {
    return false;
  }

  enable_val = enable << 4;
  val       &= 0b11101111;
  val       |= enable_val;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_ENABLE, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_proximity_int_enable(struct mgos_apds9960 *sensor, bool *enabled) {
  uint8_t val;

  if (!sensor || !enabled) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ENABLE, &val)) {
    return false;
  }

  *enabled = (val >> 5) & 0b00000001;
  return true;
}

bool mgos_apds9960_set_proximity_int_enable(struct mgos_apds9960 *sensor, bool enable) {
  uint8_t val;
  uint8_t enable_val;

  if (!sensor) {
    return false;
  }
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ENABLE, &val)) {
    return false;
  }

  enable_val = enable << 5;
  val       &= 0b11011111;
  val       |= enable_val;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_ENABLE, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_gesture_int(struct mgos_apds9960 *sensor, bool *firing) {
  uint8_t val;

  if (!sensor || !firing) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_STATUS, &val)) {
    return false;
  }

  *firing = (val >> 2) & 0b00000001;
  return true;
}

bool mgos_apds9960_get_gesture_int_enable(struct mgos_apds9960 *sensor, bool *enabled) {
  uint8_t val;

  if (!sensor || !enabled) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ENABLE, &val)) {
    return false;
  }

  *enabled = (val >> 1) & 0b00000001;
  return true;
}

bool mgos_apds9960_set_gesture_int_enable(struct mgos_apds9960 *sensor, bool enable) {
  uint8_t val;
  uint8_t enable_val;

  if (!sensor) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF4, &val)) {
    return false;
  }

  enable_val = enable << 1;
  val       &= 0b11111101;
  val       |= enable_val;

  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GCONF4, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_light_int(struct mgos_apds9960 *sensor, bool *firing) {
  uint8_t val;

  if (!sensor || !firing) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_STATUS, &val)) {
    return false;
  }

  *firing = (val >> 4) & 0b00000001;
  return true;
}

bool mgos_apds9960_clear_int(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return false;
  }
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_AICLEAR, &val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_proximity_int(struct mgos_apds9960 *sensor, bool *firing) {
  uint8_t val;

  if (!sensor || !firing) {
    return false;
  }

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_STATUS, &val)) {
    return false;
  }

  *firing = (val >> 5) & 0b00000001;
  return true;
}

bool mgos_apds9960_read_ambient_light(struct mgos_apds9960 *sensor, uint16_t *val) {
  uint8_t val_byte;

  if (!sensor || !val) {
    return false;
  }
  *val = 0;

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CDATAL, &val_byte)) {
    return false;
  }
  *val = val_byte;

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CDATAH, &val_byte)) {
    return false;
  }
  *val += ((uint16_t)val_byte << 8);

  return true;
}

bool mgos_apds9960_read_red_light(struct mgos_apds9960 *sensor, uint16_t *val) {
  uint8_t val_byte;

  if (!sensor || !val) {
    return false;
  }
  *val = 0;

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_RDATAL, &val_byte)) {
    return false;
  }
  *val = val_byte;

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_RDATAH, &val_byte)) {
    return false;
  }
  *val += ((uint16_t)val_byte << 8);

  return true;
}

bool mgos_apds9960_read_green_light(struct mgos_apds9960 *sensor, uint16_t *val) {
  uint8_t val_byte;

  if (!sensor || !val) {
    return false;
  }
  *val = 0;

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GDATAL, &val_byte)) {
    return false;
  }
  *val = val_byte;

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GDATAH, &val_byte)) {
    return false;
  }
  *val += ((uint16_t)val_byte << 8);

  return true;
}

bool mgos_apds9960_read_blue_light(struct mgos_apds9960 *sensor, uint16_t *val) {
  uint8_t val_byte;

  if (!sensor || !val) {
    return false;
  }
  *val = 0;

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_BDATAL, &val_byte)) {
    return false;
  }
  *val = val_byte;

  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_BDATAH, &val_byte)) {
    return false;
  }
  *val += ((uint16_t)val_byte << 8);

  return true;
}

/* End sparkfun import */

// Fifo size is 32 tuples of 4 bytes -- so *fifo must be at least 128 bytes!
bool mgos_apds9960_get_gesture_fifo(struct mgos_apds9960 *sensor, uint8_t *fifo, uint8_t *bytes_read) {
  uint8_t fifo_level;
  int     readlen;

  if (!sensor || !fifo || !bytes_read) {
    return false;
  }
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GFLVL, &fifo_level)) {
    return false;
  }
  if (fifo_level == 0) {
    return false;
  }

  readlen = mgos_apds9960_wireReadDataBlock(sensor, APDS9960_GFIFO_U, fifo, (fifo_level * 4));
  if (readlen < 1) {
    return false;
  }
  *bytes_read = readlen;
  return true;
}

bool mgos_apds9960_i2c_init(void) {
  return true;
}
