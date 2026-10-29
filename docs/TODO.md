# Color Kinetics ESP32-S3 implementation TODO

Mark complete only after acceptance evidence is available.

## Initial repository
- [x] Import pure Chromasic framing, RMT/DMA transmitter and safe default.
- [x] Configure ESP-IDF v6.1 for ESP32-S3 N16R8 (16 MB flash).
- [x] Implement strict C host tests for start, idle, LSB-first channels,
      last node, and six flush nodes.
- [x] Document protocol assumptions, electrical safety and unknown pinout.
- [x] Configure pinned ESP-IDF GitHub Actions build and artifact.
- [ ] Confirm exact-commit CI outcome and record evidence in docs/PROGRESS.md.

## Physical hardware qualification
- [ ] Identify all conductor assignments and connector pinout.
- [ ] Confirm actual differential/single-ended levels and driver polarity.
- [ ] Design verified level shifter or differential driver plus fused 24 V supply.
- [ ] Measure 2.2 us waveform and continuous idle, detect gaps and underruns.
- [ ] Demonstrate controlled low-brightness output on all 60 nodes.
- [ ] Compare measured response to reverse-engineered assumptions.

## Firmware hardening
- [ ] Add explicit Kconfig output option (still disabled by default).
- [ ] Implement RMT underrun/recovery metrics and Wi-Fi stress tests.
- [ ] Refactor a reusable Chromasic component/API and make timing configurable.
- [ ] Harden frame atomicity, node count, gamma, brightness range and tests.

## UI (after proven physical interface)
- [ ] Add Wi-Fi provisioning, local web UI, global brightness and power.
- [ ] Add individual node control, effects, presets and animation speed.
- [ ] Consider WLED/Home Assistant integration.

## Wiring and white-channel qualification
- [x] Add editable proposed differential wiring diagram and safety verification notes.
- [ ] Measure actual 5-foot harness continuity, converter and signal polarity.
- [ ] Confirm exact differential transmitter module and physical connector cavity mapping.
- [ ] Validate whether the eW Compact nodes require two active white channels.

## Protocol signal documentation
- [x] Document the transmitted RMT bit cells, framing, idle timing and proposed differential signal path with editable SVG.
- [ ] Verify GPIO timing and uninterrupted output on a real ESP32-S3 with a logic analyzer.
- [ ] Compare actual eW Flex HE electrical waveform and white channel behavior with published iColor Flex observations.

## Procurement and BOM
- [x] Document costed preliminary BOM, existing hardware, reference suppliers, and optional equipment.
- [ ] Verify original leader's receiver and connector before committing to wiring-specific purchases.
- [ ] Confirm actual transmitter's termination, 3.3 V DI logic compatibility and correct A/B labeling.
- [ ] Select fuse and wire gauge from actual startup/load measurements and harness ratings.
- [ ] Reconcile actual purchase prices, shipping and final as-built parts against the BOM.
