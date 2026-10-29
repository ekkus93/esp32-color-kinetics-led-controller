# ESP32-S3 Color Kinetics LED Controller

Experimental, **safe-by-default** Chromasic transmitter for a **Color Kinetics eW Flex Compact HE** 60-node fixed-white 4000 K string (item 999-009302-44; 24 VDC, 2.5 A max). Built using **ESP-IDF v6.1** for **ESP32-S3 N16R8** (16 MB flash, 8 MB PSRAM). PSRAM is not currently used.

> **SAFETY:** Default firmware does **not** transmit: `CK_OUTPUT_ENABLED` is `0` in `main/main.c`. The four-wire connector pinout, differential receiver and actual LED response are unverified. **Never connect 24 V to an ESP32 pin or GPIO18 directly to an unidentified leader.** Read [WIRING.md](docs/WIRING.md) before touching the LED power wiring.

## Ubuntu development setup (22.04 or 24.04)

Commands are for **Bash** and the repository root unless otherwise stated. Arduino and PlatformIO are not required. This project uses the exact ESP-IDF revision in [GitHub Actions](.github/workflows/firmware.yml).

### 1. Install Ubuntu prerequisites

```bash
sudo apt update
sudo apt install -y \
  git wget flex bison gperf python3 python3-pip python3-venv \
  cmake ninja-build ccache libffi-dev libssl-dev \
  dfu-util libusb-1.0-0 gcc shellcheck
```

ESP-IDF v6.1 requires Python 3.10 or newer. The installer below installs the **Xtensa ESP32-S3 cross-compiler** and its own Python environment, normally under `~/.espressif/`. The system `gcc` runs host tests; it is not the firmware compiler.

### 2. Install the pinned ESP-IDF v6.1 SDK (first time only)

These commands match the SDK checkout used by CI, avoiding the ambiguity of a floating `latest` branch.

```bash
export IDF_DIR="$HOME/esp/esp-idf-v6.1"
export IDF_SHA=fff9895c82d744c7237be8847347bdd1b07c6643

mkdir -p "$IDF_DIR"
git -C "$IDF_DIR" init
git -C "$IDF_DIR" remote add origin https://github.com/espressif/esp-idf.git
git -C "$IDF_DIR" fetch --depth 1 origin "$IDF_SHA"
git -C "$IDF_DIR" checkout --detach "$IDF_SHA"
git -C "$IDF_DIR" submodule update --init --recursive --depth 1

"$IDF_DIR/install.sh" esp32s3
source "$IDF_DIR/export.sh"

idf.py --version
git -C "$IDF_DIR" rev-parse HEAD
xtensa-esp-elf-gcc --version | head -n 1
```

**Verify:** The output of `git -C "$IDF_DIR" rev-parse HEAD` must exactly match `fff9895c82d744c7237be8847347bdd1b07c6643`. The previously verified cross-compiler reports Xtensa GCC 15.2.0. Allow disk space and internet downloads for SDK submodules and tools. Run this installation sequence only for a **new checkout**; if your SDK already exists, do not re-run `git remote add`.

**Every new Bash terminal** needs ESP-IDF activation:

```bash
export IDF_DIR="$HOME/esp/esp-idf-v6.1"
source "$IDF_DIR/export.sh"
```

Do not commit `~/.espressif/`, `sdkconfig` or build artifacts.

### 3. Clone this project

```bash
git clone https://github.com/ekkus93/esp32-color-kinetics-led-controller.git
cd esp32-color-kinetics-led-controller
```

### 4. Lint and check source files

```bash
bash tests/run_lint.sh
```

The script runs **Bash syntax checks**, **ShellCheck**, strict hardware-independent **C11 syntax/compile checks** (`-Wall -Wextra -Werror -pedantic`), and **Git whitespace checks**. It requires `shellcheck` from Step 1. Firmware-only code in `main/main.c` depends on ESP-IDF headers and is instead validated by the real cross-compiler in Step 6.

You can also run the checks separately:

```bash
bash -n tests/run_lint.sh tests/run_host_tests.sh
shellcheck --severity=warning tests/run_lint.sh tests/run_host_tests.sh
cc -std=c11 -Wall -Wextra -Werror -pedantic -fsyntax-only -I main \
  main/chromasic_protocol.c tests/test_chromasic.c
git diff --check
```

### 5. Run unit tests without hardware

```bash
bash tests/run_host_tests.sh
```

Expected output:

```text
PASS: idle, start, LSB-first channels, last node, six flush nodes
```

This proves only host-side frame encoding behavior; it does **not** validate real signal quality, actual LED wiring or driver timing. Host lint and tests can run even before installing ESP-IDF.

### 6. Configure and compile the ESP32-S3 firmware

After running `source "$IDF_DIR/export.sh"`, from the project root:

```bash
idf.py set-target esp32s3
idf.py build
idf.py size
```

The committed `sdkconfig.defaults` selects **16 MB flash**. `set-target` initializes the generated `sdkconfig` and may clear a previous configuration, so run it at first setup or when switching targets. For normal incremental builds use `idf.py build` only.

