#include "ESP_8_BIT_composite_video.h"

ESP_8_BIT_composite_video::ESP_8_BIT_composite_video(int ntsc)
{
  printf("ESP_8_BIT_composite_video() constructor\n");
  _ntsc = ntsc;
}

void ESP_8_BIT_composite_video::print_hello()
{
  printf("Howdy everyone\n");
}
