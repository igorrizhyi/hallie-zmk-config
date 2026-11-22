#pragma once

#define TAPPING_TERM 200
#define HOLD_ON_OTHER_KEY_PRESS   // matches the Hallie's hold-preferred flavour
#define RETRO_TAPPING
#define QUICK_TAP_TERM 200

// Keystrokes sent from code are held a little longer than QMK's default so
// remote desktop sessions register them reliably.
#define TAP_CODE_DELAY 30

// Mouse movement tuned to the Hallie: 1200 units/s top speed reached over
// roughly 300 ms, reported every 16 ms.
#define MOUSEKEY_DELAY          0
#define MOUSEKEY_INTERVAL       16
#define MOUSEKEY_MAX_SPEED      19
#define MOUSEKEY_TIME_TO_MAX    19

// Scroll is deliberately slower and unaccelerated.
#define MOUSEKEY_WHEEL_DELAY        0
#define MOUSEKEY_WHEEL_INTERVAL     120
#define MOUSEKEY_WHEEL_MAX_SPEED    1
#define MOUSEKEY_WHEEL_TIME_TO_MAX  0