| Generated artifact | Path |
| --- | --- |
| Application binary | `build/chromasic_esp32s3.bin` |
| Bootloader | `build/bootloader/bootloader.bin` |
| Partition table | `build/partition_table/partition-table.bin` |
| ELF/debug symbols | `build/chromasic_esp32s3.elf` |

**Important:** The application binary is **not** a complete combined flash image. Never write `build/chromasic_esp32s3.bin` at `0x0`; use `idf.py flash` to program all required images at the correct offsets.

For a clean build after SDK/configuration changes:

```bash
idf.py fullclean
idf.py set-target esp32s3
idf.py build
```

### 7. Identify the board's USB serial port

Connect the ESP32-S3 via a **USB data cable** (no LED power supply or leader attached yet). Find its port:

```bash
ls -l /dev/serial/by-id/ 2>/dev/null || true
python -m serial.tools.list_ports
```

On Ubuntu it may be `/dev/ttyACM0` or `/dev/ttyUSB0`. Substitute the device discovered on your machine in the commands below; a stable `/dev/serial/by-id/...` symlink is also suitable.

If you get `Permission denied`, add yourself to `dialout` and **log out and back in** (or reboot) before retrying:

```bash
sudo usermod -aG dialout "$USER"
```

Avoid `sudo idf.py`.

### 8. Flash the **safe** firmware

With the intended ESP32 connected and the LED-side hardware **disconnected**:

```bash
idf.py -p /dev/ttyACM0 flash
```

This builds if necessary and writes bootloader, partition table and app. **Flashing overwrites any existing firmware on the selected board.** Check the port carefully.

Flash and start the serial monitor in one command:

```bash
idf.py -p /dev/ttyACM0 flash monitor
```

If bootloader auto-entry fails, hold the board's **BOOT** button, tap **RESET**, release **BOOT**, and retry. Labels vary by hardware.

### 9. Monitor logs and stop the monitor

```bash
idf.py -p /dev/ttyACM0 monitor
```

The default firmware is expected to report:

```text
SAFE MODE: GPIO output disabled.
Verify connector voltage, pinout and line driver first.
```

Quit IDF Monitor with **Ctrl+]**. Close serial monitors before flashing again.

### 10. Experimental GPIO output (hardware-qualified testing only)

Only **after** confirming the leader connector, verifying the differential driver/receiver and measuring the disconnected waveform, `main/main.c` permits changing:

```c
#define CK_OUTPUT_ENABLED 1
```

Then repeat `idf.py build` and `idf.py -p /dev/ttyACM0 flash monitor`. It requests a Chromasic waveform on **GPIO18** and a 60-node dim chasing pattern. **Do not enable transmission just to discover whether a guessed connection works.** No uninterrupted-RMT or physical-LED acceptance evidence exists yet.

## CI, troubleshooting and project files

GitHub Actions uses **Ubuntu 24.04** to run the same `tests/run_lint.sh` and `tests/run_host_tests.sh`, then installs the pinned ESP-IDF revision, builds the default **safe** firmware, and publishes the bootloader, partition table and app binaries. Get them from the latest successful run of [ESP32-S3 firmware](https://github.com/ekkus93/esp32-color-kinetics-led-controller/actions) under the **esp32s3-safe-firmware** artifact.

| Problem | Resolution |
| --- | --- |
| `idf.py: command not found` | In the current terminal, run `source "$HOME/esp/esp-idf-v6.1/export.sh"`. |
| Wrong SDK/compiler | Compare SDK HEAD with the pinned SHA above; run `install.sh esp32s3` for missing tools. |
| Wrong ESP32 target / flash size | Run `idf.py set-target esp32s3`; ensure generated `sdkconfig` has `CONFIG_ESPTOOLPY_FLASHSIZE_16MB=y`. |
| `/dev/ttyACM0` missing | Check data cable and `python -m serial.tools.list_ports`. |
| USB permission denied / port busy | Re-login after joining `dialout`; close other serial terminals. |
| Old cache after changing SDK | `idf.py fullclean`, then `idf.py set-target esp32s3`, then `idf.py build`. |
| LEDs remain off | **Expected** for safe firmware. Do not bypass cable and waveform validation. |

References: [wiring SVG](docs/WIRING_DIAGRAM.svg) · [wiring notes](docs/WIRING.md) · [signal SVG](docs/CHROMASIC_SIGNAL_DIAGRAM.svg) · [protocol notes](docs/CHROMASIC_SIGNALS.md) · [BOM and costs](docs/BOM.md) · [specification](docs/SPEC.md) · [TODO](docs/TODO.md) · [progress](docs/PROGRESS.md).

Source: `main/main.c` (RMT transmit); `main/chromasic_protocol.[ch]` (frame encoding); `tests/` (host verification).

Protocol research: [Glen Akins, reverse engineering iColor Flex](https://bikerglen.com/blog/reverse-engineering-the-icolor-flex-protocol/). Official instructions: [ESP-IDF v6.1 ESP32-S3 quick start](https://docs.espressif.com/projects/esp-idf/en/release-v6.1/esp32s3/get-started/linux-macos-start-project.html).

Independent experimental research; not an official or endorsed Color Kinetics/Signify controller. [Apache-2.0 license](LICENSE).
