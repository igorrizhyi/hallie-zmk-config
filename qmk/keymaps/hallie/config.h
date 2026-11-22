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

// Matched to the Hallie's pointer. ZMK moves at ZMK_POINTING_DEFAULT_MOVE_VAL
// = 1200 units/s; QMK's accelerated mode tops out at
//     MOUSEKEY_MOVE_DELTA * mk_max_speed   units per report
// clamped to 127, one report every MOUSEKEY_INTERVAL ms. So:
//     1 * 24 units / 16 ms = 1500 units/s
// which is the Hallie's speed plus the small bias Igor asked for.
//
// MOVE_DELTA is deliberately 1 rather than QMK's default 8: with 8 the only
// way to reach 24 is max_speed 3, which gives a coarse three-step ramp and a
// 500 units/s floor. At 1 the ramp has 24 steps and starts at 62 units/s,
// which is much closer to ZMK's exponent-1 curve starting near zero.
//
// The previous values (DELTA 8 defaulted in, max_speed 19) computed to 152,
// clamped to 127 = 7937 units/s: 6.6x the Hallie, and the clamp made every
// max_speed >= 16 behave identically.
#define MOUSEKEY_DELAY          0
#define MOUSEKEY_INTERVAL       16      // 62.5 reports/s
#define MOUSEKEY_MOVE_DELTA     1
#define MOUSEKEY_MAX_SPEED      24      // -> 1500 units/s
#define MOUSEKEY_TIME_TO_MAX    16      // 16 reports = 256 ms ramp

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
