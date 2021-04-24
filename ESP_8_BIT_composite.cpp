#include "ESP_8_BIT_composite.h"

static const char *TAG = "ESP_8_BIT";

ESP_8_BIT_composite::ESP_8_BIT_composite(int ntsc)
{
  _ntsc = ntsc;
}

void ESP_8_BIT_composite::print_hello()
{
  ESP_LOGV(TAG, "Howdy hi");
}
