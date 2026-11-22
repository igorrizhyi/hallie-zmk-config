/*
 * Copyright 2022 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
/* Handedness. */
#define MASTER_LEFT

// To use the handedness pin, resistors need to be installed on the adapter PCB.
// If so, uncomment the following code, and undefine MASTER_RIGHT above.
// #define SPLIT_HAND_PIN GP15
// #define SPLIT_HAND_PIN_LOW_IS_LEFT  // High -> right, Low -> left.

/* Reset. */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U

#undef  DIODE_DIRECTION
#define DIODE_DIRECTION COL2ROW
#define USB_VBUS_PIN GP19

#define MK_KINETIC_SPEED

#define MOUSEKEY_INTERVAL 7
// #define MOUSEKEY_MAX_SPEED 8
#define MOUSEKEY_INITIAL_SPEED 400
#define MOUSEKEY_MOVE_DELTA 22
#define MOUSEKEY_BASE_SPEED 3000

#define MOUSEKEY_WHEEL_INTERVAL 120
#define MOUSEKEY_WHEEL_DELTA 0.1
#define MOUSEKEY_WHEEL_MAX_SPEED 12
#define MOUSEKEY_WHEEL_TIME_TO_MAX 800
#define MOUSEKEY_WHEEL_INITIAL_MOVEMENTS 12
#define MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS 6

// #define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX
// #define UNICODE_SELECTED_MODES UC_LINX

// #define COMBO_TERM 120
