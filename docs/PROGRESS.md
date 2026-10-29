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

## Proposed wiring documentation — 2026-10-28
- Added docs/WIRING_DIAGRAM.svg and docs/WIRING.md: MAX485-style TX, 24V power supply, 5V buck and leader colors. Nothing has been electrically connected or hardware-qualified.
- Sources: Color Kinetics eW Flex Compact Product Guide and Glen Akins' leader reverse engineering. Connector cavity numbering/polarity remains unverified for this custom 5-foot harness.
- Earlier exact-head CI success observed at commit 38f8bd81f36862c2e4a9ce60ca4acc0a016d1878 (run 40825204684). Current commit needs independent CI verification.
- Firmware follow-up: product guide says first and second white channels can drive separate LED groups; prototype only sets first channel.

## Protocol reference diagram — 2026-10-28
- Added docs/CHROMASIC_SIGNALS.md and docs/CHROMASIC_SIGNAL_DIAGRAM.svg to explain bit pulse widths, 21-bit start code, three 12-bit LSB-first channels, 61-bit nodes, six flush nodes, and frame cadence.
- Correctly distinguishes Glen's measured/reverse-engineered iColor Flex characteristics from the 6,000-zero idle count and unverified eW Flex mapping in our code.
- Safe firmware remains unchanged (GPIO output disabled); no physical capture has been performed.

## Preliminary hardware BOM — 2026-10-28
- Added docs/BOM.md with dated US-dollar component references (MAX485 $1.09, 24-to-5 V converter $7.00, 24 V/4 A external adapter $27.41, inline 3 A fuse holder $5.99) and non-quoted wiring/connector allowances.
- Incremental bench-prototype budget $49.49–$58.49, excluding existing ESP32/LED string, taxes, postage, tools and optional enclosure. These are estimates, not purchases.
- Procurement is blocked pending electrical identification of custom leader and existing receiver. Fuse selection and line termination remain unverified.
- Previous exact-commit firmware CI was green at 2960be415f7d6b660bd413ca0844f8f06880cdbd, GitHub Actions run 40827308809. This new documentation-only commit needs its own CI observation.

## Developer instructions — 2026-10-28
- Documented fresh Ubuntu 22.04/24.04 pinned-SDK setup, exact toolchain checks, lint, tests, build artifacts, safe flash, serial monitor, troubleshooting and 16MB N16R8 configuration in README.md.
- Added tests/run_lint.sh (Bash syntax, ShellCheck, C11 with warnings-as-errors, Git whitespace) and corresponding GitHub Actions step.
- Local host C compiler syntax and protocol tests passed; ShellCheck was unavailable locally and must be confirmed by exact-commit CI.
- Previous exact-commit CI passed at 11d11570110bbed3220ecef306a788791fbdb7d6 (run 40827838576). This commit requires independent CI confirmation.
- No physical ESP32-S3 flashing or serial monitor verification has been performed.
