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
  sensor->i2caddr             = i2caddr;
  sensor->i2c                 = i2c;
  sensor->gesture_ud_delta_   = 0;
  sensor->gesture_lr_delta_   = 0;
  sensor->gesture_ud_count_   = 0;
  sensor->gesture_lr_count_   = 0;
  sensor->gesture_near_count_ = 0;
  sensor->gesture_far_count_  = 0;
  sensor->gesture_state_      = 0;
  sensor->gesture_motion_     = APDS9960_DIR_NONE;
  mgos_apds9960_resetGestureParameters(sensor);

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
static void mgos_apds9960_resetGestureParameters(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return;
  }
  sensor->gesture_data_.index          = 0;
  sensor->gesture_data_.total_gestures = 0;
  sensor->gesture_ud_delta_            = 0;
  sensor->gesture_lr_delta_            = 0;
  sensor->gesture_ud_count_            = 0;
  sensor->gesture_lr_count_            = 0;
  sensor->gesture_near_count_          = 0;
  sensor->gesture_far_count_           = 0;
  sensor->gesture_state_  = 0;
  sensor->gesture_motion_ = APDS9960_DIR_NONE;
  return;
}

static bool mgos_apds9960_processGestureData(struct mgos_apds9960 *sensor) {
  uint8_t u_first = 0;
  uint8_t d_first = 0;
  uint8_t l_first = 0;
  uint8_t r_first = 0;
  uint8_t u_last  = 0;
  uint8_t d_last  = 0;
  uint8_t l_last  = 0;
  uint8_t r_last  = 0;
  int     ud_ratio_first;
  int     lr_ratio_first;
  int     ud_ratio_last;
  int     lr_ratio_last;
  int     ud_delta;
  int     lr_delta;
  int     i;

  if (!sensor) {
    return false;
  }

  /* If we have less than 4 total gestures, that's not enough */
  if (sensor->gesture_data_.total_gestures <= 4) {
    return false;
  }

  /* Check to make sure our data isn't out of bounds */
  if ((sensor->gesture_data_.total_gestures <= 32) && \
      (sensor->gesture_data_.total_gestures > 0)) {
    /* Find the first value in U/D/L/R above the threshold */
    for (i = 0; i < sensor->gesture_data_.total_gestures; i++) {
      if ((sensor->gesture_data_.u_data[i] > APDS9960_GESTURE_THRESHOLD_OUT) &&
          (sensor->gesture_data_.d_data[i] > APDS9960_GESTURE_THRESHOLD_OUT) &&
          (sensor->gesture_data_.l_data[i] > APDS9960_GESTURE_THRESHOLD_OUT) &&
          (sensor->gesture_data_.r_data[i] > APDS9960_GESTURE_THRESHOLD_OUT)) {
        u_first = sensor->gesture_data_.u_data[i];
        d_first = sensor->gesture_data_.d_data[i];
        l_first = sensor->gesture_data_.l_data[i];
        r_first = sensor->gesture_data_.r_data[i];
        break;
      }
    }

    /* If one of the _first values is 0, then there is no good data */
    if ((u_first == 0) || (d_first == 0) || \
        (l_first == 0) || (r_first == 0)) {
      return false;
    }
    /* Find the last value in U/D/L/R above the threshold */
    for (i = sensor->gesture_data_.total_gestures - 1; i >= 0; i--) {
#if DEBUG
      LOG(LL_DEBUG, ("Finding last: U=%d D=%d L=%d R=%d", sensor->gesture_data_.u_data[i], sensor->gesture_data_.d_data[i], sensor->gesture_data_.l_data[i], sensor->gesture_data_.r_data[i]));

      /*
       * Serial.print(F("Finding last: "));
       * Serial.print(F("U:"));
       * Serial.print(sensor->gesture_data_.u_data[i]);
       * Serial.print(F(" D:"));
       * Serial.print(sensor->gesture_data_.d_data[i]);
       * Serial.print(F(" L:"));
       * Serial.print(sensor->gesture_data_.l_data[i]);
       * Serial.print(F(" R:"));
       * Serial.println(sensor->gesture_data_.r_data[i]);
       */
#endif
      if ((sensor->gesture_data_.u_data[i] > APDS9960_GESTURE_THRESHOLD_OUT) &&
          (sensor->gesture_data_.d_data[i] > APDS9960_GESTURE_THRESHOLD_OUT) &&
          (sensor->gesture_data_.l_data[i] > APDS9960_GESTURE_THRESHOLD_OUT) &&
          (sensor->gesture_data_.r_data[i] > APDS9960_GESTURE_THRESHOLD_OUT)) {
        u_last = sensor->gesture_data_.u_data[i];
        d_last = sensor->gesture_data_.d_data[i];
        l_last = sensor->gesture_data_.l_data[i];
        r_last = sensor->gesture_data_.r_data[i];
        break;
      }
    }
  }

  /* Calculate the first vs. last ratio of up/down and left/right */
  ud_ratio_first = ((u_first - d_first) * 100) / (u_first + d_first);
  lr_ratio_first = ((l_first - r_first) * 100) / (l_first + r_first);
  ud_ratio_last  = ((u_last - d_last) * 100) / (u_last + d_last);
  lr_ratio_last  = ((l_last - r_last) * 100) / (l_last + r_last);

#if DEBUG
  LOG(LL_DEBUG, ("Last Values: U=%d D=%d L=%d R=%d", u_last, d_last, l_last, r_last));
  LOG(LL_DEBUG, ("Ratios: UDFi=%d UDLa=%d LRFi=%d LRLa=%d", ud_ratio_first, ud_ratio_last, lr_ratio_first, lr_ratio_last));

  /*
   * Serial.print(F("Last Values: "));
   * Serial.print(F("U:"));
   * Serial.print(u_last);
   * Serial.print(F(" D:"));
   * Serial.print(d_last);
   * Serial.print(F(" L:"));
   * Serial.print(l_last);
   * Serial.print(F(" R:"));
   * Serial.println(r_last);
   *
   * Serial.print(F("Ratios: "));
   * Serial.print(F("UD Fi: "));
   * Serial.print(ud_ratio_first);
   * Serial.print(F(" UD La: "));
   * Serial.print(ud_ratio_last);
   * Serial.print(F(" LR Fi: "));
   * Serial.print(lr_ratio_first);
   * Serial.print(F(" LR La: "));
   * Serial.println(lr_ratio_last);
   */
#endif

  /* Determine the difference between the first and last ratios */
  ud_delta = ud_ratio_last - ud_ratio_first;
  lr_delta = lr_ratio_last - lr_ratio_first;

#if DEBUG
  LOG(LL_DEBUG, ("Deltas: UD=%d LR=%d", ud_delta, lr_delta));

  /*
   * Serial.print("Deltas: ");
   * Serial.print("UD: ");
   * Serial.print(ud_delta);
   * Serial.print(" LR: ");
   * Serial.println(lr_delta);
   */
#endif

  /* Accumulate the UD and LR delta values */
  sensor->gesture_ud_delta_ += ud_delta;
  sensor->gesture_lr_delta_ += lr_delta;

#if DEBUG
  LOG(LL_DEBUG, ("Accumulations: UD=%d LR=%d", sensor->gesture_ud_delta_, sensor->gesture_lr_delta_));

  /*
   * Serial.print("Accumulations: ");
   * Serial.print("UD: ");
   * Serial.print(sensor->gesture_ud_delta_);
   * Serial.print(" LR: ");
   * Serial.println(sensor->gesture_lr_delta_);
   */
#endif

  /* Determine U/D gesture */
  if (sensor->gesture_ud_delta_ >= APDS9960_GESTURE_SENSITIVITY_1) {
    sensor->gesture_ud_count_ = 1;
  } else if (sensor->gesture_ud_delta_ <= -APDS9960_GESTURE_SENSITIVITY_1) {
    sensor->gesture_ud_count_ = -1;
  } else {
    sensor->gesture_ud_count_ = 0;
  }

  /* Determine L/R gesture */
  if (sensor->gesture_lr_delta_ >= APDS9960_GESTURE_SENSITIVITY_1) {
    sensor->gesture_lr_count_ = 1;
  } else if (sensor->gesture_lr_delta_ <= -APDS9960_GESTURE_SENSITIVITY_1) {
    sensor->gesture_lr_count_ = -1;
  } else {
    sensor->gesture_lr_count_ = 0;
  }

  /* Determine Near/Far gesture */
  if ((sensor->gesture_ud_count_ == 0) && (sensor->gesture_lr_count_ == 0)) {
    if ((abs(ud_delta) < APDS9960_GESTURE_SENSITIVITY_2) && \
        (abs(lr_delta) < APDS9960_GESTURE_SENSITIVITY_2)) {
      if ((ud_delta == 0) && (lr_delta == 0)) {
        sensor->gesture_near_count_++;
      } else if ((ud_delta != 0) || (lr_delta != 0)) {
        sensor->gesture_far_count_++;
      }

      if ((sensor->gesture_near_count_ >= 10) && (sensor->gesture_far_count_ >= 2)) {
        if ((ud_delta == 0) && (lr_delta == 0)) {
          sensor->gesture_state_ = APDS9960_NEAR_STATE;
        } else if ((ud_delta != 0) && (lr_delta != 0)) {
          sensor->gesture_state_ = APDS9960_FAR_STATE;
        }
        return true;
      }
    }
  } else {
    if ((abs(ud_delta) < APDS9960_GESTURE_SENSITIVITY_2) && \
        (abs(lr_delta) < APDS9960_GESTURE_SENSITIVITY_2)) {
      if ((ud_delta == 0) && (lr_delta == 0)) {
        sensor->gesture_near_count_++;
      }

      if (sensor->gesture_near_count_ >= 10) {
        sensor->gesture_ud_count_ = 0;
        sensor->gesture_lr_count_ = 0;
        sensor->gesture_ud_delta_ = 0;
        sensor->gesture_lr_delta_ = 0;
      }
    }
  }

#if DEBUG
  LOG(LL_DEBUG, ("UD_CT=%d LR_CT=%d NEAR_CT=%d FAR_CT=%d", sensor->gesture_ud_count_, sensor->gesture_lr_count_, sensor->gesture_near_count_, sensor->gesture_far_count_));

  /*
   * Serial.print("UD_CT: ");
   * Serial.print(sensor->gesture_ud_count_);
   * Serial.print(" LR_CT: ");
   * Serial.print(sensor->gesture_lr_count_);
   * Serial.print(" NEAR_CT: ");
   * Serial.print(sensor->gesture_near_count_);
   * Serial.print(" FAR_CT: ");
   * Serial.println(sensor->gesture_far_count_);
   * Serial.println("----------");
   */
#endif
  return false;
}

