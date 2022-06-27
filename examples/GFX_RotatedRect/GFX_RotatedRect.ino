/*

Example for ESP_8_BIT color composite video generator library on ESP32.
Connect GPIO25 to signal line, usually the center of composite video plug.

GFX Rotated Rectangles

This demonstrates using the ESP_8_BIT_GFX class, which inherits from the
Adafruit GFX base class to deliver an easy to use graphics API. Specifically
tests the setRotation function, drawing rectangle in all four orientations
in the same frame. Loops through different parameters of drawRect: X, Y, width,
and height.

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
#include <ESP_8_BIT_GFX.h>

long fractionSecond;
uint8_t currentState;
uint8_t delta;

// Create an instance of the graphics library
ESP_8_BIT_GFX videoOut(true /* = NTSC */, 8 /* = RGB332 color */);

void setup() {
  // put your setup code here, to run once:
  videoOut.begin();

  currentState = 0;
  fractionSecond = millis()%1000;
  delta = 0;
}

void loop() {
  if (millis()%1000 < fractionSecond)
  {
    // Cycle through one of four states every second
    currentState = (currentState+1)%4;
  }
  fractionSecond = millis()%1000;

  // Calculate the fractional progress through animated delta value
  delta = fractionSecond/5;
  if(delta > 100)
  {
    delta = 200-delta;
  }
  
  videoOut.waitForFrame();
  videoOut.fillScreen(0);
  for(uint8_t r = 0; r < 4; r++)
  {
    videoOut.setRotation(r);
    switch(currentState)
    {
      case 0:
        videoOut.drawRect(40+delta, 40, 20, 20, 0xFF);
        break;
      case 1:
        videoOut.drawRect(40, 40+delta, 20, 20, 0xFF);
        break;
      case 2:
        videoOut.drawRect(40, 40, 20+delta, 20, 0xFF);
        break;
      case 3:
        videoOut.drawRect(40, 40, 20, 20+delta, 0xFF);
        break;
      default:
        currentState = 0;
        break;
    }
  }
}
