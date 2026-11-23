// TBK Mini keymap mirroring the Hallie ZMK layout.
// Build: qmk compile -kb bastardkb/tbkmini -km hallie
//
// Same 42 keys in the same order as the Hallie, so muscle memory carries
// over between the two boards. Differences are only where the hardware
// forces them: no Bluetooth (wired), no encoders, and the settings layer
// holds QMK's bootloader/reset instead of BT profiles.

#include QMK_KEYBOARD_H
#include "mousekey.h"   // mk_max_speed, for the R3 cursor boost

enum layers {
    BASE, UA, SYM, UASYM, NAV, UANAV, NUM, UANUM, DESK, CTL, ALTL, MOUSE, SET
};

enum custom_keycodes {
    TO_UKR = SAFE_RANGE,  // Ctrl+Shift+3 + switch to the UA layer
    TO_ENG,               // Ctrl+Shift+2 + switch to the base layer
    SWAP_CT,              // Ctrl+Tab, Ctrl held across presses
    SWAP_CST,             // Ctrl+Shift+Tab, Ctrl held across presses
    SWAP_AT,              // Alt+Tab, Alt held across presses
    DOT_COM,              // types . ; with shift held types , instead
    MOU_SMT,              // hold = mouse layer, tap = toggle mouse layer
    MOU_FST,              // held: cursor at 2x, mirroring the Hallie FAST layer
};

// Cursor speeds, in "units per report" (see config.h for the arithmetic).
// The Hallie boosts with &zip_xy_scaler 2 1 on its FAST layer, i.e. exactly
// x2, so the boost here doubles mk_max_speed rather than using KC_ACL2.
// KC_ACL2 computes MOUSEKEY_MOVE_DELTA * mk_max_speed, which IS the sustained
// top speed, so it gave no boost at all -- only an instant jump to max.
#define MOU_SPEED_BASE 24        // 1500 units/s
#define MOU_SPEED_FAST 48        // 3000 units/s

// MOU_SPEED_BASE restores the resting speed, so it has to stay equal to the
// value config.h compiled in; catch any future drift at build time.
_Static_assert(MOU_SPEED_BASE == MOUSEKEY_MAX_SPEED,
               "MOU_SPEED_BASE must match MOUSEKEY_MAX_SPEED in config.h");

#define CAD LCTL(LALT(KC_DEL))   // local machine security screen
#define CAE LCTL(LALT(KC_END))   // the same screen inside an RDP session

// ---------------------------------------------------------------------
// Swappers
//
// The modifier goes down on the first press and stays down across
// repeats so the Windows switcher stays open; it is released when the
// layer that hosts the key is left. Mirrors the tri-state behaviour used
// on the Hallie, including the pacing that keeps remote desktop sessions
// from seeing Tab before the modifier.
// ---------------------------------------------------------------------
// The modifier leads Tab by 90 ms so a laggy host cannot see Tab first.
// Shortening this does NOT hide the Windows switcher: it paints on the Tab
// press itself, not after a delay (tested at 15 ms, still blinked). The
// silent flip is KC_F18 on the DESK home row instead.
#define SWAP_LEAD_RDP_MS  90
#define SWAP_TAB_RDP_MS   30

static bool ctl_swap_active = false;
static bool alt_swap_active = false;

static void swap_step(bool *active, uint16_t mod, bool shifted,
                      uint16_t lead_ms, uint16_t tab_ms) {
    if (!*active) {
        register_code(mod);
        *active = true;
        wait_ms(lead_ms);        // let a laggy host register the modifier
    }
    // Spelled out rather than tap_code() so the Tab hold is per-call;
    // tap_code() would always use the global TAP_CODE_DELAY of 30 ms.
    if (shifted) register_code(KC_LSFT);
    register_code(KC_TAB);
    wait_ms(tab_ms);
    unregister_code(KC_TAB);
    if (shifted) unregister_code(KC_LSFT);
}

static void swap_end(bool *active, uint16_t mod) {
    if (*active) {
        unregister_code(mod);
        *active = false;
    }
}

