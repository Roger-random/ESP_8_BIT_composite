/*

Example for ESP_8_BIT color composite video generator library on ESP32.
Connect GPIO25 to signal line, usually the center of composite video plug.

HSV_Colors

Display colors available while using this library arranged as HSV
(Hue, Saturation, Value) spectrum converted to underlying RGB332.

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

// From https://www.vagrearg.org/content/hsvrgb
#include "fast_hsv2rgb.h"

ESP_8_BIT_composite video_out(true /* = NTSC */);

// HSV to RGB conversion code copied from
// https://www.codespeedy.com/hsv-to-rgb-in-cpp/
// Modified to return 8-bit RGB332 instead of printing to cout

uint8_t HSVtoRGB332(float H, float S,float V){
  if(H>360 || H<0 || S>1.0 || S<0 || V>1.0 || V<0)
  {
    printf("The given HSV values are not in valid range.\n");
    return 0;
  }

  float s = S;
  float v = V;
  float C = s*v;
  float X = C*(1-fabs(fmod(H/60.0, 2)-1));
  float m = v-C;
  float r,g,b;
  if(H >= 0 && H < 60){
      r = C,g = X,b = 0;
  }
  else if(H >= 60 && H < 120){
      r = X,g = C,b = 0;
  }
  else if(H >= 120 && H < 180){
      r = 0,g = C,b = X;
  }
  else if(H >= 180 && H < 240){
      r = 0,g = X,b = C;
  }
  else if(H >= 240 && H < 300){
      r = X,g = 0,b = C;
  }
  else{
      r = C,g = 0,b = X;
  }

  uint8_t R = (uint8_t)((r+m)*8) << 5;
  uint8_t G = (uint8_t)((g+m)*8) << 2;
  uint8_t B = (uint8_t)((b+m)*4);

  return R | G | B;
}

// Modified from
// https://dystopiancode.blogspot.com/2012/06/hsv-rgb-conversion-algorithms-in-c.html

bool HSV_IsValid(float h, float s, float v)
{
  if ( h >= 0.0 && h <= 360.0 &&
       s >= 0.0 && s <= 1.0 &&
       v >= 0.0 && v <= 1.0 )
  {
    return true;
  }

  return false;
}

uint8_t RGB332_Create(float r, float g, float b)
{
  uint8_t R = (uint8_t)(r*8) << 5;
  uint8_t G = (uint8_t)(g*8) << 2;
  uint8_t B = (uint8_t)(b*4);

  return R | G | B;
}

uint8_t RGB332_CreateFromHSV(float h, float s, float v)
{
    float c = 0.0, m = 0.0, x = 0.0;
    uint8_t colorRGB332 = 0;
    if (HSV_IsValid(h, s, v) == true)
    {
        c = v * s;
        x = c * (1.0 - fabs(fmod(h / 60.0, 2) - 1.0));
        m = v - c;
        if (h >= 0.0 && h < 60.0)
        {
            colorRGB332 = RGB332_Create(c + m, x + m, m);
        }
        else if (h >= 60.0 && h < 120.0)
        {
            colorRGB332 = RGB332_Create(x + m, c + m, m);
        }
        else if (h >= 120.0 && h < 180.0)
        {
            colorRGB332 = RGB332_Create(m, c + m, x + m);
        }
        else if (h >= 180.0 && h < 240.0)
        {
            colorRGB332 = RGB332_Create(m, x + m, c + m);
        }
        else if (h >= 240.0 && h < 300.0)
        {
            colorRGB332 = RGB332_Create(x + m, m, c + m);
        }
        else if (h >= 300.0 && h < 360.0)
        {
            colorRGB332 = RGB332_Create(c + m, m, x + m);
        }
        else
        {
            colorRGB332 = RGB332_Create(m, m, m);
        }
    }
    return colorRGB332;
}

float currentS;
float deltaS;

void setup() {
  // put your setup code here, to run once:
  video_out.setup();

  currentS = 1.0;
  deltaS = -0.025;
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t** frameBufferLines = video_out.get_lines();

  video_out.vsync();

  for (uint8_t y = 0; y < 240; y++)
  {
    for (uint8_t x = 0; x < 256; x++)
    {
      frameBufferLines[y][x] = RGB332_CreateFromHSV(
        360*(float)x/256,
        currentS,
        (float)(y+16)/256);
    }
  }

  if (currentS >= 1.0)
  {
    deltaS = -0.025;
  }
  else if (currentS <= 0)
  {
    deltaS = 0.025;
  }
  currentS += deltaS;
}
