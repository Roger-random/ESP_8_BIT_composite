/*

Example application of ESP_8_BIT color composite video generator library on ESP32.
Connect GPIO25 to signal line, usually the center of composite video plug.

Plays an animated GIF using the AnimatedGIF library by Larry Bank [bitbank2]
https://github.com/bitbank2/AnimatedGIF

Lightly modified from AnimatedGIF library's example "ESP32_LEDMatrix_I2S"

AnimatedGIF library and example was released under Apache 2.0 license.
https://github.com/bitbank2/AnimatedGIF/blob/master/LICENSE

Cat and Galactic Squid friend by Emily Velasco
https://twitter.com/MLE_Online/status/1393660363191717888
Released under Creative Commons Attribution-ShareAlike (CC BY-SA 4.0) license
https://creativecommons.org/licenses/by-sa/4.0/

Converted to byte array via Unix/Linux command line utility xxd

  xxd -i cat_and_galactic_squid.gif cat_and_galactic_squid.h

Then manually adding 'const' to move it out of precious dynamic memory

*/

#include <AnimatedGIF.h>
#include <ESP_8_BIT_GFX.h>
#include "cat_and_galactic_squid.h"

// Create an instance of the graphics library
ESP_8_BIT_GFX videoOut(true /* = NTSC */, 16 /* = RGB565 colors will be downsampled to 8-bit RGB332 */);
AnimatedGIF gif;

// Vertical margin to compensate for aspect ratio
const int margin = 29;

// Draw a line of image to ESP_8_BIT_GFX frame buffer
void GIFDraw(GIFDRAW *pDraw)
{
    uint8_t *s;
    uint16_t *d, *usPalette, usTemp[320];
    int x, y;

    usPalette = pDraw->pPalette;
    y = pDraw->iY + pDraw->y; // current line

    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2) // restore to background color
    {
      for (x=0; x<pDraw->iWidth; x++)
      {
        if (s[x] == pDraw->ucTransparent)
           s[x] = pDraw->ucBackground;
      }
      pDraw->ucHasTransparency = 0;
    }
    // Apply the new pixels to the main image
    if (pDraw->ucHasTransparency) // if transparency used
    {
      uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
      int x, iCount;
      pEnd = s + pDraw->iWidth;
      x = 0;
      iCount = 0; // count non-transparent pixels
      while(x < pDraw->iWidth)
      {
        c = ucTransparent-1;
        d = usTemp;
        while (c != ucTransparent && s < pEnd)
        {
          c = *s++;
          if (c == ucTransparent) // done, stop
          {
            s--; // back up to treat it like transparent
          }
          else // opaque
          {
             *d++ = usPalette[c];
             iCount++;
          }
        } // while looking for opaque pixels
        if (iCount) // any opaque pixels?
        {
          for(int xOffset = 0; xOffset < iCount; xOffset++ ){
            videoOut.drawPixel(pDraw->iX + x + xOffset, margin + y, usTemp[xOffset]);
          }
          x += iCount;
          iCount = 0;
        }
        // no, look for a run of transparent pixels
        c = ucTransparent;
        while (c == ucTransparent && s < pEnd)
        {
          c = *s++;
          if (c == ucTransparent)
             iCount++;
          else
             s--;
        }
        if (iCount)
        {
          x += iCount; // skip these
          iCount = 0;
        }
      }
    }
    else
    {
      s = pDraw->pPixels;
      // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
      for (x=0; x<pDraw->iWidth; x++)
      {
        videoOut.drawPixel(x,margin + y, usPalette[*s++]);
      }
    }
} /* GIFDraw() */


void setup() {
  videoOut.begin();
  videoOut.copyAfterSwap = true; // gif library depends on data from previous buffer
  videoOut.fillScreen(0);
  videoOut.waitForFrame();

  gif.begin(LITTLE_ENDIAN_PIXELS);
}

void loop() {
  if (gif.open((uint8_t *)cat_and_galactic_squid_gif, cat_and_galactic_squid_gif_len, GIFDraw))
  {
    while (gif.playFrame(true, NULL))
    {
      videoOut.waitForFrame();
    }
    videoOut.waitForFrame();
    gif.close();
  }
}
