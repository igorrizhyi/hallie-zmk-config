// Copyright 2022 LucW (@luc-languagetools)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "timer.h"
//#include <stdio.h>
//#include "dvorak_42_key.h"

// to build: qmk compile -kb crkbd/rev1 -km dvorak_42_key

enum my_keycodes {
  TO_UKR = SAFE_RANGE,
  TO_ENG_FAST,
  TO_ENG,
  CTRL_TAB,
  SFT_CTRL_TAB,
  ALT_TAB,
  SFT_ALT_TAB,
  TO_4_1,
  TO_4_3,
  MO_4_ESC,
  MO_1_DOWN,
  MO_6_ENT,
  LAYER_7_SHIFT,
  TO_DESK_1,
  TO_DESK_2,
  TO_DESK_3,
  TO_DESK_4,
  TO_DESK_5,
  TO_DESK_6,
  TO_DESK_8,
  TO_DESK_10,
  PREV_DESK,
  OPEN_LINK,
  ESC_MOUSE,
  ESC_AND_MOD,
  KC_ESC_12,
  MOUSE_FREE_MODE,
  MOUSE_ACCEL,
  MODE_BISECT,
  MODE_BISECT_EXIT,
  MODE_BISECT_EXIT_FULL,
  SPEACH_DET,
  SPEAK_ENG,
  SPEAK_RUS,
  SPEAK_UKR,
  RECENT_DISPLAY_SWITCH,
  MOUSE_MODE_CLICK,
  MOUSE_MODE_HOVER,
  CTRL_V,
  CTRL_L,
  CTRL_K
};


bool switch_lang_pending = false;

bool need_to_switch = false;
bool ua_layer_pending = false;
bool mo_4_used = false;
bool mo_1_used = false;
bool mo_6_used = false;
bool esc_mod_used = false;

bool mo_3_used = false;
bool mo_8_used = false;
bool mo_3_pressed = false;
bool mo_9_used = false;
bool mo_mouse_used = false;

bool shift_held = false;
bool layer_4_held = false;
bool cycle_pending = false;

int current_desktop = 0;
int prev_desktop = 0;
int desk_4_index = 0;
int desk_5_index = 0;

int display_1_app = 0;
int display_2_app = 0;

int prev_layer = 0;

bool mouse_click_pending = false;
int mouse_click_keys = 0;

int speak_index_activated = 0;

// const uint16_t PROGMEM mouse_free[] = {MO(3), KC_S, COMBO_END};
// const uint16_t PROGMEM mouse_free_ua[] = {MO(9), KC_SCLN, COMBO_END};
// combo_t key_combos[] = {
//     COMBO(mouse_free, MOUSE_FREE_MODE),
//     COMBO(mouse_free_ua, MOUSE_FREE_MODE),
// };

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case 4:
            break;
        default: //  for any other layers, or the default layer
            if (get_mods() & MOD_BIT(KC_LCTL)) {
                unregister_code(KC_LCTL);
            }
            if (get_mods() & MOD_BIT(KC_LALT)) {
                unregister_code(KC_LALT);
            }
            if (get_mods() & MOD_BIT(KC_LGUI)) {
                unregister_code(KC_LGUI);
            }
            if (get_mods() & MOD_BIT(KC_LSFT) & !shift_held) {
                unregister_code(KC_LSFT);
            }
            break;
        }
    return state;
}

void change_lang(void) {
    register_code(KC_LALT);
    register_code(KC_LSFT);
    unregister_code(KC_LSFT);
    unregister_code(KC_LALT);
}

void mo_4_esc_press(void) {
    mo_4_used = false;
    if (get_mods() & MOD_BIT(KC_CAPS)) {
        register_code(KC_CAPS);
        unregister_code(KC_CAPS);
    }
    if (IS_LAYER_ON(7)) {
        layer_off(7);
        layer_move(0);
        ua_layer_pending = true;
        change_lang();
        switch_lang_pending = true;
    } else if (IS_LAYER_ON(12) || IS_LAYER_ON(13)) {
        // if (mouse_click_pending) {
        //     register_code(KC_ESC);
        //     unregister_code(KC_ESC);
        // }

        layer_off(12);
        layer_off(13);
        unregister_code(KC_MS_ACCEL0);

        // mouse_click_keys = 0;
        // mouse_click_pending = false;

        if (prev_layer == 7) {
            ua_layer_pending = true;
            layer_move(0);
        }

        prev_layer = 12;
        mo_4_used = true;
    }
    layer_on(4);
}

void mo_4_esc_unpress(void) {
    layer_off(4);
    if (!mo_4_used) {
        register_code(KC_ESC);
        unregister_code(KC_ESC);
    }
    if (ua_layer_pending) {
        layer_move(7);
        if (prev_layer == 12) {
            if (!switch_lang_pending) {
                change_lang();
            }
            prev_layer = 7;
        }
        ua_layer_pending = false;
    }
    if (switch_lang_pending) {
        switch_lang_pending = false;
        change_lang();
    }
    mo_4_used = false;
}

void to_desk_5(void) {
    mo_4_used = true;
    if (current_desktop == 5 && desk_5_index == 0) {
        register_code(KC_LCTL);
        register_code(KC_5);
        unregister_code(KC_5);
        unregister_code(KC_LCTL);
        desk_5_index = 1;
    } else if (current_desktop == 5 && desk_5_index == 1) {
        register_code(KC_LCTL);
        register_code(KC_5);
        unregister_code(KC_5);
        unregister_code(KC_LCTL);
        desk_5_index = 0;
    } else if (desk_5_index == 0) {
        prev_desktop = current_desktop;
        register_code(KC_LCTL);
        register_code(KC_5);
        unregister_code(KC_5);
        unregister_code(KC_LCTL);
    } else if (desk_5_index == 1) {
        prev_desktop = current_desktop;
        register_code(KC_LCTL);
        register_code(KC_5);
        unregister_code(KC_5);
        unregister_code(KC_LCTL);
    }
    current_desktop = 5;
    ua_layer_pending = true;
    switch_lang_pending = true;
}

