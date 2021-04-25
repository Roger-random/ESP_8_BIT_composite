/*

Example for ESP_8_BIT color composite video generator library on ESP32.
Connect GPIO25 to signal line, usually the center of composite video plug.

RGB332_PulseB

Draws 8 by 8 = 64 blocks on screen, each one filled with a color. More red
down the vertical axis, more green across the horizontal axis. Value of
blue over the entire screen changes once per second in a pulsing cycle.

Also demonstrates drawing by directly manipulating bytes in frame buffer,
without using any drawing library functions. This is an advanced usage
scenario, see GFX_HelloWorld for a more user-friendly interface.

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

ESP_8_BIT_composite videoOut(true /* = NTSC */);

// Current value for blue channel
uint8_t currentBlue;

// Blue channel change for next frame
uint8_t changeBlueBy;

void setup() {
  videoOut.begin();
  currentBlue = 0;
}

void loop() {
  uint8_t redChannel;
  uint8_t** frameBufferLines = videoOut.getFrameBufferLines();

  // Wait for the next frame to minimize chance of visible tearing
  videoOut.waitForFrame();

  // Draw all the colors available in our RGB332 palette with current blue
  for (int y = 0; y < 240; y++)
  {
    // Y axis determines red channel (most significant 3 bits of RGB332)
    redChannel = (y/30) << 5;
    for (int x = 0; x < 256; x++)
    {
      // X axis determines green channel (middle 3 bits of RGB332)
      frameBufferLines[y][x] = redChannel | (x & 0xE0) >> 3 | currentBlue;
    }
  }

  // Update blue channel value for next redraw, pulses 0-1-2-3-2-1-0
  // for the least significant 2 bits of RGB222.
  if (0 == currentBlue)
  {
    changeBlueBy = 1;
  }
  else if (3 <= currentBlue)
  {
    changeBlueBy = -1;
  }
  currentBlue += changeBlueBy;

  // Wait before next redraw
  delay(1000);
}
