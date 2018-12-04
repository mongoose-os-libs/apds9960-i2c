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
#include "mgos_apds9960.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG                              1

/* APDS9960 I2C address */
#define APDS9960_I2C_ADDR                  0x39

/* Gesture parameters */
#define APDS9960_GESTURE_THRESHOLD_OUT     10
#define APDS9960_GESTURE_SENSITIVITY_1     50
#define APDS9960_GESTURE_SENSITIVITY_2     20

/* Error code for returned values */
#define APDS9960_ERROR                     0xFF

/* Acceptable device IDs */
#define APDS9960_ID_1                      0xAB
#define APDS9960_ID_2                      0x9C

/* Misc parameters */
#define APDS9960_FIFO_PAUSE_TIME           30    // Wait period (ms) between FIFO reads

/* APDS-9960 register addresses */
#define APDS9960_ENABLE                    0x80
#define APDS9960_ATIME                     0x81
#define APDS9960_WTIME                     0x83
#define APDS9960_AILTL                     0x84
#define APDS9960_AILTH                     0x85
#define APDS9960_AIHTL                     0x86
#define APDS9960_AIHTH                     0x87
#define APDS9960_PILT                      0x89
#define APDS9960_PIHT                      0x8B
#define APDS9960_PERS                      0x8C
#define APDS9960_CONFIG1                   0x8D
#define APDS9960_PPULSE                    0x8E
#define APDS9960_CONTROL                   0x8F
#define APDS9960_CONFIG2                   0x90
#define APDS9960_ID                        0x92
#define APDS9960_STATUS                    0x93
#define APDS9960_CDATAL                    0x94
#define APDS9960_CDATAH                    0x95
#define APDS9960_RDATAL                    0x96
#define APDS9960_RDATAH                    0x97
#define APDS9960_GDATAL                    0x98
#define APDS9960_GDATAH                    0x99
#define APDS9960_BDATAL                    0x9A
#define APDS9960_BDATAH                    0x9B
#define APDS9960_PDATA                     0x9C
#define APDS9960_POFFSET_UR                0x9D
#define APDS9960_POFFSET_DL                0x9E
#define APDS9960_CONFIG3                   0x9F
#define APDS9960_GPENTH                    0xA0
#define APDS9960_GEXTH                     0xA1
#define APDS9960_GCONF1                    0xA2
#define APDS9960_GCONF2                    0xA3
#define APDS9960_GOFFSET_U                 0xA4
#define APDS9960_GOFFSET_D                 0xA5
#define APDS9960_GOFFSET_L                 0xA7
#define APDS9960_GOFFSET_R                 0xA9
#define APDS9960_GPULSE                    0xA6
#define APDS9960_GCONF3                    0xAA
#define APDS9960_GCONF4                    0xAB
#define APDS9960_GFLVL                     0xAE
#define APDS9960_GSTATUS                   0xAF
#define APDS9960_IFORCE                    0xE4
#define APDS9960_PICLEAR                   0xE5
#define APDS9960_CICLEAR                   0xE6
#define APDS9960_AICLEAR                   0xE7
#define APDS9960_GFIFO_U                   0xFC
#define APDS9960_GFIFO_D                   0xFD
#define APDS9960_GFIFO_L                   0xFE
#define APDS9960_GFIFO_R                   0xFF

/* Bit fields */
#define APDS9960_PON                       0b00000001
#define APDS9960_AEN                       0b00000010
#define APDS9960_PEN                       0b00000100
#define APDS9960_WEN                       0b00001000
#define APSD9960_AIEN                      0b00010000
#define APDS9960_PIEN                      0b00100000
#define APDS9960_GEN                       0b01000000
#define APDS9960_GVALID                    0b00000001

/* On/Off definitions */
#define APDS9960_OFF                       0
#define APDS9960_ON                        1

/* Acceptable parameters for setMode */
#define APDS9960_POWER                     0
#define APDS9960_AMBIENT_LIGHT             1
#define APDS9960_PROXIMITY                 2
#define APDS9960_WAIT                      3
#define APDS9960_AMBIENT_LIGHT_INT         4
#define APDS9960_PROXIMITY_INT             5
#define APDS9960_GESTURE                   6
#define APDS9960_ALL                       7