void to_desk_4(void) {
    mo_4_used = true;
    if (current_desktop == 4 && desk_4_index == 0) {
        register_code(KC_LGUI);
        register_code(KC_LALT);
        register_code(KC_4);
        unregister_code(KC_4);
        unregister_code(KC_LALT);
        unregister_code(KC_LGUI);
        desk_4_index = 1;
    } else if (current_desktop == 4 && desk_4_index == 1) {
        register_code(KC_LALT);
        register_code(KC_4);
        unregister_code(KC_4);
        unregister_code(KC_LALT);
        desk_4_index = 0;
    } else if (desk_4_index == 0) {
        prev_desktop = current_desktop;
        register_code(KC_LALT);
        register_code(KC_4);
        unregister_code(KC_4);
        unregister_code(KC_LALT);
    } else if (desk_4_index == 1) {
        prev_desktop = current_desktop;
        register_code(KC_LGUI);
        register_code(KC_LALT);
        register_code(KC_4);
        unregister_code(KC_4);
        unregister_code(KC_LALT);
        unregister_code(KC_LGUI);
    }
    current_desktop = 4;
    ua_layer_pending = true;
    switch_lang_pending = true;
}

void recent_display_switch(void) {
    int to_display = 0;
    int display_app = 0;
    mo_4_used = true;

    if (current_desktop == 1) {
        to_display = 2;
    } else if (current_desktop == 2) {
        to_display = 2;
    } else if (current_desktop == 3) {
        to_display = 2;
    } else if (current_desktop == 4) {
        to_display = 1;
    } else if (current_desktop == 5) {
        to_display = 1;
    } else if (current_desktop == 6) {
        to_display = 1;
    } else if (current_desktop == 7) {
        to_display = 1;
    } else if (current_desktop == 8) {
        to_display = 2;
    } else if (current_desktop == 9) {
        to_display = 1;
    }

    if (to_display == 1) {
        display_app = display_1_app;
    } else {
        display_app = display_2_app;
    }

    if (display_app == 1) {
        prev_desktop = current_desktop;
        current_desktop = 1;
        ua_layer_pending = false;
        register_code(KC_LALT);
        register_code(KC_1);
        unregister_code(KC_1);
        unregister_code(KC_LALT);
        switch_lang_pending = false;
    } else if (display_app == 2) {
        prev_desktop = current_desktop;
        current_desktop = 2;
        ua_layer_pending = false;
        register_code(KC_LALT);
        register_code(KC_2);
        unregister_code(KC_2);
        unregister_code(KC_LALT);
        switch_lang_pending = false;
    } else if (display_app == 3) {
        prev_desktop = current_desktop;
        current_desktop = 3;
        ua_layer_pending = false;
        register_code(KC_LALT);
        register_code(KC_3);
        unregister_code(KC_3);
        unregister_code(KC_LALT);
        switch_lang_pending = false;
    } else if (display_app == 4) {
        to_desk_4();
    } else if (display_app == 5) {
        to_desk_5();
    } else if (display_app == 6) {
        prev_desktop = current_desktop;
        current_desktop = 6;
        ua_layer_pending = false;
        register_code(KC_LALT);
        register_code(KC_6);
        unregister_code(KC_6);
        unregister_code(KC_LALT);
        switch_lang_pending = false;
    } else if (display_app == 8) {
        prev_desktop = current_desktop;
        current_desktop = 8;
        ua_layer_pending = false;
        register_code(KC_LALT);
        register_code(KC_8);
        unregister_code(KC_8);
        unregister_code(KC_LALT);
        switch_lang_pending = false;
    } else if (display_app == 10) {
        prev_desktop = current_desktop;
        current_desktop = 10;
        ua_layer_pending = false;
        register_code(KC_LALT);
        register_code(KC_0);
        unregister_code(KC_0);
        unregister_code(KC_LALT);
        switch_lang_pending = false;
    }
    register_code(KC_MS_DOWN);
    unregister_code(KC_MS_DOWN);
    register_code(KC_MS_UP);
    unregister_code(KC_MS_UP);
}

void mouse_jump(bool with_click) {
    mouse_click_pending = true;
    mouse_click_keys = 0;
    layer_off(13);
    layer_move(12);
    if (with_click) {
        register_code(KC_LGUI);
        register_code(KC_G);
        unregister_code(KC_G);
        unregister_code(KC_LGUI);
    } else {
        register_code(KC_LGUI);
        register_code(KC_N);
        unregister_code(KC_N);
        unregister_code(KC_LGUI);
    }
}

