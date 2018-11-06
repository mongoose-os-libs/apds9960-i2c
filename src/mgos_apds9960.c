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

static bool is_apds9960(struct mgos_i2c *i2c, uint8_t i2caddr) {
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
  sensor->i2caddr = i2caddr;
  sensor->i2c     = i2c;

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

bool mgos_apds9960_getStats(struct mgos_apds9960 *sensor, struct mgos_apds9960_stats *stats) {
  if (!sensor || !stats) {
    return false;
  }

  memcpy((void *)stats, (const void *)&sensor->stats, sizeof(struct mgos_apds9960_stats));
  return true;
}

bool mgos_apds9960_i2c_init(void) {
  return true;
}
