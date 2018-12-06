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
#include "mgos_gpio.h"
#include "mgos_apds9960_api.h"

struct mgos_apds9960;

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
bool mgos_apds9960_set_callback_gesture(struct mgos_apds9960 *sensor, mgos_apds9960_gesture_event_t handler);

/*
 * Read the clear (ambient), red, green, and blue light values from the sensor.
 * Lower values mean less light was detected. The arguments clear, red, green
 * and blue may be NULL, in which case they are not polled from the sensor.
 * Returns true on success, or false otherwise. If false is returned, the
 * values of clear, red, green and blue cannot be used.
 */
bool mgos_apds9960_read_light(struct mgos_apds9960 *sensor, uint16_t *clear, uint16_t *red, uint16_t *green, uint16_t *blue);

/*
 * Read the proximity value from the sensor. Lower values mean further away,
 * higher values mean closer to the sensor.
 * Returns true on success, or false otherwise. If false is returned, the
 * value of proximity cannot be used.
 */
bool mgos_apds9960_read_proximity(struct mgos_apds9960 *sensor, uint8_t *proximity);

bool mgos_apds9960_is_gesture_available(struct mgos_apds9960 *sensor);
int mgos_apds9960_read_gesture(struct mgos_apds9960 *sensor);
