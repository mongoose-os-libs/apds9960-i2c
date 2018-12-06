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
#include "mgos_apds9960_api.h"

struct mgos_apds9960;
struct mgos_apds9960_stats {
  double   last_read_time;       // value of mg_time() upon last call to _read()
  uint32_t read;                 // calls to _read()
  uint32_t read_success;         // successful _read()
  uint32_t read_success_cached;  // calls to _read() which were cached
  // Note: read_errors := read - read_success - read_success_cached
  double   read_success_usecs;   // time spent in successful uncached _read()
};

/* Direction definitions */
enum mgos_apds9960_direction_t {
  APDS9960_DIR_NONE,
  APDS9960_DIR_LEFT,
  APDS9960_DIR_RIGHT,
  APDS9960_DIR_UP,
  APDS9960_DIR_DOWN,
  APDS9960_DIR_NEAR,
  APDS9960_DIR_FAR,
  APDS9960_DIR_ALL
};

enum mgos_apds9960_state_t {
  APDS9960_NA_STATE,
  APDS9960_NEAR_STATE,
  APDS9960_FAR_STATE,
  APDS9960_ALL_STATE
};


// Callback handlers
typedef void (*mgos_apds9960_light_event_t)(uint16_t clear, uint16_t red, uint16_t green, uint16_t blue);
typedef void (*mgos_apds9960_proximity_event_t)(uint8_t proximity);
typedef void (*mgos_apds9960_gesture_event_t)(enum mgos_apds9960_direction_t direction);

/*
 * Initialize a APDS9960 on the I2C bus `i2c` at address specified in `i2caddr`
 * parameter (default APDS9960 is on address 0x39). The sensor will be polled for
 * validity, upon success a new `struct mgos_apds9960` is allocated and
 * returned. If the device could not be found, NULL is returned.
 */
struct mgos_apds9960 *mgos_apds9960_create(struct mgos_i2c *i2c, uint8_t i2caddr);

/*
 * Destroy the data structure associated with a APDS9960 device. The reference
 * to the pointer of the `struct mgos_apds9960` has to be provided, and upon
 * successful destruction, its associated memory will be freed and the pointer
 * set to NULL.
 */
void mgos_apds9960_destroy(struct mgos_apds9960 **sensor);

bool mgos_apds9960_set_callback_light(struct mgos_apds9960 *sensor, uint16_t low_threshold, uint16_t high_threshold, mgos_apds9960_light_event_t handler);
bool mgos_apds9960_set_callback_proximity(struct mgos_apds9960 *sensor, uint16_t low_threshold, uint16_t high_threshold, mgos_apds9960_proximity_event_t handler);
bool mgos_apds9960_set_callback_gesture(struct mgos_apds9960 *sensor, uint16_t low_threshold, uint16_t high_threshold, mgos_apds9960_gesture_event_t handler);

bool mgos_apds9960_read_light(struct mgos_apds9960 *sensor, uint16_t *clear, uint16_t *red, uint16_t *green, uint16_t *blue);
bool mgos_apds9960_read_proximity(struct mgos_apds9960 *sensor, uint8_t *val);

bool mgos_apds9960_is_gesture_available(struct mgos_apds9960 *sensor);
int mgos_apds9960_read_gesture(struct mgos_apds9960 *sensor);

/*
 * Returns the running statistics on the sensor interaction, the user provides
 * a pointer to a `struct mgos_apds9960_stats` object, which is filled in by this
 * call.
 *
 * Upon success, true is returned. Otherwise, false is returned, in which case
 * the contents of `stats` is undetermined.
 */
bool mgos_apds9960_get_stats(struct mgos_apds9960 *sensor, struct mgos_apds9960_stats *stats);
