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

#include "ESP_8_BIT_GFX.h"

static const char *TAG = "ESP_8_BIT_GFX";

ESP_8_BIT_GFX::ESP_8_BIT_GFX(bool ntsc, uint8_t colorDepth)
  : Adafruit_GFX(256, 240)
{
  _pVideo = new ESP_8_BIT_composite(ntsc);
  if (NULL==_pVideo)
  {
    ESP_LOGE(TAG, "Video signal generator allocation failed");
    ESP_ERROR_CHECK(ESP_FAIL);
  }

  if (8 == colorDepth || 16 == colorDepth)
  {
    _colorDepth = colorDepth;
  }
  else
  {
    ESP_LOGE(TAG, "Unsupported color depth");
    ESP_ERROR_CHECK(ESP_FAIL);
  }
}

void ESP_8_BIT_GFX::setup()
{
  _pVideo->setup();
}

void ESP_8_BIT_GFX::setup_prealloc(uint8_t** allocated_lines)
{
  _pVideo->setup_prealloc(allocated_lines);
}

void ESP_8_BIT_GFX::vsync()
{
  _pVideo->vsync();
}

// Extract most significant 3 red, 3 green and 2 blue bits.
uint8_t ESP_8_BIT_GFX::RGB565toRGB332(uint16_t color)
{
  return (uint8_t)(
        (color & 0xE000) >> 8 |
        (color & 0x0700) >> 6 |
        (color & 0x0018) >> 3
      );
}

uint8_t ESP_8_BIT_GFX::getColor8(uint16_t color)
{
  switch(_colorDepth)
  {
    case 8:
      // Use lower 8 bits directly
      return (uint8_t)color;
      break;
    case 16:
      // Downsample from 16 to 8-bit color.
      return RGB565toRGB332(color);
      break;
  }
}

void ESP_8_BIT_GFX::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if (y > 239)
  {
    ESP_LOGE(TAG, "Clamping Y");
    y = 239;
  }
  if (x > 255)
  {
    ESP_LOGE(TAG, "Clamping X");
    x = 255;
  }
  _pVideo->get_lines()[y][x] = getColor8(color);
}

void ESP_8_BIT_GFX::fillScreen(uint16_t color)
{
  uint8_t color8 = getColor8(color);

  // We can't do a single memset() because it is possible for _lines to point
  // into non-contingous pieces of memory.
  for(uint8_t y = 0; y < 240; y++)
  {
    memset(_pVideo->get_lines()[y], color8, 256);
  }
}