void mouse_mode_func(void) {
    if (IS_LAYER_ON(7)) {
        layer_off(7);
        change_lang();
        prev_layer = 7;
    } else {
        layer_off(0);
        prev_layer = 0;
    }
    layer_move(13);
    register_code(KC_MS_ACCEL0);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case CTRL_K:
        if (record->event.pressed) {
            mo_6_used = true;
            if (current_desktop == 1) {
                register_code(KC_LGUI);
                register_code(KC_K);
                unregister_code(KC_K);
                unregister_code(KC_LGUI);
            } else {
                register_code(KC_LCTL);
                register_code(KC_K);
                unregister_code(KC_K);
                unregister_code(KC_LCTL);
            }
        }
        return false;
    case CTRL_L:
        if (record->event.pressed) {
            mo_6_used = true;
            if (current_desktop == 1) {
                register_code(KC_LGUI);
                register_code(KC_RGHT);
                unregister_code(KC_RGHT);
                unregister_code(KC_LGUI);
            } else {
                register_code(KC_LCTL);
                register_code(KC_L);
                unregister_code(KC_L);
                unregister_code(KC_LCTL);
            }
        }
        return false;
    case CTRL_V:
        if (record->event.pressed) {
            if (current_desktop == 1) {
                register_code(KC_LSFT);
                register_code(KC_INS);
                unregister_code(KC_INS);
                unregister_code(KC_LSFT);
            } else {
                register_code(KC_LCTL);
                register_code(KC_V);
                unregister_code(KC_V);
                unregister_code(KC_LCTL);
            }
        }
        return false;
    case SPEAK_ENG:
        if (record->event.pressed) {
            speak_index_activated = 1;
            register_code(KC_LGUI);
            register_code(KC_LSFT);
            register_code(KC_1);
            unregister_code(KC_1);
            unregister_code(KC_LSFT);
            unregister_code(KC_LGUI);
        }
        return false;
    case SPEAK_RUS:
        if (record->event.pressed) {
            speak_index_activated = 2;
            register_code(KC_LGUI);
            register_code(KC_LSFT);
            register_code(KC_2);
            unregister_code(KC_2);
            unregister_code(KC_LSFT);
            unregister_code(KC_LGUI);
        }
        return false;
    case SPEAK_UKR:
        if (record->event.pressed) {
            speak_index_activated = 3;
            register_code(KC_LGUI);
            register_code(KC_LSFT);
            register_code(KC_3);
            unregister_code(KC_3);
            unregister_code(KC_LSFT);
            unregister_code(KC_LGUI);
        }
        return false;
    case SPEACH_DET:
        if (record->event.pressed) {
            mo_3_used = true;
            register_code(KC_LGUI);
            register_code(KC_S);
            unregister_code(KC_S);
            unregister_code(KC_LGUI);
        } else {
            register_code(KC_LSFT);
            register_code(KC_LGUI);
            register_code(KC_S);
            unregister_code(KC_S);
            unregister_code(KC_LGUI);
            unregister_code(KC_LSFT);
        }
        return false;
    case MODE_BISECT:
        if (record->event.pressed) {
            layer_off(13);
            layer_move(14);
            register_code(KC_LGUI);
            register_code(KC_N);
            unregister_code(KC_N);
            unregister_code(KC_LGUI);
        }
        return false;
    case MODE_BISECT_EXIT:
        if (record->event.pressed) {
            layer_off(14);
            layer_move(13);
            register_code(KC_ENT);
            unregister_code(KC_ENT);
        }
        return false;
    case MODE_BISECT_EXIT_FULL:
        if (record->event.pressed) {
            layer_off(14);
            layer_move(13);
            register_code(KC_ENT);
            unregister_code(KC_ENT);
        }
        return false;
    case RECENT_DISPLAY_SWITCH:
        if (record->event.pressed) {
            recent_display_switch();
        }
        return false;
    case MOUSE_ACCEL:
        if (record->event.pressed) {
            unregister_code(KC_MS_ACCEL0);
            register_code(KC_MS_ACCEL1);
        } else {
            unregister_code(KC_MS_ACCEL1);
            register_code(KC_MS_ACCEL0);
        }
        return false;
    case MOUSE_MODE_CLICK:
        if (record->event.pressed) {
            mouse_jump(true);
        }
        return false;
    case MOUSE_MODE_HOVER:
        if (record->event.pressed) {
            mouse_jump(false);
        }
        return false;
    case MOUSE_FREE_MODE:
        if (record->event.pressed) {
            mo_mouse_used = false;
            if (IS_LAYER_ON(7)) {
                switch_lang_pending = true;
            }
            mouse_mode_func();
        } else {

            if (!mouse_click_pending) {
                layer_off(12);
                layer_off(13);
                if (prev_layer == 7) {
                    change_lang();
                    layer_move(7);
                }
                unregister_code(KC_MS_ACCEL0);
            }

            mo_mouse_used = false;
        }
        return false;
    case KC_ESC_12:
        if (record->event.pressed) {
            register_code(KC_ESC);
            unregister_code(KC_ESC);
            mouse_click_keys = 0;
            mouse_click_pending = false;
        }
        return false;
    case ESC_MOUSE:
        if (record->event.pressed) {
            register_code(KC_ESC);
            unregister_code(KC_ESC);
            layer_off(12);
            layer_off(13);
            unregister_code(KC_MS_ACCEL0);
            layer_move(prev_layer);
            if (prev_layer == 7) {
                register_code(KC_LGUI);
                register_code(KC_SPC);
                unregister_code(KC_SPC);
                unregister_code(KC_LGUI);
            }
        }
        return false;
    case OPEN_LINK:
        if (record->event.pressed) {
            if (IS_LAYER_ON(7)) {
                layer_off(7);
            }
            if (IS_LAYER_ON(13)) {
                register_code(KC_SPC);
                unregister_code(KC_SPC);
                layer_off(13);
                unregister_code(KC_MS_ACCEL0);
            } else {
                register_code(KC_BTN1);
                unregister_code(KC_BTN1);
            }
            layer_move(0);
            ua_layer_pending = false;
            mo_4_used = true;
            prev_desktop = current_desktop;
            current_desktop = 3;
        }
        return false;
    case LAYER_7_SHIFT:
        if (record->event.pressed) {
            layer_off(9);
            register_code(KC_LSFT);
        } else {
            unregister_code(KC_LSFT);
        }
        return false;
    case MO_6_ENT:
        if (record->event.pressed) {
            mo_6_used = false;
		    layer_on(6);
        } else {
		    layer_off(6);
            if (!mo_6_used) {
                register_code(KC_ENT);
                unregister_code(KC_ENT);
            }
            mo_6_used = false;
        }
        return false;
    case ESC_AND_MOD:
        if (record->event.pressed) {
            // register_code(KC_ESC);
            // unregister_code(KC_ESC);
            if (mouse_click_pending) {
                register_code(KC_ESC);
                unregister_code(KC_ESC);
            }
            layer_off(12);
            layer_off(13);
            unregister_code(KC_MS_ACCEL0);
            if (prev_layer == 7) {
                ua_layer_pending = true;
                layer_on(9);
            } else {
                layer_on(3);
            }
        } else {

            if (IS_LAYER_ON(3)) {
                layer_off(3);
                esc_mod_used = mo_3_used;
            } else if (IS_LAYER_ON(9)) {
                layer_off(9);
                esc_mod_used = mo_9_used;
            }

            if (esc_mod_used) {
                if (ua_layer_pending) {
                    layer_move(7);
                    register_code(KC_LALT);
                    register_code(KC_LSFT);
                    unregister_code(KC_LSFT);
                    unregister_code(KC_LALT);
                    ua_layer_pending = false;
                } else {
                    layer_move(prev_layer);
                }
            } else {
                mouse_mode_func();
            }
        }
        return false;
    case MO_4_ESC:
        if (record->event.pressed) {
            mo_4_esc_press();
        } else {
            mo_4_esc_unpress();
        }
        return false;
    case MO_1_DOWN:
        if (record->event.pressed) {
            mo_1_used = false;
		    layer_on(1);
        } else {
		    layer_off(1);
            if (layer_4_held) {
                layer_off(2);
                layer_4_held = false;
                layer_on(3);
                return false;
            } else if (!mo_1_used) {
                register_code(KC_DOWN);
                unregister_code(KC_DOWN);
            }
            mo_1_used = false;
        }
        return false;
    case TO_4_1:
        if (record->event.pressed) {
            layer_off(3);
            layer_off(1);
		    layer_on(2);
            layer_4_held = true;
            mo_3_used = true;
        } else {
            if (layer_4_held) {
                layer_off(3);
                layer_off(2);
                layer_4_held = false;
                layer_on(1);
            } else if (IS_LAYER_ON(3)) {
                layer_off(3);
            }
        }
        return false;
    case TO_4_3:
        if (record->event.pressed) {
            layer_off(3);
            layer_off(1);
		    layer_on(2);
            layer_4_held = true;
            mo_3_used = true;
        } else {
            if (layer_4_held) {
                layer_off(1);
                layer_off(2);
                layer_4_held = false;
                layer_on(3);
            } else if (IS_LAYER_ON(1)) {
                layer_off(1);
            }
        }
        return false;
    case TO_DESK_1:
        if (record->event.pressed) {
            mo_4_used = true;
            prev_desktop = current_desktop;
            current_desktop = 1;
            register_code(KC_LALT);
            register_code(KC_1);
            unregister_code(KC_1);
            unregister_code(KC_LALT);
            ua_layer_pending = false;
            switch_lang_pending = false;
            display_1_app = 1;
        }
        return false;
    case TO_DESK_2:
        if (record->event.pressed) {
            mo_4_used = true;
            prev_desktop = current_desktop;
            current_desktop = 2;
            register_code(KC_LALT);
            register_code(KC_2);
            unregister_code(KC_2);
            unregister_code(KC_LALT);
            ua_layer_pending = false;
            switch_lang_pending = false;
            display_1_app = 2;
        }
        return false;
    case TO_DESK_3:
        if (record->event.pressed) {
            mo_4_used = true;
            prev_desktop = current_desktop;
            current_desktop = 3;
            register_code(KC_LALT);
            register_code(KC_3);
            unregister_code(KC_3);
            unregister_code(KC_LALT);
            ua_layer_pending = false;
            switch_lang_pending = false;
            display_1_app = 3;
        }
        return false;
    case TO_DESK_4:
        if (record->event.pressed) {
            to_desk_4();
            display_2_app = 4;
        }
        return false;
    case TO_DESK_5:
        if (record->event.pressed) {
            to_desk_5();
            display_2_app = 5;
        }
        return false;
    case TO_DESK_6:
        if (record->event.pressed) {
            mo_4_used = true;
            prev_desktop = current_desktop;
            current_desktop = 6;
            register_code(KC_LALT);
            register_code(KC_6);
            unregister_code(KC_6);
            unregister_code(KC_LALT);
            ua_layer_pending = false;
            switch_lang_pending = false;
            display_2_app = 6;
        }
        return false;
    case TO_DESK_8:
        if (record->event.pressed) {
            mo_4_used = true;
            prev_desktop = current_desktop;
            current_desktop = 8;
            register_code(KC_LALT);
            register_code(KC_8);
            unregister_code(KC_8);
            unregister_code(KC_LALT);
            ua_layer_pending = false;
            switch_lang_pending = false;
            display_1_app = 8;
        }
        return false;
    case TO_DESK_10:
        if (record->event.pressed) {
            mo_4_used = true;
            prev_desktop = current_desktop;
            current_desktop = 10;
            register_code(KC_LALT);
            register_code(KC_0);
            unregister_code(KC_0);
            unregister_code(KC_LALT);
            ua_layer_pending = false;
            switch_lang_pending = false;
        }
        return false;
    case PREV_DESK:
        if (record->event.pressed) {
            mo_4_used = true;
            need_to_switch = true;
            if (current_desktop == 4) {
                register_code(KC_LCTL);
                register_code(KC_LSFT);
                register_code(KC_4);
                unregister_code(KC_4);
                unregister_code(KC_LCTL);
                unregister_code(KC_LSFT);
                need_to_switch = false;
            } else if (current_desktop == 5) {
                register_code(KC_LCTL);
                register_code(KC_LSFT);
                register_code(KC_5);
                unregister_code(KC_5);
                unregister_code(KC_LCTL);
                unregister_code(KC_LSFT);
                need_to_switch = false;
            }
            if (prev_desktop == 1) {
                prev_desktop = current_desktop;
                current_desktop = 1;
                ua_layer_pending = false;
                if (need_to_switch) {
                    register_code(KC_LALT);
                    register_code(KC_1);
                    unregister_code(KC_1);
                    unregister_code(KC_LALT);
                }
                switch_lang_pending = false;
                display_1_app = 1;
            } else if (prev_desktop == 2) {
                prev_desktop = current_desktop;
                current_desktop = 2;
                ua_layer_pending = false;
                if (need_to_switch) {
                    register_code(KC_LALT);
                    register_code(KC_2);
                    unregister_code(KC_2);
                    unregister_code(KC_LALT);
                }
                switch_lang_pending = false;
                display_1_app = 2;
            } else if (prev_desktop == 3) {
                prev_desktop = current_desktop;
                current_desktop = 3;
                ua_layer_pending = false;
                if (need_to_switch) {
                    register_code(KC_LALT);
                    register_code(KC_3);
                    unregister_code(KC_3);
                    unregister_code(KC_LALT);
                }
                switch_lang_pending = false;
                display_1_app = 3;
            } else if (prev_desktop == 4) {
                to_desk_4();
                display_2_app = 4;
            } else if (prev_desktop == 5) {
                to_desk_5();
                display_2_app = 5;
            } else if (prev_desktop == 6) {
                prev_desktop = current_desktop;
                current_desktop = 6;
                ua_layer_pending = false;
                if (need_to_switch) {
                    register_code(KC_LALT);
                    register_code(KC_6);
                    unregister_code(KC_6);
                    unregister_code(KC_LALT);
                }
                switch_lang_pending = false;
                display_2_app = 6;
            } else if (prev_desktop == 8) {
                prev_desktop = current_desktop;
                current_desktop = 8;
                ua_layer_pending = false;
                if (need_to_switch) {
                    register_code(KC_LALT);
                    register_code(KC_8);
                    unregister_code(KC_8);
                    unregister_code(KC_LALT);
                }
                switch_lang_pending = false;
                display_1_app = 8;
            } else if (prev_desktop == 10) {
                prev_desktop = current_desktop;
                current_desktop = 10;
                ua_layer_pending = false;
                if (need_to_switch) {
                    register_code(KC_LALT);
                    register_code(KC_0);
                    unregister_code(KC_0);
                    unregister_code(KC_LALT);
                }
                switch_lang_pending = false;
            }
            register_code(KC_MS_DOWN);
            unregister_code(KC_MS_DOWN);
            register_code(KC_MS_UP);
            unregister_code(KC_MS_UP);
        }
        return false;
    case ALT_TAB:
        // if (record->event.pressed) {
        //     mo_4_used = true;
        //     register_code(KC_LALT);
        //     register_code(KC_L);
        //     unregister_code(KC_L);
        //     unregister_code(KC_LALT);
        // }
        if (record->event.pressed) {
            mo_4_used = true;
            if (IS_LAYER_ON(4) && !(get_mods() & MOD_BIT(KC_LGUI))) {
                register_code(KC_LGUI);
            }
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
        }
        return false;
    case SFT_ALT_TAB:
        // if (record->event.pressed) {
        //     mo_4_used = true;
        //     register_code(KC_LALT);
        //     register_code(KC_H);
        //     unregister_code(KC_H);
        //     unregister_code(KC_LALT);
        // }
        if (record->event.pressed) {
            mo_4_used = true;
            if (IS_LAYER_ON(4) && !(get_mods() & MOD_BIT(KC_LGUI))) {
                register_code(KC_LGUI);
            }
            register_code(KC_LSFT);
            register_code(KC_TAB);
        } else {
            unregister_code(KC_LSFT);
            unregister_code(KC_TAB);
        }
        return false;
    case CTRL_TAB:
        if (record->event.pressed) {
            mo_3_used = true;
            if (!cycle_pending) {
                cycle_pending = true;
            }
            if ((IS_LAYER_ON(3) || IS_LAYER_ON(9)) && (get_mods() & MOD_BIT(KC_LSFT))) {
                unregister_code(KC_LSFT);
            }
            if ((IS_LAYER_ON(3) || IS_LAYER_ON(9)) && !(get_mods() & MOD_BIT(KC_LCTL))) {
                register_code(KC_LCTL);
            }
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
        }
        return false;
    case SFT_CTRL_TAB:
        if (record->event.pressed) {
            mo_3_used = true;
            if (!cycle_pending) {
                cycle_pending = true;
            }
            if (IS_LAYER_ON(3) && !(get_mods() & MOD_BIT(KC_LCTL))) {
                register_code(KC_LCTL);
            }
            if (IS_LAYER_ON(3) && !(get_mods() & MOD_BIT(KC_LSFT))) {
                register_code(KC_LSFT);
            }
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
            unregister_code(KC_LSFT);
        }
        return false;
    case KC_PGUP:
        if (record->event.pressed) {
            if ((IS_LAYER_ON(3) || IS_LAYER_ON(9)) && (get_mods() & MOD_BIT(KC_LSFT))) {
                unregister_code(KC_LSFT);
            }
            if ((IS_LAYER_ON(3) || IS_LAYER_ON(9)) && !(get_mods() & MOD_BIT(KC_LCTL))) {
                register_code(KC_LCTL);
            }
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
        }
        return false;
    case KC_PGDN:
        if (record->event.pressed) {
            if (IS_LAYER_ON(3) && !(get_mods() & MOD_BIT(KC_LCTL))) {
                register_code(KC_LCTL);
            }
            if (IS_LAYER_ON(3) && !(get_mods() & MOD_BIT(KC_LSFT))) {
                register_code(KC_LSFT);
            }
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
            unregister_code(KC_LSFT);
        }
        return false;
    case KC_DOT:
        if (record->event.pressed) {
            if ((get_mods() & MOD_BIT(KC_LSFT))) {
                unregister_code(KC_LSFT);
                register_code(KC_COMM);
                unregister_code(KC_COMM);
                register_code(KC_LSFT);
            } else {
                register_code(KC_DOT);
                unregister_code(KC_DOT);
            }
        }
        return false;
    case TO_UKR:
        if (record->event.pressed) {
            mo_1_used = true;
            mo_3_used = true;
            change_lang();
            layer_move(7);
        }
        return false;
    case TO_ENG_FAST:
        if (record->event.pressed) {
            mo_9_used = true;
            change_lang();
            layer_off(7);
            layer_move(0);
        }
        return false;
    case TO_ENG:
        if (record->event.pressed) {
            change_lang();
            layer_off(8);
            layer_move(0);
            layer_on(1);
        }
        return false;
    case MO(8):
        if (record->event.pressed) {
            mo_8_used = false;
            layer_on(8);
        } else {
            if (IS_LAYER_ON(8)) {
                layer_off(8);
            } else if (IS_LAYER_ON(1)) {
                layer_off(1);
            }
            if (!mo_8_used) {
                register_code(KC_SLSH);
                unregister_code(KC_SLSH);
            }
            mo_8_used = false;
        }
        return false;
    case MO(14):
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_LSFT);
            layer_move(0);
            layer_on(4);
            unregister_code(KC_LALT);
            unregister_code(KC_LSFT);
        } else {
            if (IS_LAYER_ON(14)) {
                layer_off(14);
            } else if (IS_LAYER_ON(4)) {
                layer_off(4);
            }
        }
        return false;
    case MO(1):
        if (record->event.pressed) {
            mo_1_used = false;
            layer_on(1);
        } else {
            layer_off(1);
            if (layer_4_held) {
                layer_off(1);
                layer_off(2);
                layer_4_held = false;
                layer_on(3);
                return false;
            } else if (!mo_1_used) {
                register_code(KC_DOT);
                unregister_code(KC_DOT);
            }
            mo_1_used = false;
        }
        return false;
    case MO(9):
        if (record->event.pressed) {
            mo_9_used = false;
            layer_on(9);
        } else {
            layer_off(9);
            mo_9_used = false;
        }
        return false;
    case MO(3):
        if (record->event.pressed) {
            mo_3_used = false;
            mo_3_pressed = true;
            layer_on(3);
        } else {
            mo_3_pressed = false;
            layer_off(3);
            if (layer_4_held) {
                layer_off(2);
                layer_4_held = false;
                layer_on(1);
            }
            // if (!mo_3_used) {
            //     mouse_mode_func(false);
            // }
            // if (cycle_pending) {
            //     cycle_pending = false;
            //     register_code(KC_LALT);
            //     register_code(KC_LSFT);
            //     register_code(KC_0);
            //     unregister_code(KC_LALT);
            //     unregister_code(KC_LSFT);
            //     unregister_code(KC_0);
            // }
            mo_3_used = false;
        }
        return false;
    case MO(15):
        if (record->event.pressed) {
            layer_on(15);
        } else {
            layer_off(15);
            if (speak_index_activated == 1) {
                register_code(KC_LGUI);
                register_code(KC_LCTL);
                register_code(KC_1);
                unregister_code(KC_1);
                unregister_code(KC_LCTL);
                unregister_code(KC_LGUI);
            } else if (speak_index_activated == 2) {
                register_code(KC_LGUI);
                register_code(KC_LCTL);
                register_code(KC_2);
                unregister_code(KC_2);
                unregister_code(KC_LCTL);
                unregister_code(KC_LGUI);
            } else if (speak_index_activated == 3) {
                register_code(KC_LGUI);
                register_code(KC_LCTL);
                register_code(KC_3);
                unregister_code(KC_3);
                unregister_code(KC_LCTL);
                unregister_code(KC_LGUI);
            }
        }
        return false;
    default:
        if (record->event.pressed) {
            if (IS_LAYER_ON(4)) {
                mo_4_used = true;
            }
            if (IS_LAYER_ON(1) || IS_LAYER_ON(2)) {
                mo_1_used = true;
            }
            if (IS_LAYER_ON(6)) {
                mo_6_used = true;
            }
            if (IS_LAYER_ON(3)) {
                mo_3_used = true;
            }
            if (IS_LAYER_ON(1)) {
                mo_1_used = true;
            }
            if (IS_LAYER_ON(8)) {
                mo_8_used = true;
            }
            if (IS_LAYER_ON(9)) {
                mo_9_used = true;
            }
            if (IS_LAYER_ON(12) || IS_LAYER_ON(13)) {
                mo_mouse_used = true;
            }
            if (IS_LAYER_ON(3) && (get_mods() & MOD_BIT(KC_LCTL))) {
                unregister_code(KC_LCTL);
            }
            if (IS_LAYER_ON(3) && (get_mods() & MOD_BIT(KC_LSFT) & shift_held)) {
                unregister_code(KC_LSFT);
            }

            // if (IS_LAYER_ON(4) && (get_mods() & MOD_BIT(KC_LGUI))) {
            //     unregister_code(KC_LGUI);
            // }
            // if (IS_LAYER_ON(4) && (get_mods() & MOD_BIT(KC_LGUI))) {
            //     unregister_code(KC_LALT);
            // }
        } else {
            if (mouse_click_pending) {
                mouse_click_keys = mouse_click_keys + 1;
                if (mouse_click_keys >= 2) {
                    mouse_click_keys = 0;
                    mouse_click_pending = false;
                    layer_off(12);

                    if (prev_layer == 7) {
                        layer_move(7);
                        change_lang();
                    } else {
                        layer_move(0);
                    }
                }
            }
        }
        return true;
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        KC_UP, OSM(MOD_LCTL), KC_A,  KC_QUOT,  KC_P,   KC_Y,                      KC_F,   KC_K,   KC_G,     KC_R,  KC_CAPS, KC_TAB,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LSFT, MO(3),     KC_O,     KC_E,    KC_I,   KC_U,                     KC_D,   KC_H,   KC_T,     KC_N,  KC_S, KC_RSFT,
 //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LALT, OSM(MOD_LGUI), KC_X,  KC_C,    KC_J,   KC_Z,                 KC_B, KC_M,   KC_W,     KC_L,  KC_Q, OSM(MOD_LCTL),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                            MO_4_ESC, MO(1), MOUSE_FREE_MODE,                          MO_6_ENT, KC_SPC, KC_V),
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
	[1] = LAYOUT_split_3x6_3(
			KC_NO, KC_EXLM, KC_AT, KC_HASH, LSFT(KC_SEMICOLON), KC_PERC, 					KC_COMM, KC_COMM, KC_ASTR, LSFT(KC_COMMA), LSFT(KC_DOT), KC_NO,
			KC_NO, TO_4_1, KC_LBRC, KC_RBRC, KC_QUES, KC_SEMICOLON, 			KC_CIRC, KC_UNDS, KC_PIPE, KC_EQL, KC_DLR, KC_PLUS,
			KC_NO, KC_BSLS, KC_COMM, KC_LPRN, KC_RPRN, KC_AMPR, 				KC_GRV, KC_MINS, KC_LCBR, KC_RCBR, KC_TILD, KC_NO,
											KC_COMM, KC_DOT, KC_EQL, 		        KC_SEMICOLON, KC_SLSH, TO_UKR),
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
	[2] = LAYOUT_split_3x6_3(
			KC_NO, KC_PIPE, KC_7, KC_8, KC_9, KC_PLUS, 			KC_LBRC, KC_7, KC_8, KC_9, KC_RBRC, KC_NO,
			KC_NO, KC_0, LALT(KC_M), LALT(KC_W), LALT(KC_L), LSFT(KC_DOT), 					KC_0, KC_4, KC_5, KC_6, KC_0, KC_NO,
			KC_NO, KC_TILD, KC_1, KC_2, KC_3, KC_DELETE, 				        LSFT(KC_LBRC), KC_1, KC_2, KC_3, LSFT(KC_RBRC), KC_NO,
												MO(3), MO(3), KC_LBRC, 			KC_RBRC, KC_RPRN, KC_NO),
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
	[3] = LAYOUT_split_3x6_3(
			LCTL(LSFT(KC_C)), KC_NO, LCTL(KC_W), CTRL_V, LCTL(KC_W), LCTL(KC_P), 				LCTL(KC_Y), KC_UP, LCTL(KC_K), LCTL(KC_R), LCTL(KC_V), KC_NO,
			LCTL(KC_1), KC_LSFT, SFT_CTRL_TAB, KC_BSPC, CTRL_TAB, MO(15), 			LCTL(KC_D), KC_LEFT, LCTL(KC_T), TO_UKR, KC_END, KC_TAB,
			LCTL(KC_Z), KC_NO, LGUI(KC_L), LCTL(KC_C), KC_DOWN, LCTL(LSFT(KC_I)), 		LCTL(KC_LEFT), LCTL(LSFT(KC_A)), LCTL(KC_RGHT), KC_RGHT, LCTL(KC_ENT), KC_NO,
											LCTL(KC_1), TO_4_3, KC_ENT, 			KC_ENT, LCTL(KC_1), LCTL(KC_BSPC)),
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
	[4] = LAYOUT_split_3x6_3(
			KC_NO, KC_BTN3, TO_DESK_2, TO_DESK_3, TO_DESK_4, TO_DESK_5, 	LALT(KC_6), KC_NO, KC_NO, LCTL(KC_9), KC_NO, KC_NO,
			KC_LSFT, TO_DESK_1, TO_DESK_10, LGUI(KC_D), LGUI(KC_W), TO_DESK_8, 	        LGUI(KC_1), LALT(KC_H), LALT(KC_W), LALT(KC_L), LGUI(KC_5), TO(11),
			KC_NO, RECENT_DISPLAY_SWITCH, LGUI(KC_E), PREV_DESK, LGUI(KC_J), TO_DESK_6, 				LGUI(KC_6), KC_VOLD, KC_VOLU, LALT(KC_L), LALT(KC_COMM), KC_NO,
										        LGUI(KC_D), LGUI(KC_A), LGUI(KC_D), 			OSM(MOD_LSFT), LALT(KC_A), KC_DEL),
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
	[5] = LAYOUT_split_3x6_3(
        KC_NO, KC_DOT, KC_COMMA, MO(4),  KC_P,   KC_Y,                                        KC_F,   KC_K,   KC_G,     KC_R,  KC_NO, KC_NO,
      //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_NO, KC_A,     LALT(KC_M),     LALT(KC_W),    LALT(KC_L),   KC_U,                                      KC_D,   KC_H,   KC_T,     KC_N,  KC_S, KC_NO,
     //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_NO, KC_X, KC_V,  KC_C,    KC_J,   KC_Z,                                            KC_B,   KC_M,   KC_W,     KC_L,  KC_Q, KC_NO,
										        KC_NO, KC_SPC, KC_NO, 			KC_TRNS, KC_NO, KC_NO),
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
	[6] = LAYOUT_split_3x6_3(
			KC_NO, KC_1, KC_2, KC_3, LCTL(KC_MINS), LCTL(KC_PLUS), 								KC_6, CTRL_K, KC_8, KC_9, KC_0, KC_NO,
			KC_NO, LCTL(KC_F), LCTL(KC_G), LCTL(KC_Y), LCTL(KC_U), LCTL(KC_Y), 	LCTL(KC_H), LCTL(KC_H), LCTL(KC_K), LCTL(KC_L), LCTL(KC_W), KC_NO,
			KC_NO, LSFT(KC_TAB), LALT(KC_R), LCTL(KC_J), LCTL(KC_B), KC_NO, 				KC_F6, KC_SLSH, KC_F8, CTRL_L, LCTL(KC_L), KC_NO,
										        MO(5), LCTL(KC_A), KC_NO, 			LCTL(KC_N), LCTL(KC_A), KC_NO),


  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
	[7] = LAYOUT_split_3x6_3(
			TO(0), KC_Q, KC_W, KC_E, KC_R, KC_T, 				                KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC,
			KC_LSFT, MO(9), KC_S, KC_D, KC_F, KC_G, 								KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_LSFT,
			KC_LALT, KC_Z, KC_X, KC_C, KC_V, KC_B, 								KC_N, KC_M, KC_COMM, KC_DOT, KC_QUOT, MOUSE_FREE_MODE,
                            MO_4_ESC, MO(8), MOUSE_FREE_MODE,  		                        KC_ENT, KC_SPC, KC_A
			),
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
	[8] = LAYOUT_split_3x6_3(
			KC_NO, KC_EXLM, KC_AT, KC_HASH, KC_CIRC, KC_PERC, 					LSFT(KC_SLSH), LSFT(KC_SLSH), KC_ASTR, KC_LPRN, KC_LBRC, KC_NO,
			KC_NO, MO(10), KC_LBRC, KC_RBRC, KC_AMPR, KC_QUES, 			KC_CIRC, KC_UNDS, KC_PIPE, KC_EQL, KC_DLR, KC_PLUS,
        KC_NO, KC_BSLS, LSFT(KC_SLSH), KC_LPRN, KC_RPRN, KC_QUOT, 				KC_GRV, KC_MINS, KC_LCBR, KC_RCBR, KC_TILD, KC_NO,
                                    LSFT(KC_SLSH), KC_SLSH, KC_ENT, 					KC_SEMICOLON, KC_SLSH, TO_ENG_FAST
			),
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
	[9] = LAYOUT_split_3x6_3(
			LCTL(LSFT(KC_C)), KC_NO, LCTL(KC_W), LCTL(KC_V), LCTL(KC_W), LCTL(KC_P), 				LCTL(KC_Y), KC_UP, LCTL(KC_K), LCTL(KC_R), LCTL(KC_V), KC_NO,
			LCTL(KC_1), KC_LSFT, SFT_CTRL_TAB, KC_BSPC, CTRL_TAB, KC_LSFT, 			LCTL(KC_D), KC_LEFT, LCTL(KC_T), TO_ENG_FAST, KC_LSFT, LCTL(KC_BSPC),
			LCTL(KC_Z), KC_NO, LCTL(KC_L), LCTL(KC_C), KC_DOWN, LCTL(LSFT(KC_I)), 		LALT(KC_LEFT), LCTL(LSFT(KC_A)), LALT(KC_RGHT), KC_RGHT, LCTL(KC_ENT), KC_NO,
											LCTL(KC_1), MO(10), KC_ENT, 			KC_ENT, KC_NO, LCTL(KC_BSPC)),

  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
	[10] = LAYOUT_split_3x6_3(
			KC_NO, KC_PIPE, KC_7, KC_8, KC_9, KC_PLUS, 			KC_LBRC, KC_7, KC_8, KC_9, KC_RBRC, KC_NO,
			KC_NO, KC_0, KC_4, KC_5, KC_6, LSFT(KC_DOT), 					KC_0, KC_4, KC_5, KC_6, KC_0, KC_NO,
			KC_NO, KC_TILD, KC_1, KC_2, KC_3, KC_GRV, 				        LSFT(KC_LBRC), KC_1, KC_2, KC_3, LSFT(KC_RBRC), KC_NO,
												KC_TRNS, KC_NO, KC_LBRC, 			KC_RBRC, KC_RPRN, KC_NO),

  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
	[11] = LAYOUT_split_3x6_3(
			KC_NO, KC_TAB, KC_Q, KC_W, KC_E, KC_R, 							    KC_Y, KC_U, KC_I, KC_O, KC_P, KC_NO,
			KC_NO, KC_LSFT, KC_A, KC_S, KC_D, KC_F, 							KC_H, KC_J, KC_K, KC_L, KC_SCLN, TO(0),
			KC_NO, KC_LCTL, KC_Z, KC_X, KC_C, KC_V, 							KC_1, KC_2, KC_3, KC_4, KC_5, KC_SLSH,
										MO(15), KC_SPC, MO(15), 				KC_RSFT, KC_NO, KC_NO
			),
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
	[12] = LAYOUT_split_3x6_3(
        KC_UP, KC_SEMICOLON, KC_A,  KC_QUOT,  KC_P,   KC_Y,                      KC_F,   KC_K,   KC_G,     KC_R,  KC_CAPS, KC_LGUI,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LSFT, ESC_AND_MOD,     KC_O,     KC_E,    KC_I,   KC_U,                     KC_D,   KC_H,   KC_T,     KC_N,  KC_S, KC_RSFT,
 //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        OSM(MOD_LALT), OSM(MOD_LGUI), KC_X,  KC_C,    KC_J,   KC_Z,                 KC_B, KC_M,   KC_W,     KC_L,  KC_Q, OSM(MOD_LCTL),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                            MO_4_ESC, KC_SPC, OPEN_LINK,                          OPEN_LINK, KC_NO, KC_V),
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.

	[13] = LAYOUT_split_3x6_3(
        KC_UP, KC_SEMICOLON, KC_A,  KC_MS_UP,  KC_P,   KC_Y,                      KC_F,   KC_MS_WH_LEFT,   KC_MS_WH_RIGHT,     KC_MS_WH_RIGHT,  KC_CAPS, KC_LGUI,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        MOUSE_ACCEL, MOUSE_MODE_CLICK,     KC_MS_LEFT,     KC_MS_DOWN,    KC_MS_RIGHT,   KC_U,                     KC_MS_BTN3,   KC_MS_WH_DOWN,   KC_MS_BTN1,     KC_MS_WH_UP,  KC_MS_BTN2, KC_MS_BTN3,
 //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        OSM(MOD_LALT), KC_O, KC_E,  KC_I,    KC_U,   KC_U,                 KC_D, KC_D,   KC_H,     KC_T,  KC_N, OSM(MOD_LCTL),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                            MO_4_ESC, MOUSE_ACCEL, OPEN_LINK,                          KC_J, MOUSE_ACCEL, MOUSE_MODE_HOVER),


	[14] = LAYOUT_split_3x6_3(
			KC_NO, KC_TAB, KC_Q, KC_W, KC_E, KC_R, 							    KC_Y, KC_U, KC_I, KC_O, KC_P, KC_NO,
			KC_NO, KC_A, KC_S, KC_D, KC_F, KC_F, 							KC_H, KC_J, KC_K, KC_L, KC_SCLN, TO(0),
			KC_NO, KC_LCTL, KC_Z, KC_X, KC_C, KC_V, 							KC_1, KC_2, KC_3, KC_4, KC_5, KC_SLSH,
                            MODE_BISECT_EXIT_FULL, KC_SPC, MO(13), 				KC_RSFT, MODE_BISECT_EXIT, KC_NO),
    [15] = LAYOUT_split_3x6_3(
			KC_NO, KC_1, KC_7, KC_8, KC_9, KC_5, 								KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO,
			KC_NO, KC_1, KC_4, KC_5, KC_6, KC_5, 					KC_D, KC_LEFT, KC_T, KC_N, KC_NO, TO(0),
			KC_NO, KC_ESC, KC_1, KC_2, KC_3, KC_NO, 						    LCTL(KC_LEFT), SPEAK_ENG, SPEAK_RUS, SPEAK_UKR, LCTL(KC_ENT), KC_NO,
										MO(2), KC_NO, KC_LALT, 					KC_ENT, KC_NO, KC_NO
			)

  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
};

