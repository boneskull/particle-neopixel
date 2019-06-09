# particle-neopixel

> Color cycle a NeoPixel (WS2812B) strip w/ Particle Argon/Xenon/Boron

There are many like it, but this one is mine.

## Particle Cloud Functions

- `enable()` - Turns the NeoPixel strip "on"
- `disable()` - Turns the NeoPixel strip "off"
- `setBrightness(short)` - Set brightness from 1-100, inclusive
- `setDelay(short)` - Set update delay in ms

> Note: Particle Cloud Functions only accept a single `String` value; any args are converted/coerced into the proper data type.

## Particle Cloud Variables

- `settings` - `{String}`:  All settings in a JSON object.  Properties:
  - `enabled` - `{bool}` (default `true`): Whether or not the strip is currently enabled
  - `delay` - `{short}` (default `20`): Current delay setting
  - `brightness` - `{short}` (default `50`): Current brightness setting

## Notes

- Settings are persisted in EEPROM.  Should I be doing this?  I have no idea.
- `setBrightness(0)` just invokes `disable()`. It does not save `0` as the `brightness` setting.
- Very obviously adapted from Adafruit's "rainbow" example.

## License

Copyright © 2019 Christopher Hiller. Licensed Apache-2.0
