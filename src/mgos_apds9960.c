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

/*static*/  bool is_apds9960(struct mgos_i2c *i2c, uint8_t i2caddr) {
  return false;

  (void)i2c;
  (void)i2caddr;
}

struct mgos_apds9960 *mgos_apds9960_create(struct mgos_i2c *i2c, uint8_t i2caddr) {
  struct mgos_apds9960 *sensor;

  if (!i2c) {
    return NULL;
  }

  if (!is_apds9960(i2c, i2caddr)) {
    LOG(LL_ERROR, ("Failed to create APDS9960 at I2C 0x%02x", i2caddr));
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
  sensor->gesture_motion_     = DIR_NONE;

  LOG(LL_INFO, ("APDS9960 created at I2C 0x%02x", i2caddr));
  return sensor;
}

void mgos_apds9960_destroy(struct mgos_apds9960 **sensor) {
  if (!*sensor) {
    return;
  }

  free(*sensor);
  *sensor = NULL;
  return;
}

bool mgos_apds9960_get_stats(struct mgos_apds9960 *sensor, struct mgos_apds9960_stats *stats) {
  if (!sensor || !stats) {
    return false;
  }

  memcpy((void *)stats, (const void *)&sensor->stats, sizeof(struct mgos_apds9960_stats));
  return true;
}

/* Start sparkfun import */
/*static*/  void mgos_apds9960_resetGestureParameters(struct mgos_apds9960 *sensor) {
  if (!sensor) return;
  return;
}

/*static*/  bool mgos_apds9960_processGestureData(struct mgos_apds9960 *sensor) {
  if (!sensor) return false;
  return false;
}

/*static*/  bool mgos_apds9960_decodeGesture(struct mgos_apds9960 *sensor) {
  if (!sensor) return false;
  return false;
}

/*static*/  uint8_t mgos_apds9960_getProxIntLowThresh(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

/*static*/  bool mgos_apds9960_setProxIntLowThresh(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) return false;
  return false;

  (void) threshold;
}

/*static*/  uint8_t mgos_apds9960_getProxIntHighThresh(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

/*static*/  bool mgos_apds9960_setProxIntHighThresh(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) return false;
  return false;

  (void) threshold;
}

/*static*/  uint8_t mgos_apds9960_getLEDBoost(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

/*static*/  bool mgos_apds9960_setLEDBoost(struct mgos_apds9960 *sensor, uint8_t boost) {
  if (!sensor) return false;
  return false;

  (void) boost;
}

/*static*/  uint8_t mgos_apds9960_getProxGainCompEnable(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

/*static*/  bool mgos_apds9960_setProxGainCompEnable(struct mgos_apds9960 *sensor, uint8_t enable) {
  if (!sensor) return false;
  return false;

  (void) enable;
}

/*static*/  uint8_t mgos_apds9960_getProxPhotoMask(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

/*static*/  bool mgos_apds9960_setProxPhotoMask(struct mgos_apds9960 *sensor, uint8_t mask) {
  if (!sensor) return false;
  return false;

  (void) mask;
}

/*static*/  uint8_t mgos_apds9960_getGestureEnterThresh(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

/*static*/  bool mgos_apds9960_setGestureEnterThresh(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) return false;
  return false;

  (void) threshold;
}

/*static*/  uint8_t mgos_apds9960_getGestureExitThresh(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

/*static*/  bool mgos_apds9960_setGestureExitThresh(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) return false;
  return false;

  (void) threshold;
}

/*static*/  uint8_t mgos_apds9960_getGestureWaitTime(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

/*static*/  bool mgos_apds9960_setGestureWaitTime(struct mgos_apds9960 *sensor, uint8_t time) {
  if (!sensor) return false;
  return false;

  (void) time;
}

/*static*/  uint8_t mgos_apds9960_getGestureMode(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

/*static*/  bool mgos_apds9960_setGestureMode(struct mgos_apds9960 *sensor, uint8_t mode) {
  if (!sensor) return false;
  return false;

  (void) mode;
}

/*static*/  bool mgos_apds9960_wireWriteByte(struct mgos_apds9960 *sensor, uint8_t val) {
  if (!sensor) return false;
  return false;

  (void) val;
}

/*static*/  bool mgos_apds9960_wireWriteDataByte(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t val) {
  if (!sensor) return false;
  return false;

  (void) reg;
  (void) val;
}

/*static*/  bool mgos_apds9960_wireWriteDataBlock(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t *val, unsigned int len) {
  if (!sensor) return false;
  return false;

  (void) reg;
  (void) val;
  (void) len;
}

/*static*/  bool mgos_apds9960_wireReadDataByte(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t *val) {
  if (!sensor) return false;
  return false;

  (void) reg;
  (void) val;
}

/*static*/  int mgos_apds9960_wireReadDataBlock(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t *val, unsigned int len) {
  if (!sensor) return -1;
  return -1;

  (void) reg;
  (void) val;
  (void) len;
}

void mgos_apds9960_init(struct mgos_apds9960 *sensor) {
  if (!sensor) return;
  return;
}

void mgos_apds9960_enable(struct mgos_apds9960 *sensor) {
  if (!sensor) return;
  return;
}

void mgos_apds9960_disable(struct mgos_apds9960 *sensor) {
  if (!sensor) return;
  return;
}

uint8_t mgos_apds9960_get_mode(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

bool mgos_apds9960_set_mode(struct mgos_apds9960 *sensor, uint8_t mode, uint8_t enable) {
  if (!sensor) return false;
  return false;

  (void) mode;
  (void) enable;
}

bool mgos_apds9960_enable_light_sensor(struct mgos_apds9960 *sensor, bool interrupts) {
  if (!sensor) return false;
  return false;

  (void) interrupts;
}

bool mgos_apds9960_disable_light_sensor(struct mgos_apds9960 *sensor, bool interrupts) {
  if (!sensor) return false;
  return false;

  (void) interrupts;
}

bool mgos_apds9960_enable_proximity_sensor(struct mgos_apds9960 *sensor, bool interrupts) {
  if (!sensor) return false;
  return false;

  (void) interrupts;
}

bool mgos_apds9960_disable_proximity_sensor(struct mgos_apds9960 *sensor, bool interrupts) {
  if (!sensor) return false;
  return false;

  (void) interrupts;
}

bool mgos_apds9960_enable_gesture_sensor(struct mgos_apds9960 *sensor, bool interrupts) {
  if (!sensor) return false;
  return false;

  (void) interrupts;
}

bool mgos_apds9960_disable_gesture_sensor(struct mgos_apds9960 *sensor, bool interrupts) {
  if (!sensor) return false;
  return false;

  (void) interrupts;
}

uint8_t mgos_apds9960_get_led_drive(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

bool mgos_apds9960_set_led_drive(struct mgos_apds9960 *sensor, uint8_t drive) {
  if (!sensor) return false;
  return false;

  (void) drive;
}

uint8_t mgos_apds9960_get_gesture_led_drive(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

bool mgos_apds9960_set_gesture_led_drive(struct mgos_apds9960 *sensor, uint8_t drive) {
  if (!sensor) return false;
  return false;

  (void) drive;
}

bool mgos_apds9960_get_light_int_low_threshold(struct mgos_apds9960 *sensor, uint16_t *threshold) {
  if (!sensor) return false;
  return false;

  (void) threshold;
}

bool mgos_apds9960_set_light_int_low_threshold(struct mgos_apds9960 *sensor, uint16_t threshold) {
  if (!sensor) return false;
  return false;

  (void) threshold;
}

bool mgos_apds9960_get_light_int_high_threshold(struct mgos_apds9960 *sensor, uint16_t *threshold) {
  if (!sensor) return false;
  return false;

  (void) threshold;
}

bool mgos_apds9960_set_light_int_high_threshold(struct mgos_apds9960 *sensor, uint16_t threshold) {
  if (!sensor) return false;
  return false;

  (void) threshold;
}

bool mgos_apds9960_get_proximity_int_low_threshold(struct mgos_apds9960 *sensor, uint8_t *threshold) {
  if (!sensor) return false;
  return false;

  (void) threshold;
}

bool mgos_apds9960_set_proximity_int_low_threshold(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) return false;
  return false;

  (void) threshold;
}

bool mgos_apds9960_get_proximity_int_high_threshold(struct mgos_apds9960 *sensor, uint8_t *threshold) {
  if (!sensor) return false;
  return false;

  (void) threshold;
}

bool mgos_apds9960_set_proximity_int_high_threshold(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) return false;
  return false;

  (void) threshold;
}

uint8_t mgos_apds9960_get_ambient_light_int_enable(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

bool mgos_apds9960_set_ambient_light_int_enable(struct mgos_apds9960 *sensor, uint8_t enable) {
  if (!sensor) return false;
  return false;

  (void) enable;
}

uint8_t mgos_apds9960_get_proximity_int_enable(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

bool mgos_apds9960_set_proximity_int_enable(struct mgos_apds9960 *sensor, uint8_t enable) {
  if (!sensor) return false;
  return false;

  (void) enable;
}

uint8_t mgos_apds9960_get_gesture_int_enable(struct mgos_apds9960 *sensor) {
  if (!sensor) return 0;
  return 0;
}

bool mgos_apds9960_set_gesture_int_enable(struct mgos_apds9960 *sensor, uint8_t enable) {
  if (!sensor) return false;
  return false;

  (void) enable;
}

bool mgos_apds9960_clear_ambientlight_int(struct mgos_apds9960 *sensor) {
  if (!sensor) return false;
  return false;
}

bool mgos_apds9960_clear_proximity_int(struct mgos_apds9960 *sensor) {
  if (!sensor) return false;
  return false;
}


bool mgos_apds9960_read_ambient_light(struct mgos_apds9960 *sensor, uint16_t *val) {
  if (!sensor) return false;
  return false;

  (void) val;
}

bool mgos_apds9960_read_red_light(struct mgos_apds9960 *sensor, uint16_t *val) {
  if (!sensor) return false;
  return false;

  (void) val;
}

bool mgos_apds9960_read_green_light(struct mgos_apds9960 *sensor, uint16_t *val) {
  if (!sensor) return false;
  return false;

  (void) val;
}

bool mgos_apds9960_read_blue_light(struct mgos_apds9960 *sensor, uint16_t *val) {
  if (!sensor) return false;
  return false;

  (void) val;
}


bool mgos_apds9960_read_proximity(struct mgos_apds9960 *sensor, uint8_t *val) {
  if (!sensor) return false;
  return false;

  (void) val;
}

bool mgos_apds9960_is_gesture_available(struct mgos_apds9960 *sensor) {
  if (!sensor) return false;
  return false;
}

int mgos_apds9960_read_gesture(struct mgos_apds9960 *sensor) {
  if (!sensor) return -1;
  return -1;
}
/* End sparkfun import */

bool mgos_apds9960_i2c_init(void) {
  return true;
}
