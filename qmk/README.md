# QMK keymap — TBK Mini

The Hallie layout ported to the wired TBK Mini, kept here so GitHub Actions
can build it without a local toolchain.

- Source: `qmk/keymaps/hallie/`
- Built by `.github/workflows/qmk.yml` against QMK `0.22.14`
- Target: `bastardkb/tbkmini/v2/splinky_3`
- Artifact: **tbkmini-hallie-firmware** on the workflow run

Same 42 keys in the same positions as the Hallie. Hardware differences:
no Bluetooth (the settings layer holds bootloader/reset instead of BT
profiles), no encoders, and the mouse speed boost uses QMK's own
acceleration keycode.

To flash: double-tap the reset button, then copy the `.uf2` onto the
drive that appears.
