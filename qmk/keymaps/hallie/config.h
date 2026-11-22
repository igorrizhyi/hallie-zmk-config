#pragma once

#define TAPPING_TERM 200
#define HOLD_ON_OTHER_KEY_PRESS   // matches the Hallie's hold-preferred flavour
#define RETRO_TAPPING
#define QUICK_TAP_TERM 200

// Keystrokes sent from code are held a little longer than QMK's default so
// remote desktop sessions register them reliably.
#define TAP_CODE_DELAY 30

// The splinky_3 board config.h sets up kinetic mouse mode with its own
// tuning. A keymap config.h is included after the keyboard's, so drop
// those first: the values below assume QMK's accelerated mode, and
// redefining them without #undef is a warning at best and the board's
// kinetic curve at worst. (The legacy keymap ships no config.h and so
// deliberately keeps the board's original feel.)
#undef MK_KINETIC_SPEED
#undef MOUSEKEY_INITIAL_SPEED
#undef MOUSEKEY_BASE_SPEED
#undef MOUSEKEY_MOVE_DELTA
#undef MOUSEKEY_DELAY
#undef MOUSEKEY_INTERVAL
#undef MOUSEKEY_MAX_SPEED
#undef MOUSEKEY_TIME_TO_MAX

// Mouse movement tuned to the Hallie: 1200 units/s top speed reached over
// roughly 300 ms, reported every 16 ms.
#define MOUSEKEY_DELAY          0
#define MOUSEKEY_INTERVAL       16
#define MOUSEKEY_MAX_SPEED      19
#define MOUSEKEY_TIME_TO_MAX    19

#undef MOUSEKEY_WHEEL_DELAY
#undef MOUSEKEY_WHEEL_INTERVAL
#undef MOUSEKEY_WHEEL_DELTA
#undef MOUSEKEY_WHEEL_MAX_SPEED
#undef MOUSEKEY_WHEEL_TIME_TO_MAX
#undef MOUSEKEY_WHEEL_INITIAL_MOVEMENTS
#undef MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS

// Scroll is deliberately slower and unaccelerated.
#define MOUSEKEY_WHEEL_DELAY        0
#define MOUSEKEY_WHEEL_INTERVAL     120
#define MOUSEKEY_WHEEL_MAX_SPEED    1
#define MOUSEKEY_WHEEL_TIME_TO_MAX  0
