/*

Example for ESP_8_BIT color composite video generator library on ESP32.
Connect GPIO25 to signal line, usually the center of composite video plug.

GFX Fill Limits

This demonstrates using the ESP_8_BIT_GFX class, which inherits from the
Adafruit GFX base class to deliver an easy to use graphics API. But the ease
of use is a tradeoff against performance, and this sample sketch pushes
close to the limits in each of four APIs.

1. fillRect() - can almost fill the entire screen before anything flickers.
2. fillCircle() - radius of 100 is solid, more than that starts to flicker.
3. drawFastVLine() - we can fill about half the screen before flickering.
4. drawFastHLine() - we can fill about half the screen before flickering.

Sketches that demand even greater performance will have to give up on the
Adafruit GFX API and work with the frame buffer directly via
ESP_8_BIT_composite class.

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

// Create an instance of the graphics library
ESP_8_BIT_GFX videoOut(true /* = NTSC */, 8 /* = RGB332 color */);

// Counts through one of four tests
uint8_t stage;

// Tracks the progress percentage to know when we need to move to next test.
float previousProgress;

// How many milliseconds to spend on each stage of test.
uint32_t millisPerStage = 3000;

void setup() {
  // Initial setup of graphics library
  videoOut.begin();

  // Start multistage test at beginning
  stage = 0;
}

void loop() {
  // Get the current time and calculate a scaling factor
  float progress = (float)(millis() % millisPerStage)/(float)millisPerStage;
  if (progress < previousProgress)
  {
    stage++;
    if (stage > 3) {
      stage = 0;
    }
  }
  previousProgress = progress;

  // Modify progress so it goes from 0.0 to 1.0 and back to 0.0 in each stage.
  if (progress < 0.5)
  {
    progress *= 2;
  }
  else
  {
    progress = (1-progress)*2;
  }
  
  // Wait for the next frame to minimize chance of visible tearing
  videoOut.waitForFrame();

  // Clear screen
  videoOut.fillScreen(0);

  // Draw one of four tests
  switch(stage)
  {
    case 0: 
      videoOut.fillRect(128*(1-progress),120*(1-progress),256*progress,240*progress, 0xF0);
      break;
    case 1:
      videoOut.fillCircle(128,120,110*progress,0xBC);
      break;
    case 2:
      for(int16_t x = 128*(1-progress); x < 128+(128*progress); x++)
      {
        videoOut.drawFastVLine(x, 60, 120, 0x1F);
      }
      break;
    case 3:
      for(int16_t y = 120*(1-progress); y < 120+(120*progress);y++)
      {
        videoOut.drawFastHLine(64, y, 128, 0xE3);
      }
      break;
    default:
      stage = 0;
      break;
  }
}
