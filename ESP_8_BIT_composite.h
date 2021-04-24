#ifndef ESP_8_BIT_COMPOSITE_H
#define ESP_8_BIT_COMPOSITE_H

#include "Arduino.h"

#include "esp_err.h"
#include "esp_log.h"

class ESP_8_BIT_composite
{
  public:
    ESP_8_BIT_composite(int ntsc);
    void print_hello();
  private:
    int _ntsc;
};

#endif // ESP_8_BIT_COMPOSITE_H
