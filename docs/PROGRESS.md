# Progress / acceptance evidence

## Initial source import — 2026-10-28
- Source recovered from the previously compiled experimental Chromasic
  ESP32-S3 package.
- Prior isolated compilation reported successful safe and active ESP-IDF
  v6.1 builds using Xtensa GCC 15.2.0, with 16 MB flash configured.
- Repository host tests passed with GCC and Clang before import.
- CI defined to use exact ESP-IDF source SHA
  fff9895c82d744c7237be8847347bdd1b07c6643.
- **Physical validation: none.** Connector pinout, output voltage/polarity,
  scope measurements, board boot and actual LED response are still pending.
- **CI: pending exact-commit result.** Do not mark complete until observed.
