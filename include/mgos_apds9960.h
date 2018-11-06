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

#pragma once
#include "mgos.h"
#include "mgos_i2c.h"

struct mgos_apds9960;
struct mgos_apds9660_stats {
  double   last_read_time;       // value of mg_time() upon last call to _read()
  uint32_t read;                 // calls to _read()
  uint32_t read_success;         // successful _read()
  uint32_t read_success_cached;  // calls to _read() which were cached
  // Note: read_errors := read - read_success - read_success_cached
  double   read_success_usecs;   // time spent in successful uncached _read()
};

/*
 * Initialize a APDS9660 on the I2C bus `i2c` at address specified in `i2caddr`
 * parameter (default APDS9660 is on address 0x39). The sensor will be polled for
 * validity, upon success a new `struct mgos_apds9960` is allocated and
 * returned. If the device could not be found, NULL is returned.
 */
struct mgos_apds9960 *mgos_apds9960_create(struct mgos_i2c *i2c, uint8_t i2caddr);

/*
 * Destroy the data structure associated with a APDS9660 device. The reference
 * to the pointer of the `struct mgos_apds9960` has to be provided, and upon
 * successful destruction, its associated memory will be freed and the pointer
 * set to NULL.
 */
void mgos_apds9960_destroy(struct mgos_apds9960 **sensor);

/*
 * Returns the running statistics on the sensor interaction, the user provides
 * a pointer to a `struct mgos_apds9660_stats` object, which is filled in by this
 * call.
 *
 * Upon success, true is returned. Otherwise, false is returned, in which case
 * the contents of `stats` is undetermined.
 */
bool mgos_apds9660_getStats(struct mgos_apds9660 *sensor, struct mgos_apds9660_stats *stats);