static bool mgos_apds9960_decodeGesture(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return false;
  }
  /* Return if near or far event is detected */
  if (sensor->gesture_state_ == APDS9960_NEAR_STATE) {
    sensor->gesture_motion_ = APDS9960_DIR_NEAR;
    return true;
  } else if (sensor->gesture_state_ == APDS9960_FAR_STATE) {
    sensor->gesture_motion_ = APDS9960_DIR_FAR;
    return true;
  }

  /* Determine swipe direction */
  if ((sensor->gesture_ud_count_ == -1) && (sensor->gesture_lr_count_ == 0)) {
    sensor->gesture_motion_ = APDS9960_DIR_UP;
  } else if ((sensor->gesture_ud_count_ == 1) && (sensor->gesture_lr_count_ == 0)) {
    sensor->gesture_motion_ = APDS9960_DIR_DOWN;
  } else if ((sensor->gesture_ud_count_ == 0) && (sensor->gesture_lr_count_ == 1)) {
    sensor->gesture_motion_ = APDS9960_DIR_RIGHT;
  } else if ((sensor->gesture_ud_count_ == 0) && (sensor->gesture_lr_count_ == -1)) {
    sensor->gesture_motion_ = APDS9960_DIR_LEFT;
  } else if ((sensor->gesture_ud_count_ == -1) && (sensor->gesture_lr_count_ == 1)) {
    if (abs(sensor->gesture_ud_delta_) > abs(sensor->gesture_lr_delta_)) {
      sensor->gesture_motion_ = APDS9960_DIR_UP;
    } else {
      sensor->gesture_motion_ = APDS9960_DIR_RIGHT;
    }
  } else if ((sensor->gesture_ud_count_ == 1) && (sensor->gesture_lr_count_ == -1)) {
    if (abs(sensor->gesture_ud_delta_) > abs(sensor->gesture_lr_delta_)) {
      sensor->gesture_motion_ = APDS9960_DIR_DOWN;
    } else {
      sensor->gesture_motion_ = APDS9960_DIR_LEFT;
    }
  } else if ((sensor->gesture_ud_count_ == -1) && (sensor->gesture_lr_count_ == -1)) {
    if (abs(sensor->gesture_ud_delta_) > abs(sensor->gesture_lr_delta_)) {
      sensor->gesture_motion_ = APDS9960_DIR_UP;
    } else {
      sensor->gesture_motion_ = APDS9960_DIR_LEFT;
    }
  } else if ((sensor->gesture_ud_count_ == 1) && (sensor->gesture_lr_count_ == 1)) {
    if (abs(sensor->gesture_ud_delta_) > abs(sensor->gesture_lr_delta_)) {
      sensor->gesture_motion_ = APDS9960_DIR_DOWN;
    } else {
      sensor->gesture_motion_ = APDS9960_DIR_RIGHT;
    }
  } else {
    return false;
  }

  return true;
}

