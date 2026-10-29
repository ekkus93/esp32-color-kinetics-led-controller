# Bill of Materials (BOM) — ESP32-S3 Color Kinetics controller

**Revision:** 2026-10-28 · **Currency:** USD · **Status:** budgetary / provisional (NOT an approved purchase list)

This BOM covers the experimental **60-node Color Kinetics eW Flex Compact HE (4000 K, 24 V, 2.5 A maximum)** string and **ESP32-S3 N16R8** using the **proposed four-conductor differential leader interface**. The ESP32 generates raw Chromasic pulse timing, and a low-cost MAX485-type electrical transmitter converts it to a differential signal. It **replaces the proprietary $200+ Color Kinetics power/data controller**; no Color Kinetics controller is budgeted.

**Critical pre-purchase hold:** the exact five-foot leader's connector positions, differential receiver/converter, red/black power mapping, and blue/blue-white data polarity have **not** been electrically established for the user's harness. Do not purchase additional connector hardware or connect a powered strand until the [wiring qualification checklist](WIRING.md) is satisfied. In particular, a raw three-wire node bus cannot be attached to the A/B differential outputs shown here.

## A. Already owned — $0 incremental

| Ref | Item / specification | Qty | Additional cost | Notes |
| --- | --- | ---: | ---: | --- |
| OWN-01 | ESP32-S3 N16R8 development board (16 MB flash, 8 MB PSRAM) | 1 | **$0** | Owned. Powered independently over USB for bench tests; GPIO18 is the experimental data output. |
| OWN-02 | Color Kinetics eW Flex Compact HE LED string, 60 white 4000 K nodes, item 999-009302-44 | 1 | **$0** | Owned. Label specifies 24 VDC and 2.5 A maximum. |
| OWN-03 | Original 5-foot leader/connector (provided with string, if complete) | 1 | **$0** | Presence of a compatible differential receiver at the LED end must be verified. Do not invent its connector pinout. |
| OWN-04 | USB cable and 5 V USB port for ESP32 | 1 | **$0 assumed** | Reuse an existing suitable USB cable and power source. |

A replacement ESP32-S3 dev board would be a separate discretionary expense; this budget **does not** include buying a second board or another LED string.

## B. Proposed additional parts — bench prototype

Prices were checked **2026-10-28** from linked examples. Quoted unit prices may change; tax, shipping, tariffs, and tools are **not included**.

