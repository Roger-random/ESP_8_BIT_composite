# ESP_8_BIT Color Composite Video Out Library

## Purpose

The composite video generation code from SEGA emulator of
[ESP_8_BIT](https://github.com/rossumur/esp_8_bit)
extracted and packaged into a standalone Arduino library so everyone can
write Arduino sketches that output a color composite video signal.
NTSC and PAL are both supported.

__Huge thanks to Peter Barrett / rossumur for ESP_8_BIT, without which this
library would not have been possible.__

For more behind-the-scenes information on how this library came to be, see
[the development diary](https://newscrewdriver.com/tag/esp_8_bit/)
which has all the details anyone would ever want plus even more that nobody
ever asked for.

## Hardware requirement
* 'Newer' ESP32 (see below)
* Composite video connector to ESP32 GPIO25 video signal pin.
* Display device with composite video input port. (Usually an old-school tube TV.)

__ESP32 Details__

This composite video generation code is an extremely clever hack that used several
ESP32 peripherals in ways they were not originally designed for. See the
[original author's blog documentation](https://rossumblog.com/2020/05/10/130/)
for details. It also means older versions of ESP32 could not run this code.
I don't know exactly which Espressif errata is relevant, but here are some data
points:

* Known to work
  * `ESP32-D0WD (revision 1)` (mine)
  * `ESP32-D0WDQ6 (revision 1)` (thanks [todbot](https://github.com/todbot))
  * `ESP32-PICO-D4 (revision 1)` (thanks [alex1115alex](https://github.com/alex1115alex))
* Known to NOT work
  * `ESP32-D0WDQ6 (revision 0)` (thanks [todbot](https://github.com/todbot))

Chip identification obtained from [ESPTool](https://github.com/espressif/esptool)
with the command `esptool chip_id`

Pushing hardware limits in this manner may restrict this library to a subset of the
ESP32 family. This library was developed and tested against the original suffix-free
ESP32. Compatiblity with variants
([ESP32-S2, ESP32-S3, etc.](https://en.wikipedia.org/wiki/ESP32#ESP32-xx_family))
are untested and unknown.

## Arduino requirement
* [Adafruit GFX Library](https://learn.adafruit.com/adafruit-gfx-graphics-library)
available from Arduino IDE Library Manager. (Last verified to work with v1.11.9)
* [Espressif Arduino Core for ESP32](https://github.com/espressif/arduino-esp32),
follow installation directions at that link. (Last verified to work with v2.0.14)
* (Optional) [AnimatedGIF](https://github.com/bitbank2/AnimatedGIF),
for displaying animated GIF files. (Last verified to work with v1.4.7)
* [Arduino IDE](https://www.arduino.cc/en/software) of course.
(Last verified to work with v2.2.1)

Here's an Arduino IDE screenshot of my ESP32 configuration:

![Screenshot 2023-12-03 142653](https://github.com/Roger-random/ESP_8_BIT_composite/assets/8559196/80512b60-5f2d-4ddc-8fdc-19cd63a1ad70)

## Not Compatible with "Arduino ESP32 Boards by Arduino"

![arduino board manager esp32](https://github.com/Roger-random/ESP_8_BIT_composite/assets/8559196/01b60eb4-8d15-4aec-9f37-d9adfdf6e3f6)

This library is __NOT compatible__ with the "Arduino ESP32 Boards by Arduino" board
library. (thanks [JLBCS](https://github.com/JLBCS) for [#44](https://github.com/Roger-random/ESP_8_BIT_composite/issues/44))
Compilation will fail with the following error:
```
ESP_8_BIT_composite.h:48:10: fatal error: driver/dac.h: No such file or directory
 #include "driver/dac.h"
          ^~~~~~~~~~~~~~
```


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
4. `GFX_Screen_Fillers` demonstrates several of the common ways to put
graphics on screen. Includes the following APIS: `fillRect`, `fillCircle`,
`drawFastVLine`, and `drawFastHLine`.
5. `AnimatedGIF` demonstrates how to use this video out library with the
[AnimatedGIF decoder library](https://github.com/bitbank2/AnimatedGIF)
by Larry Bank. Art used in this example is
[Cat and Galactic Squid](https://twitter.com/MLE_Online/status/1393660363191717888)
by Emily Velasco
([CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/))
6. `GFX_RotatedText` demonstrates support for Adafruit_GFX::setRotation()
by rendering text in one of four orientations and one of three text sizes.
7. `GFX_RotatedRect` demonstrates support for Adafruit_GFX::setRotation()
by drawing four rectangles - one in each supported orientation - on every
frame. Cycles through one of four animated pameters (X/Y/Width/Height)
every second.

## Screen Size

* Inherited from SEGA emulator of ESP_8_BIT, the addressible screen size is __256 pixels wide
and 240 pixels tall__. This means valid X values of 0 to 255 inclusive, and
valid Y values of 0 to 239 inclusive.
  * When displayed on a standard analog TV with 4:3 aspect ratio, these pixels
are not square. So `drawCircle()` will look like a squat wide oval on screen
and not a perfect circle. This is inherent to the system and not considered
a bug.
  * When displayed on a standard analog TV, the visible image will be slightly
cropped due to [overscan](https://en.wikipedia.org/wiki/Overscan). This is
inherent to analog televisions and not considered a bug.
* The developer-friendly `ESP_8_BIT_GFX` class checks for valid coordinates
and will only draw within the valid range. So if X is too large (say, 300)
`drawPixel()` will ignore the command and silently do nothing.
* The raw `ESP_8_BIT_composite` class gives max performance power, but with
great power comes great responsibility. Caller is responsible for making sure
X and Y stay within bounds when manipulating frame buffer bytes via
`getFrameBufferLines()[Y][X]`. Any bugs that use out of range array index
may garble the image, or trigger a memory access violation and cause your ESP32
to reset, or other general memory corruption nastiness __including the
potential for security vulnerabilities.__

## 8-Bit Color

Inherited from ESP_8_BIT is a fixed 8-bit color palette in
[RGB332 format](https://en.wikipedia.org/wiki/List_of_monochrome_and_RGB_color_formats#8-bit_RGB_(also_known_as_3-3-2_bit_RGB)).
The underlying composite video out code always works with this set of colors.
(See [Examples](https://github.com/Roger-random/ESP_8_BIT_composite#examples).)
* The developer-friendly `ESP_8_BIT_GFX` class constructor can be initialized
in either 8-bit (native) or 16-bit (compatibility) color mode.
  * Adafruit GFX was written for 16-bit color in
[RGB565 format](https://learn.adafruit.com/adafruit-gfx-graphics-library/coordinate-system-and-units).
`ESP_8_BIT_GFX` in 16-bit mode is compatible with existing Adafruit GFX
code by automatically downconverting color
while drawing. The resulting colors will be approximate, but they should
closely resemble the original. Using RGB332 color values while in this mode
will result in wrong colors on screen due to interpretation as RGB565 colors.
  * In 8-bit mode, color values given to GFX APIs will be treated as native
8-bit RGB332 values. This is faster because it skips the color conversion
process. Using RGB565 color values while in this mode will result in
wrong colors on screen due to the higher 8 bits being ignored.
* The raw `ESP_8_BIT_composite` class always works in 8-bit RGB332 color.

Sample colors in 8-bit RGB332 format:
|Name|RGB332 (binary)|RGB332 (hexadecimal)|
|----:|:---:|:-----|
|Black  |0b00000000|0x00|
|Blue   |0b00000011|0x03|
|Green  |0b00011100|0x1C|
|Cyan   |0b00011111|0x1F|
|Red    |0b11100000|0xE0|
|Magenta|0b11100011|0xE3|
|Yellow |0b11111100|0xFC|
|White  |0b11111111|0xFF|

## 8-bit RGB332 Color Picker Utility

[CLICK HERE](https://roger-random.github.io/RGB332_color_wheel_three.js/)
for an interactive color picker web app. It shows all 256 possible
8-bit RGB332 colors in either a
[HSV (hue/saturation/value)](https://en.wikipedia.org/wiki/HSL_and_HSV)
color cylinder or a
[RGB (red/green/blue)](https://en.wikipedia.org/wiki/RGB_color_space)
color cube.

## Alternatives

The intent of this library is to be easy to use, with minimum complexity
for beginners. Advanced users are expected to fork this repository to add
their desired features, or go to the source and fork
[ESP_8_BIT](https://github.com/rossumur/esp_8_bit) directly.

If this library is not a good fit for your project, please consider another
library for ESP32 composite video:
* __[Bitluni's Lab](https://bitluni.net/esp32-composite-video)__: one of the
earliest ESP32 composite video generators that informed development of many
that followed.
* __[FabGL](http://www.fabglib.org/)__:
This powerful graphics library didn't have composite video output when I
started my project, but it has since been added. See the
[author's show-and-tell thread](https://github.com/fdivitto/FabGL/discussions/192)
for details. The author has a Tektronix VM700 to ensure PAL video output
signal is accurate.
* __[LovyanGFX](https://github.com/lovyan03/LovyanGFX)__:
Another very nice graphics library that has
[preliminary composite video support in a development branch](https://github.com/lovyan03/LovyanGFX/blob/develop/doc/Panel_CVBS.md).
I believe this library is unique in support for NTSC-J, and documentation
is in both Japanese and English. (Code comments are in Japanese.)

## Questions?

Please [post to discussions](https://github.com/Roger-random/ESP_8_BIT_composite/discussions)
and see if anyone knows the answer. Note there's no guarantee of an answer.

## Bugs?

Please [open an issue](https://github.com/Roger-random/ESP_8_BIT_composite/issues)
to see if it can be fixed. Note there's no guarantee of support.

## Tip jar

Just like its predecessor ESP_8_BIT, this project is shared freely with the world.
Under the MIT license, you don't owe me anything.

But if you want to toss a few coins my way, you can do so by using my
Amazon Associates link to buy your
[ESP32 development boards](https://amzn.to/3dMdIDQ)
or
[composite video cables](https://amzn.to/33K9qXP).
You'll pay the same price, but I get a small
percentage. As an Amazon Associate I earn from qualifying purchases.