static uint8_t mgos_apds9960_getProxIntLowThresh(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_PILT, &val)) {
    return 0;
  }
  return val;
}

static bool mgos_apds9960_setProxIntLowThresh(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_PILT, threshold)) {
    return false;
  }

  return true;
}

static uint8_t mgos_apds9960_getProxIntHighThresh(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_PIHT, &val)) {
    return 0;
  }
  return val;
}

static bool mgos_apds9960_setProxIntHighThresh(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_PILT, threshold)) {
    return false;
  }

  return true;
}

static uint8_t mgos_apds9960_getLEDBoost(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from CONFIG2 register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONFIG2, &val)) {
    return APDS9960_ERROR;
  }

  /* Shift and mask out LED_BOOST bits */
  val = (val >> 4) & 0b00000011;

  return val;
}

static bool mgos_apds9960_setLEDBoost(struct mgos_apds9960 *sensor, uint8_t boost) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  /* Read value from CONFIG2 register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONFIG2, &val)) {
    return false;
  }

  /* Set bits in register to given value */
  boost &= 0b00000011;
  boost  = boost << 4;
  val   &= 0b11001111;
  val   |= boost;

  /* Write register value back into CONFIG2 register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONFIG2, val)) {
    return false;
  }

  return true;
}

static uint8_t mgos_apds9960_getProxGainCompEnable(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from CONFIG3 register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONFIG3, &val)) {
    return APDS9960_ERROR;
  }

  /* Shift and mask out PCMP bits */
  val = (val >> 5) & 0b00000001;

  return val;
}

