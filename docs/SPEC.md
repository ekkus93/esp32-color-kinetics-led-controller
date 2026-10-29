# Experimental Chromasic ESP32-S3 controller specification

## Hardware
- ESP32-S3 N16R8: 16 MB flash, 8 MB PSRAM (PSRAM unused here).
- Color Kinetics eW Flex Compact HE, item 999-009302-44:
  60 fixed-white 4000 K nodes, nominal 12-inch pitch, label 24 V / 2.5 A max.
- No original Color Kinetics controller assumed.

## Protocol (NOT yet verified on physical LEDs)
From Glen Akins' iColor Flex reverse-engineering work:
- 454.545 kbit/s, 2.2 microseconds/bit.
- Zero: 0.6 us HIGH, 1.6 us LOW; one: 1.6 us HIGH, 0.6 us LOW.
- Start code: 13 one-bits + 8 zero-bits = 21 bits.
- Three framed, 12-bit, LSB-first channel values per node;
  fields are 20, 20, and 21 bits long (61 bits/node).
- First channel for white brightness; other two zero (unverified).
- Brightness 0–4094; 4095 reserved.
- Six additional zero-valued flush nodes, 6000 zero-bit idle intervals
  (experimental parameter).
- Total 6000 + 21 + 66 * 61 = 10047 bits (~22.1 ms / frame; ~45.2 Hz).
- RMT simple encoder with DMA attempts continuous output; **no gap-free
  transmission or underrun verification on real hardware yet.**

## Electrical interface — hard safety boundary
The original leader cable's conductor pinout is unknown: verify by measurement
before wiring. Never connect 24 V to GPIO or directly attach unknown conductors.

- Verified single-ended data input: use suitable 5 V logic level buffer
  (for example 74AHCT125 powered by a regulated 5 V source).
- Verified four-wire differential input: use appropriate RS-485-class
  *electrical* driver for raw Chromasic pulses after confirming pair,
  polarity, termination, voltages and reference. Not CAN or UART protocol.
- Separate fused, current-limited 24 V supply rated at least 3 A.
- Default firmware keeps CK_OUTPUT_ENABLED=0 and does not transmit.

## Acceptance
1. Host tests compile with strict warnings and pass.
2. ESP-IDF v6.1 ESP32-S3 N16R8 exact-revision firmware build passes.
3. Physical ESP32 serial log confirms safe mode (pending).
4. Logic analyzer verifies bit timing, idle and no DMA underruns (pending).
5. Pinout and line driver verified before attaching LEDs (pending).
6. Dim single-node through all-60-node test successful (pending).

Passing compilation alone does not prove device compatibility.

## Signal diagrams
See [CHROMASIC_SIGNALS.md](CHROMASIC_SIGNALS.md) for wire-level bit timing,
field layouts, the nominal 10,047-bit recurring frame and the proposed
RS-485-class electrical interface, with the [editable timing SVG](CHROMASIC_SIGNAL_DIAGRAM.svg).
The 6,000-bit idle interval and the fixed-white channel-1-only assignment are
experimental choices, not proven requirements of this particular LED string.