/* LED Drive values */
#define APDS9960_LED_DRIVE_100MA           0
#define APDS9960_LED_DRIVE_50MA            1
#define APDS9960_LED_DRIVE_25MA            2
#define APDS9960_LED_DRIVE_12_5MA          3

/* Proximity Gain (PGAIN) values */
#define APDS9960_PGAIN_1X                  0
#define APDS9960_PGAIN_2X                  1
#define APDS9960_PGAIN_4X                  2
#define APDS9960_PGAIN_8X                  3

/* ALS Gain (AGAIN) values */
#define APDS9960_AGAIN_1X                  0
#define APDS9960_AGAIN_4X                  1
#define APDS9960_AGAIN_16X                 2
#define APDS9960_AGAIN_64X                 3

/* Gesture Gain (GGAIN) values */
#define APDS9960_GGAIN_1X                  0
#define APDS9960_GGAIN_2X                  1
#define APDS9960_GGAIN_4X                  2
#define APDS9960_GGAIN_8X                  3

/* LED Boost values */
#define APDS9960_LED_BOOST_100             0
#define APDS9960_LED_BOOST_150             1
#define APDS9960_LED_BOOST_200             2
#define APDS9960_LED_BOOST_300             3

/* Gesture wait time values */
#define APDS9960_GWTIME_0MS                0
#define APDS9960_GWTIME_2_8MS              1
#define APDS9960_GWTIME_5_6MS              2
#define APDS9960_GWTIME_8_4MS              3
#define APDS9960_GWTIME_14_0MS             4
#define APDS9960_GWTIME_22_4MS             5
#define APDS9960_GWTIME_30_8MS             6
#define APDS9960_GWTIME_39_2MS             7

/* Default values */
#define APDS9960_DEFAULT_ATIME             219   // 103ms
#define APDS9960_DEFAULT_WTIME             246   // 27ms
#define APDS9960_DEFAULT_PROX_PPULSE       0x87  // 16us, 8 pulses
#define APDS9960_DEFAULT_GESTURE_PPULSE    0x89  // 16us, 10 pulses
#define APDS9960_DEFAULT_POFFSET_UR        0     // 0 offset
#define APDS9960_DEFAULT_POFFSET_DL        0     // 0 offset
#define APDS9960_DEFAULT_CONFIG1           0x60  // No 12x wait (WTIME) factor
#define APDS9960_DEFAULT_LDRIVE            APDS9960_LED_DRIVE_100MA
#define APDS9960_DEFAULT_PGAIN             APDS9960_PGAIN_4X
#define APDS9960_DEFAULT_AGAIN             APDS9960_AGAIN_4X
#define APDS9960_DEFAULT_PILT              0      // Low proximity threshold
#define APDS9960_DEFAULT_PIHT              50     // High proximity threshold
#define APDS9960_DEFAULT_AILT              0xFFFF // Force interrupt for calibration
#define APDS9960_DEFAULT_AIHT              0
#define APDS9960_DEFAULT_PERS              0x11   // 2 consecutive prox or ALS for int.
#define APDS9960_DEFAULT_CONFIG2           0x01   // No saturation interrupts or LED boost
#define APDS9960_DEFAULT_CONFIG3           0      // Enable all photodiodes, no SAI
#define APDS9960_DEFAULT_GPENTH            40     // Threshold for entering gesture mode
#define APDS9960_DEFAULT_GEXTH             30     // Threshold for exiting gesture mode
#define APDS9960_DEFAULT_GCONF1            0x40   // 4 gesture events for int., 1 for exit
#define APDS9960_DEFAULT_GGAIN             APDS9960_GGAIN_4X
#define APDS9960_DEFAULT_GLDRIVE           APDS9960_LED_DRIVE_100MA
#define APDS9960_DEFAULT_GWTIME            APDS9960_GWTIME_2_8MS
#define APDS9960_DEFAULT_GOFFSET           0     // No offset scaling for gesture mode
#define APDS9960_DEFAULT_GPULSE            0xC9  // 32us, 10 pulses
#define APDS9960_DEFAULT_GCONF3            0     // All photodiodes active during gesture
#define APDS9960_DEFAULT_GIEN              0     // Disable gesture interrupts

