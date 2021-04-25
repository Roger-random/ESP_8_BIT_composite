/*

Adafruit_GFX for ESP_8_BIT color composite video.

NOT AN OFFICIAL ADAFRUIT GRAPHICS LIBRARY.

Allows ESP32 Arduino sketches to draw to a composite video device using
Adafruit's graphics API.

NOTE RE:COLOR

Adafruit GFX is designed for 16-bit (RGB565) color, but ESP_8_BIT video
only handles 8-bit (RGB332) color. There are two ways to handle this,
depending on passsing "8" or "16" into the constructor:

8  = Truncate the 16-bit color values and use the lower 8 bits directly as
     RGB332 color. This is faster, but caller needs to know to use 8-bit
     color values. A good choice when writing new code using this library.
16 = Automatically extract the most significant 3 red, 3 green, and 2 blue
     bits from a 16-bit RGB565 color value to generate a RGB332 color.
     Performing this conversion slows down the code, but the caller does not
     need to know about the limitations. A good choice when reusing existing
     Adafruit GFX code that works in 16-bit color space.

An utility function RGB565toRGB332 is available to perform this conversion.

NOTE RE:ASPECT RATIO

Adafruit GFX assumes pixels are square, but this is not true of ESP_8_BIT
which has nonsquare pixels. (4:3 aspect ratio in a 256x240 frame buffer.)
Circles will look squashed as wide ovals, etc. This version of the API does
not offer any way to compensate, the caller has to deal with it.



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

/*
 * @brief Expose Adafruit GFX API for ESP_8_BIT composite video generator
 */
class ESP_8_BIT_GFX : public Adafruit_GFX {
  public:
    /*
     * @brief Constructor
     * @param ntsc true for NTSC, false for PAL
     * @param colorDepth 8 to treat color as 8-bit directly, 16 to perform
     *        downconversion from 16-bit RGB565 color to 8-bit RGB332.
     */
    ESP_8_BIT_GFX(bool ntsc, uint8_t colorDepth);

    /*
     * @brief Call once to set up the API with self-allocated frame buffer.
     */
    void setup();

    /*
     * @brief Call once to set up the API with caller-allocated buffer.
     * @param allocated_lines Caller-allocated (and freed) buffer.
     */
    void setup_prealloc(uint8_t** allocated_lines);

    /*
     * @brief Wait for frame render to complete, to avoid tearing.
     */
    void vsync();

    /*
     * @brief Utility to convert from 16-bit RGB565 color to 8-bit RGB332 color
     */
    uint8_t RGB565toRGB332(uint16_t color);

    /*
     * @brief Required Adafruit_GFX override to put a pixel on screen
     */
    void drawPixel(int16_t x, int16_t y, uint16_t color);

    /*
     * @brief Optional Adafruit_GFX override to optimize clearing the screen
     */
    void fillScreen(uint16_t color);
  private:
    /*
     * @brief Whether to treat color as 8 or 16 bit color values
     */
    uint8_t _colorDepth;

    /*
     * @brief Internal reference to ESP_8_BIT video generator wrapper class
     */
    ESP_8_BIT_composite* _pVideo;

    /*
     * @brief Retrieve color to use depending on _colorDepth
     */
    uint8_t getColor8(uint16_t color);
};

#endif // #ifndef ESP_8_BIT_GFX_H
