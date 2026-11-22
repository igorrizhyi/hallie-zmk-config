// Diagnostic build: the same layout as `hallie`, but with every custom
// keycode replaced by a plain equivalent and all custom C removed. If
// this types and `hallie` does not, the fault is in the custom code.

#include QMK_KEYBOARD_H

enum layers {
    BASE, UA, SYM, UASYM, NAV, UANAV, NUM, UANUM, DESK, CTL, ALTL, MOUSE, SET
};

#define CAD LCTL(LALT(KC_DEL))
#define CAE LCTL(LALT(KC_END))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [BASE] = LAYOUT_split_3x6_3(
        KC_UP,    OSM(MOD_LCTL), KC_A,  KC_QUOT, KC_P,  KC_Y,       KC_F,  KC_K,  KC_G,  KC_R,  KC_CAPS, KC_TAB,
        KC_LSFT,  MO(NAV),       KC_O,  KC_E,    KC_I,  KC_U,       KC_D,  KC_H,  KC_T,  KC_N,  KC_S,    KC_RSFT,
        KC_LALT,  KC_LALT,       KC_X,  KC_C,    KC_J,  KC_Z,       KC_B,  KC_M,  KC_W,  KC_L,  KC_Q,    OSM(MOD_LCTL),
                       LT(DESK, KC_ESC), LT(SYM, KC_DOT), MO(MOUSE),  LT(CTL, KC_ENT), KC_SPC, KC_V),

    [UA] = LAYOUT_split_3x6_3(
        TO(BASE), KC_Q,       KC_W,  KC_E,  KC_R,  KC_T,            KC_Y,  KC_U,  KC_I,    KC_O,   KC_P,    KC_LBRC,
        KC_LSFT,  MO(UANAV),  KC_S,  KC_D,  KC_F,  KC_G,            KC_H,  KC_J,  KC_K,    KC_L,   KC_SCLN, KC_LSFT,
        KC_LALT,  KC_Z,       KC_X,  KC_C,  KC_V,  KC_B,            KC_N,  KC_M,  KC_COMM, KC_DOT, KC_QUOT, KC_NO,
                       LT(DESK, KC_ESC), LT(UASYM, KC_SLSH), MO(MOUSE),  KC_ENT, KC_SPC, KC_A),

    [SYM] = LAYOUT_split_3x6_3(
        KC_NO, KC_EXLM, KC_AT,   KC_HASH, KC_COLN, KC_PERC,         KC_COMM, KC_COMM, KC_ASTR, KC_LT,   KC_GT,   KC_NO,
        KC_NO, MO(NUM), KC_LBRC, KC_RBRC, KC_QUES, KC_SCLN,         KC_CIRC, KC_UNDS, KC_PIPE, KC_EQL,  KC_DLR,  KC_PLUS,
        KC_NO, KC_BSLS, KC_COMM, KC_LPRN, KC_RPRN, KC_AMPR,         KC_GRV,  KC_MINS, KC_LCBR, KC_RCBR, KC_TILD, KC_NO,
                                    KC_COMM, KC_DOT, KC_EQL,       KC_SCLN, KC_SLSH, TO(UA)),

    [UASYM] = LAYOUT_split_3x6_3(
        KC_NO, KC_EXLM,   KC_AT,   KC_HASH, KC_CIRC, KC_PERC,       KC_QUES, KC_QUES, KC_ASTR, KC_LPRN, KC_LBRC, KC_NO,
        KC_NO, MO(UANUM), KC_LBRC, KC_RBRC, KC_AMPR, KC_QUES,       KC_CIRC, KC_UNDS, KC_PIPE, KC_EQL,  KC_DLR,  KC_PLUS,
        KC_NO, KC_BSLS,   KC_QUES, KC_LPRN, KC_RPRN, KC_QUOT,       KC_GRV,  KC_MINS, KC_LCBR, KC_RCBR, KC_TILD, KC_NO,
                                   KC_QUES, KC_SLSH, KC_ENT,        KC_SCLN, KC_SLSH, TO(BASE)),

    [NAV] = LAYOUT_split_3x6_3(
        LCTL(LSFT(KC_C)), KC_NO,   LCTL(KC_W), LCTL(KC_V), LCTL(KC_W), LCTL(KC_P),    LCTL(KC_Y),    KC_UP,            LCTL(KC_K),    LCTL(KC_R), LCTL(KC_V),   KC_NO,
        LCTL(KC_1),       KC_LSFT, LCTL(LSFT(KC_TAB)),   KC_BSPC,    LCTL(KC_TAB),    KC_NO,         LCTL(KC_D),    KC_LEFT,          LCTL(KC_T),    TO(UA),     KC_END,       KC_TAB,
        LCTL(KC_Z),       KC_NO,   LGUI(KC_L), LCTL(KC_C), KC_DOWN,    LCTL(LSFT(KC_I)), LCTL(KC_LEFT), LCTL(LSFT(KC_A)), LCTL(KC_RGHT), KC_RGHT,   LCTL(KC_ENT), KC_NO,
                                        LCTL(KC_1), MO(NUM), KC_ENT,  KC_ENT, LCTL(KC_1), LCTL(KC_BSPC)),

    [UANAV] = LAYOUT_split_3x6_3(
        LCTL(LSFT(KC_C)), KC_NO,   LCTL(KC_W), LCTL(KC_V), LCTL(KC_W), LCTL(KC_P),    LCTL(KC_Y),    KC_UP,            LCTL(KC_K),    LCTL(KC_R), LCTL(KC_V),   KC_NO,
        LCTL(KC_1),       KC_LSFT, LCTL(LSFT(KC_TAB)),   KC_BSPC,    LCTL(KC_TAB),    KC_LSFT,       LCTL(KC_D),    KC_LEFT,          LCTL(KC_T),    TO(BASE),     KC_LSFT,      LCTL(KC_BSPC),
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
        KC_LSFT, LGUI(KC_1), LGUI(KC_0), LALT(KC_TAB),    LGUI(KC_W), LGUI(KC_6),   LGUI(KC_1), LALT(KC_H), LALT(KC_W), LALT(KC_L), LGUI(KC_5),     KC_NO,
        KC_NO,   KC_NO,      LGUI(KC_E), LALT(KC_TAB),    LGUI(KC_J), LGUI(KC_7),   LGUI(KC_6), KC_VOLD,    KC_VOLU,    LALT(KC_L), LALT(KC_COMM), KC_NO,
                          LGUI(KC_D), LGUI(KC_A), LGUI(KC_D),   OSM(MOD_LSFT), LALT(KC_A), KC_DEL),

    [CTL] = LAYOUT_split_3x6_3(
        KC_NO, KC_1,        KC_2,       KC_3,       LCTL(KC_MINS), LCTL(KC_PLUS),  KC_6,       LCTL(KC_K), KC_8,       KC_9,       KC_0,       KC_NO,
        KC_NO, LCTL(KC_F),  LCTL(KC_G), LCTL(KC_Y), LCTL(KC_U),    LCTL(KC_Y),     LCTL(KC_H), LCTL(KC_H), LCTL(KC_K), LCTL(KC_L), LCTL(KC_W), KC_NO,
        KC_NO, LSFT(KC_TAB),LALT(KC_R), LCTL(KC_J), LCTL(KC_B),    KC_NO,          KC_F6,      KC_SLSH,    KC_F8,      LCTL(KC_L), LCTL(KC_L), KC_NO,
                                       MO(ALTL), LCTL(KC_A), KC_NO,  LCTL(KC_N), LCTL(KC_A), KC_NO),

    [ALTL] = LAYOUT_split_3x6_3(
        KC_NO, KC_DOT, KC_COMM,    MO(DESK),   KC_P,       KC_Y,   KC_F, KC_K, KC_G, KC_R, KC_NO, KC_NO,
        KC_NO, KC_A,    LALT(KC_M), LALT(KC_W), LALT(KC_L), KC_U,   KC_D, KC_H, KC_T, KC_N, KC_S,  KC_NO,
        KC_NO, KC_X,    KC_V,       KC_C,       KC_J,       KC_Z,   KC_B, KC_M, KC_W, KC_L, KC_Q,  KC_NO,
                                   KC_NO, KC_SPC, KC_NO,    KC_TRNS, KC_NO, KC_NO),

    // Jump keys F19-F22 are claimed by easy-jump as global hotkeys; they
    // do nothing until it implements them.
    [MOUSE] = LAYOUT_split_3x6_3(
        KC_NO, KC_NO, KC_NO,   KC_MS_U, KC_NO,   KC_NO,             KC_NO,   KC_NO,   KC_F20,  KC_NO,   KC_NO,   KC_NO,
        KC_NO, KC_NO, KC_MS_L, KC_MS_D, KC_MS_R, KC_NO,             KC_BTN3, KC_F19,  KC_BTN1, KC_F22,  KC_BTN2, KC_BTN3,
        KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,             KC_WH_L, KC_WH_D, KC_F21,  KC_WH_U, KC_WH_R, KC_NO,
                              KC_TRNS, KC_TRNS, KC_TRNS,            KC_BTN1, KC_BTN2, KC_ACL2),

    // Wired board: no Bluetooth profiles or output switching to keep.
    [SET] = LAYOUT_split_3x6_3(
        QK_BOOT, KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO,               KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO,   KC_NO,   KC_NO, CAD,   CAE,   KC_NO,               KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        QK_RBT,  QK_BOOT, KC_NO, KC_NO, KC_NO, KC_NO,               KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                                KC_NO, KC_NO, KC_NO,                KC_NO, KC_NO, KC_NO),
};
