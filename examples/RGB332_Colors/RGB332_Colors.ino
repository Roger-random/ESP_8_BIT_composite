/*

Example for ESP_8_BIT color composite video generator library on ESP32.
Connect GPIO25 to signal line, usually the center of composite video plug.

RGB332_Colors

Draws 256 blocks on screen, each one filled with a RGB332 color. This
represents the full palette of available colors while using this library.

Also demonstrates drawing by directly manipulating bytes in frame buffer,
without using any drawing library functions.

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

#include <ESP_8_BIT_composite.h>

ESP_8_BIT_composite video_out(true /* = NTSC */);

void setup() {
  video_out.setup();
  uint8_t** frameBufferLines = video_out.get_lines();

  // Draw all the colors available in our RGB332 palette
  uint8_t yMask;
  for (int y = 0; y < 240; y++)
  {
    yMask = (y/30) << 5;
    for (int x = 0; x < 256; x++)
    {
      frameBufferLines[y][x] = yMask | x >> 3;
    }
  }
}

void loop() {
}
