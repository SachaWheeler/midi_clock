# MIDI Clock Announcer

This Arduino project announces the current time via MIDI messages, using an electronic drum machine (Vermona DRM1 MARK III) as the sound source.

## Overview

`midi_clock` lets you turn the passage of time into rhythm! At user-defined intervals, the Arduino transmits MIDI note messages that trigger specific sounds on your drum machine to represent the current hour, minute, or other time markers.

- **Platform:** Arduino (tested on UNO/Nano, adaptable to others)
- **Output:** Standard 5-pin MIDI DIN connector
- **Target Drum Machine:** Vermona DRM1 MARK III (or any MIDI-capable drum module)
- **Use Case:** Musical time announcements, creative automation, MIDI-based art installations

## Features

- Announces the time by sending MIDI notes to a drum machine
- Configurable patterns for hours, minutes, and/or other intervals
- Customizable mapping of time units to drum sounds (e.g., kick for hours, snare for minutes)
- Simple, hackable Arduino code

## Hardware Requirements

- Arduino board (Uno, Nano, etc.)
- MIDI DIN-5 socket and optocoupler circuit (standard Arduino MIDI out circuit)
- Vermona DRM1 MARK III or any drum machine that accepts MIDI note triggers
- (Optional) Real-Time Clock (RTC) module for accurate timekeeping (e.g., DS3231)

## Wiring

1. Connect the MIDI DIN socket to your Arduino using a standard MIDI OUT circuit. [See MIDI Arduino circuit examples](https://www.midi.org/midi-articles/traditional-midi-5-pin-din-cables).
2. Connect the Arduino to the Vermona DRM1 via a MIDI cable.
3. (Optional) Connect the RTC module via I2C for precise timekeeping.

## Usage

1. Upload the `midi_clock` sketch to your Arduino.
2. Adjust the MIDI channel and note mappings in the code, if needed, to match your drum machine's configuration.
3. Power up the Arduino and the drum machine.
4. Listen for time announcements as rhythmic patterns!

## Example

- Each hour, the Arduino sends a series of MIDI notes to play a kick drum.
- Each quarter hour, a snare or other sound is triggered.
- The patterns and intervals are customizable in the Arduino code.

## Customization

- Change which MIDI notes are sent for each time unit
- Adjust announcement intervals (e.g., on the hour, half hour, every minute)
- Map different time periods to different drum sounds or channels

## License

MIT

---

© 2025 Sacha Wheeler
