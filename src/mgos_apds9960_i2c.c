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

bool mgos_apds9960_wireWriteByte(struct mgos_apds9960 *sensor, uint8_t val) {
  if (!sensor) {
    return false;
  }

  return mgos_i2c_write(sensor->i2c, sensor->i2caddr, &val, 1, true);
}

bool mgos_apds9960_wireWriteDataByte(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t val) {
  if (!sensor) {
    return false;
  }

  return mgos_i2c_write_reg_b(sensor->i2c, sensor->i2caddr, reg, val);
}

bool mgos_apds9960_wireWriteDataBlock(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t *val, unsigned int len) {
  if (!sensor || !val) {
    return false;
  }

  if (!mgos_i2c_write(sensor->i2c, sensor->i2caddr, &reg, 1, true)) {
    mgos_i2c_stop(sensor->i2c);
    return false;
  }

  return mgos_i2c_write(sensor->i2c, sensor->i2caddr, val, len, true);
}

bool mgos_apds9960_wireReadDataByte(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t *val) {
  int ret;

  if (!sensor || !val) {
    return false;
  }

  ret = mgos_i2c_read_reg_b(sensor->i2c, sensor->i2caddr, reg);
  if (ret < 0) {
    mgos_i2c_stop(sensor->i2c);
    return false;
  }

  *val = (uint8_t)ret;
  return true;
}

int mgos_apds9960_wireReadDataBlock(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t *val, unsigned int len) {
  if (!sensor) {
    return -1;
  }

  if (!mgos_i2c_write(sensor->i2c, sensor->i2caddr, &reg, 1, true)) {
    mgos_i2c_stop(sensor->i2c);
    return -1;
  }

  if (!mgos_i2c_read(sensor->i2c, sensor->i2caddr, val, len, true)) {
    mgos_i2c_stop(sensor->i2c);
    return -1;
  }

  return len;
}