| Ref | Part / purpose | Qty | Example price each | Budget used | Example purchasing reference / caveat |
| --- | --- | ---: | ---: | ---: | --- |
| TX-01 | **MAX485 5 V TTL-to-RS-485 transmitter module**, with accessible `DI`, `DE`, `/RE`, `VCC`, `GND`, `A`, `B` | 1 | **$1.09** | **$1.09** | [ProtoSupplies MOD-61](https://protosupplies.com/product/max485-ttl-to-rs-485-interface-module/) — 5 V, 2.5 Mbit/s. **Confirm the chip/module A/B conventions and whether its fitted 120 Ω resistor must be removed**; no UART transceiver board without direct DI access. |
| DC-01 | **24 V to regulated 5 V buck converter**, ≥0.5 A output; powers TX-01 only | 1 | **$7.00** | **$7.00** | [DROK 090581 fixed 5 V, 5 A](https://www.droking.com/DC-Step-Down-Voltage-Regulator-DC-12-24V-to-5V-5A-25W-Buck-Converter-Module-Waterproof-Car-Power-Supply) — 9–35 V input, over-specced for the transmitter but available. **No reverse-polarity protection**. |
| PS-01 | **24 V regulated 4 A (96 W) external desktop DC adapter**, approved for local mains | 1 | **$27.41** | **$27.41** | [DigiKey Supirit ZF120A-2404000](https://www.digikey.com/en/products/detail/supirit-inc/ZF120A-2404000/22535300), cULus-listed desktop type, **5.5 mm OD / 2.5 mm ID center-positive barrel**, AC cord according to listing. Enclosed mains side is preferred to exposed terminal AC supplies. Verify exact order code and plug. |
| F-01 | **DC-rated inline fuse holder plus fuse**, provisional 3 A blade type | 1 | **$5.99** | **$5.99** | [Neptunian Sky 18 AWG holder with 3 A fuse](https://www.neptunianskyllc.com/product-page/inline-fuse-holder-w-included-3-amp-blade-fuse). Final fuse value must respect actual cable ampacity, startup current and equipment documentation; **3 A is a budgeting example, not an approved protection design**. Place protection close to DC PSU positive output. |
| CN-01 | **Mating 5.5×2.5 mm female barrel/pigtail, terminal adapter**, for PS-01 | 1 | **$3–$7 estimate** | **$3–$7** | Generic, **not price-verified**; the mating connector must match the exact adapter. Do not assume 5.5×2.1 mm fits. |
| HW-01 | **Hookup wire, wire ferrules / terminals, insulating hardware and small mounting board** | 1 lot | **$5–$10 estimate** | **$5–$10** | Generic consumables, **not price-verified**. Use appropriate conductor gauge for the 24 V LED feed and secure connections; avoid solderless breadboard for the LED power line. |

### Budget totals

| Scenario | Cost (USD) |
| --- | ---: |
| Price-verified TX-01 + DC-01 + PS-01 + F-01 | **$41.49** |
| Connector/pigtail and wiring allowance | **+$8–$17** |
| **Additional parts for bench prototype** | **$49.49–$58.49** |
| Already-owned LED string and ESP32 | **+$0 incremental** |
| Optional project enclosure (estimate) | **+$6–$15** |
| **Including optional enclosure** | **$55.49–$73.49** |

The budget **does not include** a proprietary Color Kinetics controller, additional LED hardware, taxes, postage, tariffs, labor, or diagnostic equipment. Shipping from several retailers can materially exceed the $1.09 transmitter cost. Figures are **single-quantity examples, not quotes or guaranteed stock**.

## C. Optional / bench diagnostic equipment (not included in base total)

| Item | Planning allowance | Why / status |
| --- | ---: | --- |
| Digital multimeter | $10–$25 **estimate** if not owned | Continuity, power conductor identification, confirm output voltage and polarity. |
| 24 MHz or faster digital logic analyzer (3.3 V compatible) | $10–$30 **estimate** if not owned | Capture GPIO18's nominal 2.2 µs cells and framing. Differential A/B must be probed with correctly rated equipment / suitable differential receiver; do not overvoltage an analyzer. |
| Oscilloscope / differential-probe equipment | Existing lab equipment or rental | Confirm A/B polarity, edge timing, undershoot, voltage limits and DMA gaps; no ungrounded/high-side probing assumptions. |
| Small insulated enclosure / strain relief | $6–$15 **estimate** | Protects breadboard-free permanent wiring, power distribution and terminals. |
| Spare MAX485 module | $1–$5 **estimate** | Useful only after differential leader is confirmed. |
| TVS / additional DC protection | TBD | Specify after cable topology, termination, transients, ground and receiver have been examined. |

## D. Electrical selection criteria and hold points

1. **Power budget:** LED label 24 V × 2.5 A = **60 W maximum nominal**. A 24 V × 4 A = **96 W** external supply provides headroom. The 5 V buck draws from the 24 V supply as a small auxiliary load. Do not confuse supply current rating with current forced into LEDs.
2. **Ground / power:** ESP32 powered via USB; GPIO18 is 3.3 V logic and feeds only MAX485 `DI`. Share a **verified** DC ground with the 5 V transmitter / 24 V negative reference. Never feed 24 V to the ESP32.
3. **MAX485 electrical layer:** A genuine 5 V MAX485 accepts TTL-like 3.3 V HIGH and its cited module is rated above the 454.5 kbit/s Chromasic pulse rate. It **does not interpret Chromasic**, and no UART, Modbus, CAN or DMX framing is used. Set `DE` HIGH, `/RE` HIGH, `RO` unconnected.
4. **Termination:** ProtoSupplies documents a **120 Ω termination resistor on the module**. Inspect the original receiver and total pair termination; do **not** automatically add or retain another 120 Ω across an already terminated pair.
5. **Leader preflight:** Identify connector and all four conductors by measurement; confirm whether the LED-side differential receiver/converter actually exists. Confirm power polarity and signal pair orientation before buying any custom connectors or energizing the LEDs.
6. **Firmware preflight:** The repo ships `CK_OUTPUT_ENABLED=0` by default. Scope the experimental RMT waveform with the LEDs disconnected before attempting a low-brightness node test.

## E. Revision / procurement status

- **2026-10-28:** Created initial costed BOM from supplier listings and explicit estimates. **None of the proposed parts have been purchased or electrically qualified by this project.** Ownership assumed only for the user's ESP32-S3, LED string and the previously photographed leader.
- **Hold:** approve all physical selections only after verifying the original leader's active/passive interface and original four-wire power/data mapping.
- If the LED-side receiver/converter is absent, this BOM and wiring topology must be revised; do not simply wire differential A/B to bare node data.

Related documents: [WIRING.md](WIRING.md) · [WIRING_DIAGRAM.svg](WIRING_DIAGRAM.svg) · [CHROMASIC_SIGNALS.md](CHROMASIC_SIGNALS.md) · [TODO.md](TODO.md).
