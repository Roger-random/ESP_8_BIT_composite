/*

Example for ESP_8_BIT color composite video generator library on ESP32.
Connect GPIO25 to signal line, usually the center of composite video plug.

GFX Rotated Text

This demonstrates using the ESP_8_BIT_GFX class, which inherits from the
Adafruit GFX base class to deliver an easy to use graphics API. Specifically
tests the setRotation function, drawing text in one of four screen orientations
supported by Adafruit GFX. Also loops through different text sizes as they use
different portions of the code: drawPixel() for text size of one, and fillRect()
for text size larger than one.

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
unsigned long prevTime = 0;
uint8_t ticks = 0;
uint16_t text_width_half;
uint16_t text_height_half;

// Update values of text_width_half and text_height_half
// Call this after changing text size with setTextSize()
void updateTextWidthHeight() {
  int16_t x_discard;
  int16_t y_discard;

  videoOut.getTextBounds("setRotation(0)", 0, 0, 
    &x_discard, &y_discard, &text_width_half, &text_height_half);
  text_width_half /= 2;
  text_height_half /= 2;  
}

void setup() {
  prevTime = millis();
  
  // Initial setup of graphics library
  videoOut.begin();

  // Measure size of rendered text
  videoOut.setTextSize(1);
  updateTextWidthHeight();
}


void loop() {
  unsigned long now = millis();

  if (now > prevTime+1000)
  {
    // Update every second
    prevTime += 1000;
    ticks = (ticks+1)%12;

    // Rotation cycles through 0,1,2,3 using lowest three bits of ticks
    videoOut.setRotation(ticks & 0x03);

    // Text size cycles through 1,2,3 using next two lowest bits
    videoOut.setTextSize((ticks>>2)+1);
    updateTextWidthHeight();
  }
  
  // Wait for the next frame to minimize chance of visible tearing
  videoOut.waitForFrame();

  // Clear screen
  videoOut.fillScreen(0);
  
  // Draw text in the middle of the screen
  videoOut.setCursor((videoOut.width()/2)-text_width_half, (videoOut.height()/2)-text_height_half);
  videoOut.setTextColor(0xFF);
  videoOut.print("setRotation(");
  videoOut.print(ticks & 0x03);
  videoOut.print(")");
}