// ---------------------------------------------------------------------
// Language switching
//
// Absolute, not a toggle: each key always lands on its language. Needs
// the Windows hotkeys under Advanced keyboard settings -> Input language
// hot keys: English = Ctrl+Shift+2, Ukrainian = Ctrl+Shift+3. Paced the
// same way as the Hallie's macros so remote sessions keep up.
// ---------------------------------------------------------------------
static void send_lang(uint16_t digit) {
    register_code(KC_LCTL);
    wait_ms(40);
    register_code(KC_LSFT);
    wait_ms(40);
    tap_code(digit);
    wait_ms(40);
    unregister_code(KC_LSFT);
    unregister_code(KC_LCTL);
}

// ---------------------------------------------------------------------
// Mouse layer thumb: hold for momentary, tap to toggle
// ---------------------------------------------------------------------
static uint16_t mou_timer   = 0;
static bool     mou_toggled = false;

// ---------------------------------------------------------------------
// easy-jump signalling
//
// F23 when the mouse layer opens, F24 when it closes. Doing this from
// the layer hook covers the hold and the toggle path alike, with no
// macro gymnastics — on the Hallie this needed three separate macros.
// ---------------------------------------------------------------------
layer_state_t layer_state_set_user(layer_state_t state) {
    static bool mouse_was_on = false;
    bool mouse_is_on = layer_state_cmp(state, MOUSE);

    if (mouse_is_on != mouse_was_on) {
        tap_code(mouse_is_on ? KC_F23 : KC_F24);
        mouse_was_on = mouse_is_on;
    }

    // R3 can still be down when the layer is released, which would leave
    // the pointer stuck at 2x on the next activation.
    if (!mouse_is_on) {
        mk_max_speed = MOU_SPEED_BASE;
    }

    // Leaving a swapper's layer commits the selection.
    if (!layer_state_cmp(state, NAV) && !layer_state_cmp(state, UANAV)) {
        swap_end(&ctl_swap_active, KC_LCTL);
    }
    if (!layer_state_cmp(state, DESK)) {
        swap_end(&alt_swap_active, KC_LALT);
    }

    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MOU_FST:
            mk_max_speed = record->event.pressed ? MOU_SPEED_FAST : MOU_SPEED_BASE;
            return false;

        case TO_UKR:
            if (record->event.pressed) {
                send_lang(KC_3);
                layer_move(UA);
            }
            return false;

        case TO_ENG:
            if (record->event.pressed) {
                send_lang(KC_2);
                layer_move(BASE);
            }
            return false;

        case SWAP_CT:
            if (record->event.pressed) swap_step(&ctl_swap_active, KC_LCTL, false,
                                                 SWAP_LEAD_RDP_MS, SWAP_TAB_RDP_MS);
            return false;

        case SWAP_CST:
            if (record->event.pressed) swap_step(&ctl_swap_active, KC_LCTL, true,
                                                 SWAP_LEAD_RDP_MS, SWAP_TAB_RDP_MS);
            return false;

        case SWAP_AT:
            if (record->event.pressed) swap_step(&alt_swap_active, KC_LALT, false,
                                                 SWAP_LEAD_RDP_MS, SWAP_TAB_RDP_MS);
            return false;

        case DOT_COM:
            if (record->event.pressed) {
                uint8_t mods = get_mods() | get_oneshot_mods();
                if (mods & MOD_MASK_SHIFT) {
                    uint8_t held = get_mods();
                    del_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    tap_code(KC_COMM);
                    set_mods(held);
                } else {
                    tap_code(KC_DOT);
                }
            }
            return false;

        case MOU_SMT:
            if (record->event.pressed) {
                mou_timer = timer_read();
                if (!mou_toggled) layer_on(MOUSE);
            } else {
                if (timer_elapsed(mou_timer) < TAPPING_TERM) {
                    if (mou_toggled) {          // second tap: leave the layer
                        layer_off(MOUSE);
                        mou_toggled = false;
                    } else {                    // first tap: stay on the layer
                        mou_toggled = true;
                    }
                } else if (!mou_toggled) {      // held: momentary
                    layer_off(MOUSE);
                }
            }
            return false;
    }
    return true;
}

