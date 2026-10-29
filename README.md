# ESP32-S3 Color Kinetics LED Controller

Experimental **safe-by-default Chromasic transmitter** for the Color Kinetics
**eW Flex Compact HE** 60-node, fixed-white 4000 K LED string (item
999-009302-44, labeled **24 V DC / 2.5 A maximum**).

Uses **ESP-IDF v6.1** on an **ESP32-S3 N16R8** (16 MB flash, 8 MB PSRAM).
The code synthesizes Chromasic-style timing with RMT + DMA. The physical
power/data interface and actual LED compatibility **have not been verified**.

## Behavior

- **Default SAFE build:** boots and logs diagnostics; GPIO18 remains disabled.
- **Experimental mode only after electrical checks:** emits a continuous
  ~454.5 kbit/s Chromasic waveform; runs a moving, 60-node white-light
  pattern at brightness 700/4094, advancing every 200 ms, then pausing.
- Each node has an independently settable 12-bit brightness value (0–4094).
  The first Chromasic channel controls white; other channels are zero.
- No Wi-Fi, web interface, WLED or Home Assistant support in the prototype.

## Build and test

Install ESP-IDF **v6.1** and activate its environment. The pinned ESP-IDF
revision is **fff9895c82d744c7237be8847347bdd1b07c6643**, previously
built with **Xtensa GCC 15.2.0**.

    idf.py set-target esp32s3
    idf.py build
    idf.py -p /dev/ttyACM0 flash monitor

Use your actual serial device. The committed sdkconfig.defaults selects the
16 MB flash size; this prototype does not use PSRAM. The generated sdkconfig
and build outputs are ignored by Git.

    bash tests/run_host_tests.sh

GitHub Actions runs host tests and a clean safe-mode firmware cross-build
from the pinned SDK source. The compiled firmware, bootloader and partition
table are downloadable as CI artifacts; binaries are not committed.

## Electrical warning

**Do not connect 24 V to any ESP32 pin. Do not connect GPIO18 directly to
an unidentified four-wire Color Kinetics leader.** Verify connector mapping
and signaling before enabling the output (CK_OUTPUT_ENABLED in main/main.c).

A verified three-wire single-ended node input may need a 5 V logic buffer.
A verified differential leader may require an RS-485-class driver carrying
raw Chromasic pulses, **not CAN, UART, Modbus or DMX framing**. Use a suitable
fused 24 V power supply (at least 3 A) and inspect GPIO timing with a logic
analyzer before attaching LEDs.

## Repository

- main/main.c — RMT/DMA transmitter and moving-node test.
- main/chromasic_protocol.[ch] — hardware-independent bitstream framing.
- tests/ — host C tests.
- docs/SPEC.md — protocol model and safety constraints.
- docs/TODO.md — implementation and physical acceptance roadmap.
- docs/PROGRESS.md — validation record.
- .github/workflows/firmware.yml — pinned ESP-IDF CI.

Research reference: https://bikerglen.com/blog/reverse-engineering-the-icolor-flex-protocol/

ESP-IDF RMT API: https://docs.espressif.com/projects/esp-idf/en/v6.1/esp32s3/api-reference/peripherals/rmt.html

This is independent, experimental research, not a certified or endorsed
Color Kinetics/Signify controller. Apache 2.0 license (LICENSE).

## Wiring

The [editable SVG wiring diagram](docs/WIRING_DIAGRAM.svg) shows the proposed ESP32-S3, MAX485-type transmitter, buck converter, 24V supply and four-wire leader. Read the [wiring and verification notes](docs/WIRING.md) before attaching power: physical connector pin positions have **not** been verified.

## Chromasic waveform and frame format

See the [editable SVG timing/signal diagram](docs/CHROMASIC_SIGNAL_DIAGRAM.svg) and the [protocol signal reference](docs/CHROMASIC_SIGNALS.md) for 0/1 pulse widths, framed 12-bit brightness words, 60-node frame timing, and the ESP32-to-differential physical layer. The diagrams are **illustrative**, not measured waveforms.