static bool mgos_apds9960_setProxGainCompEnable(struct mgos_apds9960 *sensor, uint8_t enable) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  /* Read value from CONFIG3 register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONFIG3, &val)) {
    return false;
  }

  /* Set bits in register to given value */
  enable &= 0b00000001;
  enable  = enable << 5;
  val    &= 0b11011111;
  val    |= enable;

  /* Write register value back into CONFIG3 register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONFIG3, val)) {
    return false;
  }

  return true;
}

static uint8_t mgos_apds9960_getProxPhotoMask(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from CONFIG3 register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONFIG3, &val)) {
    return APDS9960_ERROR;
  }

  /* Mask out photodiode enable mask bits */
  val &= 0b00001111;

  return val;
}

static bool mgos_apds9960_setProxPhotoMask(struct mgos_apds9960 *sensor, uint8_t mask) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  /* Read value from CONFIG3 register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONFIG3, &val)) {
    return false;
  }

  /* Set bits in register to given value */
  mask &= 0b00001111;
  val  &= 0b11110000;
  val  |= mask;

  /* Write register value back into CONFIG3 register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONFIG3, val)) {
    return false;
  }

  return true;
}

static uint8_t mgos_apds9960_getGestureEnterThresh(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from GPENTH register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GPENTH, &val)) {
    val = 0;
  }

  return val;
}

static bool mgos_apds9960_setGestureEnterThresh(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GPENTH, threshold)) {
    return false;
  }

  return true;
}

static uint8_t mgos_apds9960_getGestureExitThresh(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from GEXTH register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GEXTH, &val)) {
    val = 0;
  }

  return val;
}

static bool mgos_apds9960_setGestureExitThresh(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GEXTH, threshold)) {
    return false;
  }

  return true;
}

static uint8_t mgos_apds9960_getGestureWaitTime(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from GCONF2 register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF2, &val)) {
    return APDS9960_ERROR;
  }

  /* Mask out GWTIME bits */
  val &= 0b00000111;

  return val;
}