// Settings layer: hold both tall inner thumbs together, same as the Hallie.
const uint16_t PROGMEM settings_combo[] = {MOU_SMT, LT(CTL, KC_ENT), COMBO_END};
combo_t key_combos[] = {
    COMBO(settings_combo, MO(SET)),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [BASE] = LAYOUT_split_3x6_3(
        KC_UP,    OSM(MOD_LCTL), KC_A,  KC_QUOT, KC_P,  KC_Y,       KC_F,  KC_K,  KC_G,  KC_R,  KC_CAPS, KC_TAB,
        KC_LSFT,  MO(NAV),       KC_O,  KC_E,    KC_I,  KC_U,       KC_D,  KC_H,  KC_T,  KC_N,  KC_S,    KC_RSFT,
        KC_LALT,  KC_LALT,       KC_X,  KC_C,    KC_J,  KC_Z,       KC_B,  KC_M,  KC_W,  KC_L,  KC_Q,    OSM(MOD_LCTL),
                       LT(DESK, KC_ESC), LT(SYM, KC_DOT), MOU_SMT,  LT(CTL, KC_ENT), KC_SPC, KC_V),

    [UA] = LAYOUT_split_3x6_3(
        TO(BASE), KC_Q,       KC_W,  KC_E,  KC_R,  KC_T,            KC_Y,  KC_U,  KC_I,    KC_O,   KC_P,    KC_LBRC,
        KC_LSFT,  MO(UANAV),  KC_S,  KC_D,  KC_F,  KC_G,            KC_H,  KC_J,  KC_K,    KC_L,   KC_SCLN, KC_LSFT,
        KC_LALT,  KC_Z,       KC_X,  KC_C,  KC_V,  KC_B,            KC_N,  KC_M,  KC_COMM, KC_DOT, KC_QUOT, KC_NO,
                       LT(DESK, KC_ESC), LT(UASYM, KC_SLSH), MOU_SMT,  KC_ENT, KC_SPC, KC_A),

    [SYM] = LAYOUT_split_3x6_3(
        KC_NO, KC_EXLM, KC_AT,   KC_HASH, KC_COLN, KC_PERC,         KC_COMM, KC_COMM, KC_ASTR, KC_LT,   KC_GT,   KC_NO,
        KC_NO, MO(NUM), KC_LBRC, KC_RBRC, KC_QUES, KC_SCLN,         KC_CIRC, KC_UNDS, KC_PIPE, KC_EQL,  KC_DLR,  KC_PLUS,
        KC_NO, KC_BSLS, KC_COMM, KC_LPRN, KC_RPRN, KC_AMPR,         KC_GRV,  KC_MINS, KC_LCBR, KC_RCBR, KC_TILD, KC_NO,
                                    KC_COMM, DOT_COM, KC_EQL,       KC_SCLN, KC_SLSH, TO_UKR),

    [UASYM] = LAYOUT_split_3x6_3(
        KC_NO, KC_EXLM,   KC_AT,   KC_HASH, KC_CIRC, KC_PERC,       KC_QUES, KC_QUES, KC_ASTR, KC_LPRN, KC_LBRC, KC_NO,
        KC_NO, MO(UANUM), KC_LBRC, KC_RBRC, KC_AMPR, KC_QUES,       KC_CIRC, KC_UNDS, KC_PIPE, KC_EQL,  KC_DLR,  KC_PLUS,
        KC_NO, KC_BSLS,   KC_QUES, KC_LPRN, KC_RPRN, KC_QUOT,       KC_GRV,  KC_MINS, KC_LCBR, KC_RCBR, KC_TILD, KC_NO,
                                   KC_QUES, KC_SLSH, KC_ENT,        KC_SCLN, KC_SLSH, TO_ENG),

    [NAV] = LAYOUT_split_3x6_3(
        LCTL(LSFT(KC_C)), KC_NO,   LCTL(KC_W), LCTL(KC_V), LCTL(KC_W), LCTL(KC_P),    LCTL(KC_Y),    KC_UP,            LCTL(KC_K),    LCTL(KC_R), LCTL(KC_V),   KC_NO,
        LCTL(KC_1),       KC_LSFT, SWAP_CST,   KC_BSPC,    SWAP_CT,    KC_NO,         LCTL(KC_D),    KC_LEFT,          LCTL(KC_T),    TO_UKR,     KC_END,       KC_TAB,
        LCTL(KC_Z),       KC_NO,   LGUI(KC_L), LCTL(KC_C), KC_DOWN,    LCTL(LSFT(KC_I)), LCTL(KC_LEFT), LCTL(LSFT(KC_A)), LCTL(KC_RGHT), KC_RGHT,   LCTL(KC_ENT), KC_NO,
                                        LCTL(KC_1), MO(NUM), KC_ENT,  KC_ENT, LCTL(KC_1), LCTL(KC_BSPC)),

    [UANAV] = LAYOUT_split_3x6_3(
        LCTL(LSFT(KC_C)), KC_NO,   LCTL(KC_W), LCTL(KC_V), LCTL(KC_W), LCTL(KC_P),    LCTL(KC_Y),    KC_UP,            LCTL(KC_K),    LCTL(KC_R), LCTL(KC_V),   KC_NO,
        LCTL(KC_1),       KC_LSFT, SWAP_CST,   KC_BSPC,    SWAP_CT,    KC_LSFT,       LCTL(KC_D),    KC_LEFT,          LCTL(KC_T),    TO_ENG,     KC_LSFT,      LCTL(KC_BSPC),
        LCTL(KC_Z),       KC_NO,   LCTL(KC_L), LCTL(KC_C), KC_DOWN,    LCTL(LSFT(KC_I)), LALT(KC_LEFT), LCTL(LSFT(KC_A)), LALT(KC_RGHT), KC_RGHT,   LCTL(KC_ENT), KC_NO,
                                      LCTL(KC_1), MO(UANUM), KC_ENT,  KC_ENT, KC_NO, LCTL(KC_BSPC)),

    [NUM] = LAYOUT_split_3x6_3(
        KC_NO, KC_PIPE, KC_7,       KC_8,       KC_9,       KC_PLUS,  KC_LBRC, KC_7, KC_8, KC_9, KC_RBRC, KC_NO,
        KC_NO, KC_0,    LALT(KC_M), LALT(KC_W), LALT(KC_L), KC_GT,    KC_0,    KC_4, KC_5, KC_6, KC_0,    KC_NO,
        KC_NO, KC_TILD, KC_1,       KC_2,       KC_3,       KC_DEL,   KC_LCBR, KC_1, KC_2, KC_3, KC_RCBR, KC_NO,
                                     KC_NO, KC_NO, KC_LBRC,          KC_RBRC, KC_RPRN, KC_NO),

    [UANUM] = LAYOUT_split_3x6_3(
        KC_NO, KC_PIPE, KC_7, KC_8, KC_9, KC_PLUS,                   KC_LBRC, KC_7, KC_8, KC_9, KC_RBRC, KC_NO,
        KC_NO, KC_0,    KC_4, KC_5, KC_6, KC_GT,                     KC_0,    KC_4, KC_5, KC_6, KC_0,    KC_NO,
        KC_NO, KC_TILD, KC_1, KC_2, KC_3, KC_GRV,                    KC_LCBR, KC_1, KC_2, KC_3, KC_RCBR, KC_NO,
                                   KC_TRNS, KC_NO, KC_LBRC,          KC_RBRC, KC_RPRN, KC_NO),

    [DESK] = LAYOUT_split_3x6_3(
        KC_NO,   KC_BTN3,    LGUI(KC_2), LGUI(KC_3), LGUI(KC_4), LGUI(KC_5),   LGUI(KC_6), LGUI(KC_7), LGUI(KC_8), LCTL(KC_9), KC_NO,          KC_NO,
        KC_LSFT, LGUI(KC_1), LGUI(KC_0), KC_F18,     LGUI(KC_W), LGUI(KC_6),   LGUI(KC_1), LALT(KC_H), LALT(KC_W), LALT(KC_L), LGUI(KC_5),     KC_NO,
        KC_NO,   KC_NO,      LGUI(KC_E), SWAP_AT,    LGUI(KC_J), LGUI(KC_7),   LGUI(KC_6), KC_VOLD,    KC_VOLU,    LALT(KC_L), LALT(KC_COMM), KC_NO,
                          LGUI(KC_D), LGUI(KC_A), LGUI(KC_D),   OSM(MOD_LSFT), LALT(KC_A), KC_DEL),

    [CTL] = LAYOUT_split_3x6_3(
        KC_NO, KC_1,        KC_2,       KC_3,       LCTL(KC_MINS), LCTL(KC_PLUS),  KC_6,       LCTL(KC_K), KC_8,       KC_9,       KC_0,       KC_NO,
        KC_NO, LCTL(KC_F),  LCTL(KC_G), LCTL(KC_Y), LCTL(KC_U),    LCTL(KC_Y),     LCTL(KC_H), LCTL(KC_H), LCTL(KC_K), LCTL(KC_L), LCTL(KC_W), KC_NO,
        KC_NO, LSFT(KC_TAB),LALT(KC_R), LCTL(KC_J), LCTL(KC_B),    KC_NO,          KC_F6,      KC_SLSH,    KC_F8,      LCTL(KC_L), LCTL(KC_L), KC_NO,
                                       MO(ALTL), LCTL(KC_A), KC_NO,  LCTL(KC_N), LCTL(KC_A), KC_NO),

    [ALTL] = LAYOUT_split_3x6_3(
        KC_NO, DOT_COM, KC_COMM,    MO(DESK),   KC_P,       KC_Y,   KC_F, KC_K, KC_G, KC_R, KC_NO, KC_NO,
        KC_NO, KC_A,    LALT(KC_M), LALT(KC_W), LALT(KC_L), KC_U,   KC_D, KC_H, KC_T, KC_N, KC_S,  KC_NO,
        KC_NO, KC_X,    KC_V,       KC_C,       KC_J,       KC_Z,   KC_B, KC_M, KC_W, KC_L, KC_Q,  KC_NO,
                                   KC_NO, KC_SPC, KC_NO,    KC_TRNS, KC_NO, KC_NO),

    // Jump keys F19-F22 are claimed by easy-jump as global hotkeys; they
    // do nothing until it implements them.
    [MOUSE] = LAYOUT_split_3x6_3(
        KC_NO, KC_NO, KC_NO,   KC_MS_U, KC_NO,   KC_NO,             KC_NO,   KC_NO,   KC_F20,  KC_NO,   KC_NO,   KC_NO,
        KC_NO, KC_NO, KC_MS_L, KC_MS_D, KC_MS_R, KC_NO,             KC_BTN3, KC_F19,  KC_BTN1, KC_F22,  KC_BTN2, KC_BTN3,
        KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,             KC_WH_L, KC_WH_D, KC_F21,  KC_WH_U, KC_WH_R, KC_NO,
                              KC_TRNS, KC_TRNS, KC_TRNS,            KC_BTN1, KC_BTN2, MOU_FST),

    // Wired board: no Bluetooth profiles or output switching to keep.
    [SET] = LAYOUT_split_3x6_3(
        QK_BOOT, KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO,               KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO,   KC_NO,   KC_NO, CAD,   CAE,   KC_NO,               KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        QK_RBT,  QK_BOOT, KC_NO, KC_NO, KC_NO, KC_NO,               KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                                KC_NO, KC_NO, KC_NO,                KC_NO, KC_NO, KC_NO),
};
