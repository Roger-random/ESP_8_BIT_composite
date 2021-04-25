/*

Adafruit_GFX for ESP_8_BIT color composite video.

NOT AN OFFICIAL ADAFRUIT GRAPHICS LIBRARY.

Allows Arduino sketches to draw to a composite video device using Adafruit's
graphics API.

Copyright (c) Roger Cheng

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef ESP_8_BIT_GFX_H
#define ESP_8_BIT_GFX_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "ESP_8_BIT_composite.h"

class ESP_8_BIT_GFX : public Adafruit_GFX {
  public:
    ESP_8_BIT_GFX(bool ntsc, uint8_t colorDepth);
    void setup();
    void setup_prealloc(uint8_t** allocated_lines);
    void vsync();
    uint8_t RGB565toRGB332(uint16_t color);

    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void fillScreen(uint16_t color);
  private:
    uint8_t _colorDepth;
    ESP_8_BIT_composite* _pVideo;

    uint8_t getColor8(uint16_t color);
};

#endif // #ifndef ESP_8_BIT_GFX_H