/* Direction definitions */
enum {
  APDS9960_DIR_NONE,
  APDS9960_DIR_LEFT,
  APDS9960_DIR_RIGHT,
  APDS9960_DIR_UP,
  APDS9960_DIR_DOWN,
  APDS9960_DIR_NEAR,
  APDS9960_DIR_FAR,
  APDS9960_DIR_ALL
};

/* State definitions */
enum {
  APDS9960_NA_STATE,
  APDS9960_NEAR_STATE,
  APDS9960_FAR_STATE,
  APDS9960_ALL_STATE
};

/* Container for gesture data */
typedef struct apds9960_gesture_data_type {
  uint8_t u_data[32];
  uint8_t d_data[32];
  uint8_t l_data[32];
  uint8_t r_data[32];
  uint8_t index;
  uint8_t total_gestures;
  uint8_t in_threshold;
  uint8_t out_threshold;
} apds9960_gesture_data_type;

struct mgos_apds9960 {
  struct mgos_i2c *          i2c;
  uint8_t                    i2caddr;
  struct mgos_apds9960_stats stats;

  /* Private data for the driver */
  apds9960_gesture_data_type gesture_data_;
  int                        gesture_ud_delta_;
  int                        gesture_lr_delta_;
  int                        gesture_ud_count_;
  int                        gesture_lr_count_;
  int                        gesture_near_count_;
  int                        gesture_far_count_;
  int                        gesture_state_;
  int                        gesture_motion_;
};

/* Mongoose OS intiializer */
bool mgos_apds9960_i2c_init(void);

/* Private methods */
static void mgos_apds9960_resetGestureParameters(struct mgos_apds9960 *sensor);
static bool mgos_apds9960_processGestureData(struct mgos_apds9960 *sensor);
static bool mgos_apds9960_decodeGesture(struct mgos_apds9960 *sensor);

static uint8_t mgos_apds9960_getProxIntLowThresh(struct mgos_apds9960 *sensor);
static bool mgos_apds9960_setProxIntLowThresh(struct mgos_apds9960 *sensor, uint8_t threshold);
static uint8_t mgos_apds9960_getProxIntHighThresh(struct mgos_apds9960 *sensor);
static bool mgos_apds9960_setProxIntHighThresh(struct mgos_apds9960 *sensor, uint8_t threshold);

static uint8_t mgos_apds9960_getLEDBoost(struct mgos_apds9960 *sensor);
static bool mgos_apds9960_setLEDBoost(struct mgos_apds9960 *sensor, uint8_t boost);

static uint8_t mgos_apds9960_getProxGainCompEnable(struct mgos_apds9960 *sensor);
static bool mgos_apds9960_setProxGainCompEnable(struct mgos_apds9960 *sensor, uint8_t enable);
static uint8_t mgos_apds9960_getProxPhotoMask(struct mgos_apds9960 *sensor);
static bool mgos_apds9960_setProxPhotoMask(struct mgos_apds9960 *sensor, uint8_t mask);

static uint8_t mgos_apds9960_getGestureEnterThresh(struct mgos_apds9960 *sensor);
static bool mgos_apds9960_setGestureEnterThresh(struct mgos_apds9960 *sensor, uint8_t threshold);
static uint8_t mgos_apds9960_getGestureExitThresh(struct mgos_apds9960 *sensor);
static bool mgos_apds9960_setGestureExitThresh(struct mgos_apds9960 *sensor, uint8_t threshold);

static uint8_t mgos_apds9960_getGestureWaitTime(struct mgos_apds9960 *sensor);
static bool mgos_apds9960_setGestureWaitTime(struct mgos_apds9960 *sensor, uint8_t time);

static uint8_t mgos_apds9960_getGestureMode(struct mgos_apds9960 *sensor);
static bool mgos_apds9960_setGestureMode(struct mgos_apds9960 *sensor, uint8_t mode);

static bool mgos_apds9960_wireWriteByte(struct mgos_apds9960 *sensor, uint8_t val);
static bool mgos_apds9960_wireWriteDataByte(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t val);
static bool mgos_apds9960_wireWriteDataBlock(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t *val, unsigned int len);
static bool mgos_apds9960_wireReadDataByte(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t *val);
static int mgos_apds9960_wireReadDataBlock(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t *val, unsigned int len);

#ifdef __cplusplus
}
#endif
