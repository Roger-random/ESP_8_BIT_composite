#ifndef ESP_8_BIT_COMPOSITE_VIDEO_H
#define ESP_8_BIT_COMPOSITE_VIDEO_H

#include "Arduino.h"

class ESP_8_BIT_composite_video
{
  public:
    ESP_8_BIT_composite_video(int ntsc);
    void print_hello();
  private:
    int _ntsc;
};

#endif // ESP_8_BIT_COMPOSITE_VIDEO_H
