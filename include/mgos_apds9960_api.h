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

bool mgos_apds9960_init(struct mgos_apds9960 *sensor);
bool mgos_apds9960_enable(struct mgos_apds9960 *sensor);
bool mgos_apds9960_disable(struct mgos_apds9960 *sensor);
uint8_t mgos_apds9960_get_mode(struct mgos_apds9960 *sensor);
bool mgos_apds9960_set_mode(struct mgos_apds9960 *sensor, uint8_t mode, uint8_t enable);
uint8_t mgos_apds9960_get_led_drive(struct mgos_apds9960 *sensor);
bool mgos_apds9960_set_led_drive(struct mgos_apds9960 *sensor, uint8_t drive);

/* Light sensor API calls */
bool mgos_apds9960_enable_light_sensor(struct mgos_apds9960 *sensor, bool interrupts);
bool mgos_apds9960_disable_light_sensor(struct mgos_apds9960 *sensor);
uint8_t mgos_apds9960_get_ambient_light_gain(struct mgos_apds9960 *sensor);
bool mgos_apds9960_set_ambient_light_gain(struct mgos_apds9960 *sensor, uint8_t gain);
bool mgos_apds9960_get_light_int_low_threshold(struct mgos_apds9960 *sensor, uint16_t *threshold);
bool mgos_apds9960_set_light_int_low_threshold(struct mgos_apds9960 *sensor, uint16_t threshold);
bool mgos_apds9960_get_light_int_high_threshold(struct mgos_apds9960 *sensor, uint16_t *threshold);
bool mgos_apds9960_set_light_int_high_threshold(struct mgos_apds9960 *sensor, uint16_t threshold);
uint8_t mgos_apds9960_get_ambient_light_int_enable(struct mgos_apds9960 *sensor);
bool mgos_apds9960_set_ambient_light_int_enable(struct mgos_apds9960 *sensor, uint8_t enable);
bool mgos_apds9960_clear_ambientlight_int(struct mgos_apds9960 *sensor);
bool mgos_apds9960_read_ambient_light(struct mgos_apds9960 *sensor, uint16_t *val);
bool mgos_apds9960_read_red_light(struct mgos_apds9960 *sensor, uint16_t *val);
bool mgos_apds9960_read_green_light(struct mgos_apds9960 *sensor, uint16_t *val);
bool mgos_apds9960_read_blue_light(struct mgos_apds9960 *sensor, uint16_t *val);


/* Proximity sensor API calls */
bool mgos_apds9960_enable_proximity_sensor(struct mgos_apds9960 *sensor, bool interrupts);
bool mgos_apds9960_disable_proximity_sensor(struct mgos_apds9960 *sensor);
uint8_t mgos_apds9960_get_proximity_gain(struct mgos_apds9960 *sensor);
bool mgos_apds9960_set_proximity_gain(struct mgos_apds9960 *sensor, uint8_t gain);
bool mgos_apds9960_get_proximity_int_low_threshold(struct mgos_apds9960 *sensor, uint8_t *threshold);
bool mgos_apds9960_set_proximity_int_low_threshold(struct mgos_apds9960 *sensor, uint8_t threshold);
bool mgos_apds9960_get_proximity_int_high_threshold(struct mgos_apds9960 *sensor, uint8_t *threshold);
bool mgos_apds9960_set_proximity_int_high_threshold(struct mgos_apds9960 *sensor, uint8_t threshold);
uint8_t mgos_apds9960_get_proximity_int_enable(struct mgos_apds9960 *sensor);
bool mgos_apds9960_set_proximity_int_enable(struct mgos_apds9960 *sensor, uint8_t enable);
bool mgos_apds9960_clear_proximity_int(struct mgos_apds9960 *sensor);


/* Gesture sensor API calls */
bool mgos_apds9960_enable_gesture_sensor(struct mgos_apds9960 *sensor, bool interrupts);
bool mgos_apds9960_disable_gesture_sensor(struct mgos_apds9960 *sensor);
uint8_t mgos_apds9960_get_gesture_led_drive(struct mgos_apds9960 *sensor);
bool mgos_apds9960_set_gesture_led_drive(struct mgos_apds9960 *sensor, uint8_t drive);
uint8_t mgos_apds9960_get_gesture_gain(struct mgos_apds9960 *sensor);
bool mgos_apds9960_set_gesture_gain(struct mgos_apds9960 *sensor, uint8_t gain);
uint8_t mgos_apds9960_get_gesture_int_enable(struct mgos_apds9960 *sensor);
bool mgos_apds9960_set_gesture_int_enable(struct mgos_apds9960 *sensor, uint8_t enable);

