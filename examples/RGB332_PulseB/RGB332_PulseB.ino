/*

Example for ESP_8_BIT color composite video generator library on ESP32.
Connect GPIO25 to signal line, usually the center of composite video plug.

RGB332_PulseB

Draws 8 by 8 = 64 blocks on screen, each one filled with a color. More red
down the vertical axis, more green across the horizontal axis. Value of
blue over the entire screen changes once per second in a pulsing cycle.

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

uint8_t currentB;
uint8_t deltaB;

void setup() {
  // put your setup code here, to run once:
  video_out.setup();

  currentB = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t yMask;

  uint8_t** frameBufferLines = video_out.get_lines();
  video_out.vsync();

  // Draw all the colors available in our RGB332 palette
  for (int y = 0; y < 240; y++)
  {
    yMask = (y/30) << 5;
    for (int x = 0; x < 256; x++)
    {
      frameBufferLines[y][x] = yMask | (x & 0xE0) >> 3 | currentB;
    }
  }

  if (0 == currentB)
  {
    deltaB = 1;
  }
  else if (3 <= currentB)
  {
    deltaB = -1;
  }
  currentB += deltaB;

  delay(1000);
}