static bool mgos_apds9960_setGestureWaitTime(struct mgos_apds9960 *sensor, uint8_t time) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  /* Read value from GCONF2 register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF2, &val)) {
    return false;
  }

  /* Set bits in register to given value */
  time &= 0b00000111;
  val  &= 0b11111000;
  val  |= time;

  /* Write register value back into GCONF2 register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GCONF2, val)) {
    return false;
  }

  return true;
}

static uint8_t mgos_apds9960_getGestureMode(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from GCONF4 register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF4, &val)) {
    return APDS9960_ERROR;
  }

  /* Mask out GMODE bit */
  val &= 0b00000001;

  return val;
}

static bool mgos_apds9960_setGestureMode(struct mgos_apds9960 *sensor, uint8_t mode) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  /* Read value from GCONF4 register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF4, &val)) {
    return false;
  }

  /* Set bits in register to given value */
  mode &= 0b00000001;
  val  &= 0b11111110;
  val  |= mode;

  /* Write register value back into GCONF4 register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GCONF4, val)) {
    return false;
  }

  return true;
}

static bool mgos_apds9960_wireWriteByte(struct mgos_apds9960 *sensor, uint8_t val) {
  if (!sensor) {
    return false;
  }
  return false;

  (void)val;
}

static bool mgos_apds9960_wireWriteDataByte(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t val) {
  if (!sensor) {
    return false;
  }
  return false;

  (void)reg;
  (void)val;
}

static bool mgos_apds9960_wireWriteDataBlock(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t *val, unsigned int len) {
  if (!sensor) {
    return false;
  }
  return false;

  (void)reg;
  (void)val;
  (void)len;
}

static bool mgos_apds9960_wireReadDataByte(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t *val) {
  if (!sensor) {
    return false;
  }
  return false;

  (void)reg;
  (void)val;
}

static int mgos_apds9960_wireReadDataBlock(struct mgos_apds9960 *sensor, uint8_t reg, uint8_t *val, unsigned int len) {
  if (!sensor) {
    return -1;
  }
  return -1;

  (void)reg;
  (void)val;
  (void)len;
}

bool mgos_apds9960_init(struct mgos_apds9960 *sensor) {
  uint8_t id;

  if (!sensor) {
    return false;
  }

  /* Read ID register and check against known values for APDS-9960 */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ID, &id)) {
    return false;
  }
  if (!(id == APDS9960_ID_1 || id == APDS9960_ID_2)) {
    return false;
  }

  /* Set ENABLE register to 0 (disable all features) */
  if (!mgos_apds9960_set_mode(sensor, APDS9960_ALL, APDS9960_OFF)) {
    return false;
  }

  /* Set default values for ambient light and proximity registers */
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
  if (!mgos_apds9960_set_ambient_light_gain(sensor, APDS9960_DEFAULT_AGAIN)) {
    return false;
  }
  if (!mgos_apds9960_setProxIntLowThresh(sensor, APDS9960_DEFAULT_PILT)) {
    return false;
  }
  if (!mgos_apds9960_setProxIntHighThresh(sensor, APDS9960_DEFAULT_PIHT)) {
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

  /* Set default values for gesture sense registers */
  if (!mgos_apds9960_setGestureEnterThresh(sensor, APDS9960_DEFAULT_GPENTH)) {
    return false;
  }
  if (!mgos_apds9960_setGestureExitThresh(sensor, APDS9960_DEFAULT_GEXTH)) {
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
  if (!mgos_apds9960_setGestureWaitTime(sensor, APDS9960_DEFAULT_GWTIME)) {
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

  return true;
}

bool mgos_apds9960_enable(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return false;
  }
  return mgos_apds9960_set_mode(sensor, APDS9960_POWER, 1);
}

bool mgos_apds9960_disable(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return false;
  }
  return mgos_apds9960_set_mode(sensor, APDS9960_POWER, 0);
}

