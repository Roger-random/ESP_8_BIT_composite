/*

Example for ESP_8_BIT color composite video generator library on ESP32.
Connect GPIO25 to signal line, usually the center of composite video plug.

GFX Screen Fillers

This demonstrates using the ESP_8_BIT_GFX class, which inherits from the
Adafruit GFX base class to deliver an easy to use graphics API. This example
cycles through four common ways to fill screen area.

1. fillRect() - fills the screen with fillRect() API.
2. fillCircle() - fills the screen with fillCircle() API.
3. drawFastVLine() - fills the screen by drawing many vertical lines.
4. drawFastHLine() - fills the screen by drawing many horizontal lines.

These samples should run smoothly without visible flickering or visual
artifacts. If they do, one of the following performance enhancements of
this library probably has a bug that requires investigation:
* Double-buffering
* Optimized fillRect() override
* Optimized drawFastVLine() override
* Optimized drawFastHLine() override

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

///////////////////////////////////////////////////////////////
//
//  Helpers to cycle through colors of varying bright hues.
//
//  Technical explanation: These colors are a subset of the 8-bit
//  RGB332 colors supported by ESP_8_BIT_GFX. When reorganized
//  in HSV color model, these are colors of various hues (H)
//  all with full saturation (S == 1.0) and value (V == 1.0)

uint8_t hues[] = {
  0xE0, // Red@0
  0xE4,
  0xE8,
  0xEC,
  0xF0,
  0xF4,
  0xF8,
  0xFC, // Yellow
  0xDC,
  0xBC,
  0x9C,
  0x7C,
  0x5C,
  0x3C,
  0x1C, // Green
  0x1D,
  0x1E,
  0x1F,
  0x1B,
  0x17,
  0x13,
  0x0F,
  0x0B,
  0x07,
  0x03, // Blue
  0x23,
  0x43,
  0x63,
  0x83,
  0xA3,
  0xC3,
  0xE3,
  0xE2,
  0xE1   // 33
};

// Index into hues[] of the current hue
uint8_t hueIndex;

// Constaintly cycling through hues[] is overly stimulating
// so we stay with a particular hue for a while before moving
// on to the next hue.
const uint8_t framesPerHue = 10;

// How many frames we have stayed on the current hue, should
// be incremented on each use and never exceed framesPerHue
uint8_t hueFrames;

// Called on each draw to select a color from hues[]. Increments
// hueFrames on each call and update hueIndex as needed.
uint8_t nextHue() {
  if (++hueFrames > framesPerHue) {
    hueFrames = 0;
    if ( ++hueIndex > 33) {
      hueIndex = 0;
    }
  }

  return hues[hueIndex];
}

// Counts through stages of tests
uint8_t stage;

// Tracks the progress percentage to know when we need to move to next test.
float previousProgress;

// How many milliseconds to spend on each stage of test.
uint32_t millisPerStage = 5000;

void setup() {
  // Initial setup of graphics library
  videoOut.begin();

  // Starting stage for tests cycle
  stage = 4;

  // Start hue cycle at beginning
  hueIndex = 0;
  hueFrames = 0;
}

void loop() {
  // Get the current time and calculate a scaling factor
  float progress = (float)(millis() % millisPerStage)/(float)millisPerStage;
  if (progress < previousProgress)
  {
    printf("Stage %d spent %.2f%% of time waiting for frame. (Higher is better.)\n",
      stage, (float)videoOut.newPerformanceTrackingSession()/100);
    if (++stage > 4) {
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

  // Wait for the next frame to start working on frame buffer
  videoOut.waitForFrame();

  // Clear screen
  videoOut.fillScreen(0);

  // Draw one of four tests
  switch(stage)
  {
    case 0:
      // A rectangle that grows from the center of the screen, then shrinks.
      videoOut.fillRect(128*(1-progress),120*(1-progress),256*progress,240*progress, nextHue());
      break;
    case 1:
      // A circle that grows from the center of the screen, then shrinks.
      videoOut.fillCircle(128,120,130*progress,nextHue());
      break;
    case 2:
      // Draws horizontally across the screen with lots of vertical lines.
      // (In this graphics library, vertical lines are less efficient than horizontal lines.)
      hueIndex = 0;
      hueFrames = 0;
      for(int16_t x = 128*(1-progress); x < 128+(128*progress); x++)
      {
        videoOut.drawFastVLine(x, 0, 240, nextHue());
      }
      break;
    case 3:
      // Draws verticall across the screen with lots of horizontal lines.
      // (In this graphics library, horizontal lines are the most efficient.)
      hueIndex = 0;
      hueFrames = 0;
      for(int16_t y = 120*(1-progress); y < 120+(120*progress);y++)
      {
        videoOut.drawFastHLine(0, y, 256, nextHue());
      }
      break;
    case 4:
      // Regression test for https://github.com/Roger-random/ESP_8_BIT_composite/issues/8
      videoOut.fillCircle(335*progress-40, 120, 40+20*progress, nextHue());
      videoOut.fillCircle(128, 319*progress-40, 40-20*progress, hues[(hueIndex+17)%34]);
      break;
    default:
      stage = 0;
      break;
  }
}
