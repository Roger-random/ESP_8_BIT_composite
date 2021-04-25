/*

Example for ESP_8_BIT color composite video generator library on ESP32.
Connect GPIO25 to signal line, usually the center of composite video plug.

GFX Hello World

This demonstrates using the ESP_8_BIT_GFX class, which inherits from the
Adafruit GFX base class to deliver an easy to use graphics API. Draws two
rectangles that cycle around the border of the screen. The amount of corners
cut off from these rectangle show the amount of overscan on a particular
screen. In the middle of two rectangles are a bit of text drawn using
Adafruit GFX print() API.

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

// A list of 8-bit color values that work well in a cycle.
uint8_t colorCycle[] = {
  0xFF, // White
  0xFE, // Lowering blue
  0xFD,
  0xFC, // No blue
  0xFD, // Raising blue
  0xFE,
  0xFF, // White
  0xF3, // Lowering green
  0xE7,
  0xE3, // No green
  0xE7, // Raising green
  0xF3,
  0xFF, // White
  0x9F, // Lowering red
  0x5F,
  0x1F, // No red
  0x5F, // Raising red
  0x9F,
  0xFF
};

// Create an instance of the graphics library
ESP_8_BIT_GFX videoOut(true /* = NTSC */, 8 /* = RGB332 color */);

void setup() {
  // Initial setup of graphics library
  videoOut.begin();
}

void loop() {
  // Wait for the next frame to minimize chance of visible tearing
  videoOut.waitForFrame();

  // Get the current time and calculate a scaling factor
  unsigned long time = millis();
  float partial_second = (float)(time % 1000)/1000.0;

  // Use time scaling factor to calculate coordinates and colors
  uint8_t movingX = (uint8_t)(255*partial_second);
  uint8_t invertX = 255-movingX;
  uint8_t movingY = (uint8_t)(239*partial_second);
  uint8_t invertY = 239-movingY;

  uint8_t cycle = colorCycle[(uint8_t)(17*partial_second)];
  uint8_t invertC = 0xFF-cycle;

  // Clear screen
  videoOut.fillScreen(0);

  // Draw one rectangle
  videoOut.drawLine(movingX, 0,       255,     movingY, cycle);
  videoOut.drawLine(255,     movingY, invertX, 239,     cycle);
  videoOut.drawLine(invertX, 239,     0,       invertY, cycle);
  videoOut.drawLine(0,       invertY, movingX, 0,       cycle);

  // Draw a rectangle with inverted position and color
  videoOut.drawLine(invertX, 0,       255,     invertY, invertC);
  videoOut.drawLine(255,     invertY, movingX, 239,     invertC);
  videoOut.drawLine(movingX, 239,     0,       movingY, invertC);
  videoOut.drawLine(0,       movingY, invertX, 0,       invertC);

  // Draw text in the middle of the screen
  videoOut.setCursor(25, 80);
  videoOut.setTextColor(invertC);
  videoOut.setTextSize(2);
  videoOut.setTextWrap(false);
  videoOut.print("Adafruit GFX API");
  videoOut.setCursor(110, 120);
  videoOut.setTextColor(0xFF);
  videoOut.print("on");
  videoOut.setCursor(30, 160);
  videoOut.setTextColor(cycle);
  videoOut.print("ESP_8_BIT video");
}