uint8_t mgos_apds9960_get_mode(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read current ENABLE register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ENABLE, &val)) {
    return APDS9960_ERROR;
  }

  return val;
}

bool mgos_apds9960_set_mode(struct mgos_apds9960 *sensor, uint8_t mode, uint8_t enable) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  /* Read current ENABLE register */
  val = mgos_apds9960_get_mode(sensor);
  if (val == APDS9960_ERROR) {
    return false;
  }

  /* Change bit(s) in ENABLE register */
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

  /* Write value back to ENABLE register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_ENABLE, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_enable_light_sensor(struct mgos_apds9960 *sensor, bool interrupts) {
  if (!sensor) {
    return false;
  }
  /* Set default gain, interrupts, enable power, and enable sensor */
  if (!mgos_apds9960_set_ambient_light_gain(sensor, APDS9960_DEFAULT_AGAIN)) {
    return false;
  }
  if (!mgos_apds9960_set_ambient_light_int_enable(sensor, interrupts)) {
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
  if (!mgos_apds9960_set_ambient_light_int_enable(sensor, 0)) {
    return false;
  }
  if (!mgos_apds9960_set_mode(sensor, APDS9960_AMBIENT_LIGHT, 0)) {
    return false;
  }
  return true;
}

bool mgos_apds9960_enable_proximity_sensor(struct mgos_apds9960 *sensor, bool interrupts) {
  if (!sensor) {
    return false;
  }
  /* Set default gain, LED, interrupts, enable power, and enable sensor */
  if (!mgos_apds9960_set_proximity_gain(sensor, APDS9960_DEFAULT_PGAIN)) {
    return false;
  }
  if (!mgos_apds9960_set_led_drive(sensor, APDS9960_DEFAULT_LDRIVE)) {
    return false;
  }
  if (!mgos_apds9960_set_proximity_int_enable(sensor, interrupts)) {
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

bool mgos_apds9960_enable_gesture_sensor(struct mgos_apds9960 *sensor, bool interrupts) {
  if (!sensor) {
    return false;
  }

  /* Enable gesture mode
   * Set ENABLE to 0 (power off)
   * Set WTIME to 0xFF
   * Set AUX to LED_BOOST_300
   * Enable PON, WEN, PEN, GEN in ENABLE
   */
  mgos_apds9960_resetGestureParameters(sensor);
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_WTIME, 0xFF)) {
    return false;
  }
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_PPULSE, APDS9960_DEFAULT_GESTURE_PPULSE)) {
    return false;
  }
  if (!mgos_apds9960_setLEDBoost(sensor, APDS9960_LED_BOOST_300)) {
    return false;
  }
  if (!mgos_apds9960_set_gesture_int_enable(sensor, interrupts)) {
    return false;
  }
  if (!mgos_apds9960_setGestureMode(sensor, 1)) {
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
  mgos_apds9960_resetGestureParameters(sensor);
  if (!mgos_apds9960_set_gesture_int_enable(sensor, 0)) {
    return false;
  }
  if (!mgos_apds9960_setGestureMode(sensor, 0)) {
    return false;
  }
  if (!mgos_apds9960_set_mode(sensor, APDS9960_GESTURE, 0)) {
    return false;
  }

  return true;
}

uint8_t mgos_apds9960_get_led_drive(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }


  /* Read value from CONTROL register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONTROL, &val)) {
    return APDS9960_ERROR;
  }

  /* Shift and mask out LED drive bits */
  val = (val >> 6) & 0b00000011;

  return val;
}

bool mgos_apds9960_set_led_drive(struct mgos_apds9960 *sensor, uint8_t drive) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  /* Read value from CONTROL register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONTROL, &val)) {
    return false;
  }

  /* Set bits in register to given value */
  drive &= 0b00000011;
  drive  = drive << 6;
  val   &= 0b00111111;
  val   |= drive;

  /* Write register value back into CONTROL register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONTROL, val)) {
    return false;
  }

  return true;
}

uint8_t mgos_apds9960_get_gesture_led_drive(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from GCONF2 register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF2, &val)) {
    return APDS9960_ERROR;
  }

  /* Shift and mask out GLDRIVE bits */
  val = (val >> 3) & 0b00000011;

  return val;
}

