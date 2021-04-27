# ESP_8_BIT Color Composite Video Out Library

## Intent

The composite video generation code from
[ESP_8_BIT](https://github.com/rossumur/esp_8_bit)
extracted and packaged into a standalone Arduino library so everyone can
write Arduino sketches that output a color composite video signal.

__Huge thanks to Peter Barrett / rossumur for ESP_8_BIT, without which this
library would not have been possible.__

## Hardware requirement
* ESP32 (tested on ESP32 DevKitC)
* Composite video connector to ESP32 GPIO25 video signal pin.
* Display device with composite video input port. (Usually an old-school tube TV.)

## Arduino requirement
* [Adafruit GFX Library](https://learn.adafruit.com/adafruit-gfx-graphics-library)
available from Arduino IDE Library Manager.
* [Espressif Arduino Core for ESP32](https://github.com/espressif/arduino-esp32),
follow installation directions at that link.

## Installation

This library can now be installed from within the Arduino desktop IDE via the
Library Manager. Listed as "ESP_8_BIT Color Composite Video Library"

It can also be installed from this GitHub repository if desired:
1. Download into a folder named "ESP_8_BIT_composite" under your Arduino IDE's
`libraries` folder.
2. Restart Arduino IDE.

## Classes

1. `ESP_8_BIT_GFX` offers high-level drawing commands via the
[Adafruit GFX API](https://learn.adafruit.com/adafruit-gfx-graphics-library).
Easy to use, but not the highest performance.
2. `ESP_8_BIT_composite` exposes low-level frame buffer for those who prefer
to manipulate bytes directly. Maximum performance, but not very easy to use.

## Examples

1. `GFX_HelloWorld` draws animated rectangles and text, both in changing
colors, using the Adafruit GFX API exposed by `ESP_8_BIT_GFX`.
2. `RGB332_Colors` draws all 256 available colors directly to frame buffer
allocated by `ESP_8_BIT_composite`. Draws once, no updates.
3. `RGB332_PulseB` draws 64 blocks of colors (8x8) representing different
combinations of red (vertical axis) and green (horizontal axis). Uses the
frame buffer of `ESP_8_BIT_composite` directly. Every second, the entire
screen is redrawn with one of four possible values of blue in a pulsing cycle.

## Screen Size

* Inherited from ESP_8_BIT, the addressible screen size is __256 pixels wide
and 240 pixels tall__. This means valid X values of 0 to 255 inclusive, and
valid Y values of 0 to 239 inclusive.
* When displayed on a standard analog TV with 4:3 aspect ratio, these pixels
are not square. So `drawCircle()` will look like a squat wide oval on screen
and not a perfect circle. This is inherent in the system and not considered
a bug.
* When displayed on a standard analog TV, the visible image will be slightly
cropped due to [overscan](https://en.wikipedia.org/wiki/Overscan). This is
inherent to analog televisions and not considered a bug.
* The developer-friendly `ESP_8_BIT_GFX` class checks for valid coordinates
and will clamp within the valid range. So if X is too large (say, 300)
`drawPixel()` will draw at the maximum X of 255.
* The raw `ESP_8_BIT_composite` class gives max performance power, but with
great power comes great responsibility. Caller is responsible for making sure
X and Y stay within bounds when manipulating frame buffer bytes via
`getFrameBufferLines()[Y][X]`. Any bugs that use out of range array index
will at best garble the image, at worse trigger a memory access violation and
cause your ESP32 to reset.

## Help Wanted

When I extracted the video signal generation code from ESP_8_BIT, I tried to
preserve PAL support. Since I only have a NTSC TV, I couldn't verify that PAL
support is still functional. This is
[issue #1](https://github.com/Roger-random/ESP_8_BIT_composite/issues/1).

I would appreciate one of the following:
1. Someone to test PAL and verify it works.
2. Someone to fix any PAL problems and create a pull request.

Note that I can't do anything about "PAL doesn't work" without a fix, as I
have no way to diagnose or verify fixes without a PAL TV.

## Questions?

Please [post to discussions](https://github.com/Roger-random/ESP_8_BIT_composite/discussions)
and see if anyone knows the answer. Note there's no guarantee of an answer.

## Bugs?

Please [open an issue](https://github.com/Roger-random/ESP_8_BIT_composite/issues)
to see if it can be fixed. Note there's no guarantee of support.

## Tip jar

Just like its predecessor ESP_8_BIT, this project is shared freely with the world.
Under the MIT license, you don't owe me anything.

But if you want to toss a few coins my way, use
[my Amazon Associates link](https://amzn.to/3dM0FCn)
for your next Amazon purchase.
(Perhaps some [ESP32 development boards](https://amzn.to/3dMdIDQ)?)
You'll pay the same price, but I get a small
percentage. As an Amazon Associate I earn from qualifying purchases.
