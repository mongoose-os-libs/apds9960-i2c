# APDS-960 I2C Driver

A Mongoose library for Avago's APDS-9960 RGB, IR and Gesture Sensor.

## Implementation details


### Limitations

## Example application

An example program using a timer to read data from the sensor every 5 seconds:

```
#include "mgos.h"
#include "mgos_apds9960.h"

static struct mgos_apds9960 *s_apds9960;

static void timer_cb(void *user_data) {
  uint8_t r, g, b, ir;

  mgos_apds9960_get_rgb(s_apds9960, &r, g, &b, &ir);

  LOG(LL_INFO, ("APDS9960 r=%u g=%u b=%u ir=%u", r, g, b, ir);

  (void) user_data;
}

enum mgos_app_init_result mgos_app_init(void) {
  s_apds9960=mgos_apds9960_create(0x39); // Default I2C address
  if (s_apds9960) {
    mgos_set_timer(5000, true, timer_cb, NULL);
  } else {
    LOG(LL_ERROR, ("Could not initialize sensor"));
  }
  return MGOS_APP_INIT_SUCCESS;
}
```


# Disclaimer

This project is not an official Google project. It is not supported by Google
and Google specifically disclaims all warranties as to its quality,
merchantability, or fitness for a particular purpose.