bool mgos_apds9960_set_gesture_led_drive(struct mgos_apds9960 *sensor, uint8_t drive) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  /* Read value from GCONF2 register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF2, &val)) {
    return false;
  }

  /* Set bits in register to given value */
  drive &= 0b00000011;
  drive  = drive << 3;
  val   &= 0b11100111;
  val   |= drive;

  /* Write register value back into GCONF2 register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GCONF2, val)) {
    return false;
  }

  return true;
}

uint8_t mgos_apds9960_get_ambient_light_gain(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from CONTROL register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONTROL, &val)) {
    return APDS9960_ERROR;
  }

  /* Shift and mask out AGAIN bits */
  val &= 0b00000011;

  return val;
}

bool mgos_apds9960_set_ambient_light_gain(struct mgos_apds9960 *sensor, uint8_t gain) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  /* Read value from CONTROL register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONTROL, &val)) {
    return false;
  }

  /* Set bits in register to given value */
  gain &= 0b00000011;
  val  &= 0b11111100;
  val  |= gain;

  /* Write register value back into CONTROL register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONTROL, val)) {
    return false;
  }

  return true;
}

uint8_t mgos_apds9960_get_proximity_gain(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from CONTROL register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONTROL, &val)) {
    return APDS9960_ERROR;
  }

  /* Shift and mask out PGAIN bits */
  val = (val >> 2) & 0b00000011;


  return val;
}

bool mgos_apds9960_set_proximity_gain(struct mgos_apds9960 *sensor, uint8_t gain) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  /* Read value from CONTROL register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CONTROL, &val)) {
    return false;
  }

  /* Set bits in register to given value */
  gain &= 0b00000011;
  gain  = gain << 2;
  val  &= 0b11110011;
  val  |= gain;

  /* Write register value back into CONTROL register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_CONTROL, val)) {
    return false;
  }

  return true;
}

uint8_t mgos_apds9960_get_gesture_gain(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from CONTROL register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF2, &val)) {
    return APDS9960_ERROR;
  }

  /* Shift and mask out GGAIN bits */
  val = (val >> 5) & 0b00000011;

  return val;
}

bool mgos_apds9960_set_gesture_gain(struct mgos_apds9960 *sensor, uint8_t gain) {
  uint8_t val;

  if (!sensor) {
    return false;
  }

  /* Read value from CONTROL register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF2, &val)) {
    return false;
  }

  /* Set bits in register to given value */
  gain &= 0b00000011;
  gain  = gain << 5;
  val  &= 0b10011111;
  val  |= gain;

  /* Write register value back into CONTROL register */
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

  /* Read value from ambient light low threshold, low byte register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_AILTL, &val)) {
    return false;
  }
  *threshold = val;

  /* Read value from ambient light low threshold, high byte register */
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

  /* Break 16-bit threshold into 2 8-bit values */
  val_low  = threshold & 0x00FF;
  val_high = (threshold & 0xFF00) >> 8;

  /* Write low byte */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_AILTL, val_low)) {
    return false;
  }

  /* Write high byte */
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

  /* Read value from ambient light low threshold, low byte register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_AIHTL, &val)) {
    return false;
  }
  *threshold = val;

  /* Read value from ambient light low threshold, high byte register */
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
  /* Break 16-bit threshold into 2 8-bit values */
  val_low  = threshold & 0x00FF;
  val_high = (threshold & 0xFF00) >> 8;

  /* Write low byte */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_AIHTL, val_low)) {
    return false;
  }

  /* Write high byte */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_AIHTH, val_high)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_proximity_int_low_threshold(struct mgos_apds9960 *sensor, uint8_t *threshold) {
  if (!sensor || !threshold) {
    return false;
  }
  /* Read value from proximity low threshold register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_PILT, threshold)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_set_proximity_int_low_threshold(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) {
    return false;
  }
  /* Write threshold value to register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_PILT, threshold)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_get_proximity_int_high_threshold(struct mgos_apds9960 *sensor, uint8_t *threshold) {
  if (!sensor || !threshold) {
    return false;
  }
  /* Read value from proximity high threshold register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_PIHT, threshold)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_set_proximity_int_high_threshold(struct mgos_apds9960 *sensor, uint8_t threshold) {
  if (!sensor) {
    return false;
  }
  /* Write threshold value to register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_PIHT, threshold)) {
    return false;
  }

  return true;
}

uint8_t mgos_apds9960_get_ambient_light_int_enable(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from ENABLE register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ENABLE, &val)) {
    return APDS9960_ERROR;
  }

  /* Shift and mask out AIEN bit */
  val = (val >> 4) & 0b00000001;

  return val;
}

