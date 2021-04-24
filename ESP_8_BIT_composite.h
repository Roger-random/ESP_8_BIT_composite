#ifndef ESP_8_BIT_COMPOSITE_H
#define ESP_8_BIT_COMPOSITE_H

#include "Arduino.h"

#include "esp_types.h"
#include "esp_err.h"
#include "esp_log.h"

#include "esp_attr.h"
#include "esp_heap_caps.h"
#include "esp_intr_alloc.h"
#include "driver/periph_ctrl.h"
#include "driver/dac.h"
#include "driver/gpio.h"
#include "driver/i2s.h"
#include "rom/gpio.h"
#include "rom/lldesc.h"
#include "soc/gpio_reg.h"
#include "soc/i2s_struct.h"
#include "soc/i2s_reg.h"
#include "soc/io_mux_reg.h"
#include "soc/rtc.h"
#include "soc/rtc_io_reg.h"
#include "soc/soc.h"

class ESP_8_BIT_composite
{
  public:
    ESP_8_BIT_composite(int ntsc);
    void setup();
    void vsync();
    uint8_t** get_lines();
  private:
    void instance_check();
};

#endif // ESP_8_BIT_COMPOSITE_H
