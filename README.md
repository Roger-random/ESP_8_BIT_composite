# ESP_8_BIT Composite Video Generator

## Idea:

Pull the composite video generation code from
[ESP_8_BIT](https://github.com/rossumur/esp_8_bit)
into a standalone Arduino library so others can easily generate Arduino
sketches that output color composite video signal.

Hardware requirement: ESP32

## Status:

Two APIs are available:
1. `ESP_8_BIT_GFX` offers higher-level drawing commands via the
[Adafruit GFX API](https://learn.adafruit.com/adafruit-gfx-graphics-library).
2. `ESP_8_BIT_composite` low-level API for those that want to draw directly
to the frame buffer.

Three examples are available:
1. `GFX_HelloWorld` draws animated rectangles and text, both in changing
colors, using the `ESP_8_BIT_GFX` API.
2. `RGB332_Colors` draws all 256 available colors directly to frame buffer of
`ESP_8_BIT_composite`. Draws once, no updates.
3. `RGB332_PulseB` draws 64 blocks of colors (8x8) representing different
combinations of red (vertical axis) and green (horizontal axis). Uses the
frame buffer of `ESP_8_BIT_composite` directly. Every second, the entire
screen is redrawn with one of four possible values of blue in a pulsing cycle.

## Installation:

1. Download into a folder named "ESP_8_BIT_composite" under your Arduino IDE's
`libraries` folder.
2. Restart Arduino IDE.