bool mgos_apds9960_set_ambient_light_int_enable(struct mgos_apds9960 *sensor, uint8_t enable) {
  uint8_t val;

  if (!sensor) {
    return false;
  }
  /* Read value from ENABLE register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ENABLE, &val)) {
    return false;
  }

  /* Set bits in register to given value */
  enable &= 0b00000001;
  enable  = enable << 4;
  val    &= 0b11101111;
  val    |= enable;

  /* Write register value back into ENABLE register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_ENABLE, val)) {
    return false;
  }

  return true;
}

uint8_t mgos_apds9960_get_proximity_int_enable(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from ENABLE register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ENABLE, &val)) {
    return APDS9960_ERROR;
  }

  /* Shift and mask out PIEN bit */
  val = (val >> 5) & 0b00000001;

  return val;
}

bool mgos_apds9960_set_proximity_int_enable(struct mgos_apds9960 *sensor, uint8_t enable) {
  uint8_t val;

  if (!sensor) {
    return false;
  }
  /* Read value from ENABLE register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ENABLE, &val)) {
    return false;
  }

  /* Set bits in register to given value */
  enable &= 0b00000001;
  enable  = enable << 5;
  val    &= 0b11011111;
  val    |= enable;

  /* Write register value back into ENABLE register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_ENABLE, val)) {
    return false;
  }

  return true;
}

uint8_t mgos_apds9960_get_gesture_int_enable(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return 0;
  }

  /* Read value from ENABLE register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_ENABLE, &val)) {
    return APDS9960_ERROR;
  }

  /* Shift and mask out GIEN bit */
  val = (val >> 1) & 0b00000001;

  return val;
}

bool mgos_apds9960_set_gesture_int_enable(struct mgos_apds9960 *sensor, uint8_t enable) {
  uint8_t val;

  if (!sensor) {
    return false;
  }
  /* Read value from GCONF4 register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GCONF4, &val)) {
    return false;
  }

  /* Set bits in register to given value */
  enable &= 0b00000001;
  enable  = enable << 1;
  val    &= 0b11111101;
  val    |= enable;

  /* Write register value back into GCONF4 register */
  if (!mgos_apds9960_wireWriteDataByte(sensor, APDS9960_GCONF4, val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_clear_ambientlight_int(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return false;
  }
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_AICLEAR, &val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_clear_proximity_int(struct mgos_apds9960 *sensor) {
  uint8_t val;

  if (!sensor) {
    return false;
  }
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_PICLEAR, &val)) {
    return false;
  }

  return true;
}

bool mgos_apds9960_read_ambient_light(struct mgos_apds9960 *sensor, uint16_t *val) {
  uint8_t val_byte;

  if (!sensor || !val) {
    return false;
  }
  *val = 0;

  /* Read value from clear channel, low byte register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_CDATAL, &val_byte)) {
    return false;
  }
  *val = val_byte;

  /* Read value from clear channel, high byte register */
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

  /* Read value from clear channel, low byte register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_RDATAL, &val_byte)) {
    return false;
  }
  *val = val_byte;

  /* Read value from clear channel, high byte register */
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

  /* Read value from clear channel, low byte register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_GDATAL, &val_byte)) {
    return false;
  }
  *val = val_byte;

  /* Read value from clear channel, high byte register */
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

  /* Read value from clear channel, low byte register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_BDATAL, &val_byte)) {
    return false;
  }
  *val = val_byte;

  /* Read value from clear channel, high byte register */
  if (!mgos_apds9960_wireReadDataByte(sensor, APDS9960_BDATAH, &val_byte)) {
    return false;
  }
  *val += ((uint16_t)val_byte << 8);

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
  if (!sensor) {
    return false;
  }
  return false;
}

int mgos_apds9960_read_gesture(struct mgos_apds9960 *sensor) {
  if (!sensor) {
    return -1;
  }
  return -1;
}

/* End sparkfun import */

bool mgos_apds9960_i2c_init(void) {
  return true;
}
