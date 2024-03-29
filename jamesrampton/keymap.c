#include QMK_KEYBOARD_H

// Callum swapper
// https://github.com/callum-oakley/qmk_firmware/blob/master/users/callum
#include "oneshot.h"
#include "swapper.h"
#include "keycodes.h"
#include "layer_lock.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_split_3x5_2(
                              KC_Q, KC_Y,    KC_O,    KC_U, KC_X,               KC_G,   KC_C, KC_M, KC_N, KC_Z,
                              KC_H, KC_I,    KC_E,    KC_A, KC_SLSH,            KC_D,   KC_S, KC_T, KC_R, KC_B,
                              KC_J, KC_COMM, KC_DOT,  KC_K, KC_QUOT,            KC_W,   KC_F, KC_L, KC_P, KC_V,
                                                      NAV,  OSM(MOD_LSFT),      KC_SPC, NUM 
                              ),


  [_NUM] = LAYOUT_split_3x5_2(
                              KC_LT,   KC_7, KC_8, KC_9,    KC_GT,              LCTL(KC_R), GBPD,       LCTL(KC_B), KC_GRV,  KC_BSPC,
                              KC_SCLN, KC_4, KC_5, KC_6,    KC_EQL,             LCTL(KC_C), OS_SHFT,    OS_CMD,     OS_ALT,  OS_CTRL,
                              KC_DOT,    KC_1, KC_2, KC_3,  KC_MINS,            LCTL(KC_D), LCTL(KC_N), LCTL(KC_P), KC_BSLS, KC_ENT,
                                                   KC_TRNS, KC_0,               KC_TRNS,    KC_TRNS
                              ),

  [_NAV] = LAYOUT_split_3x5_2(
                              LCTL(KC_F3), KC_ESC,       TAB_L,  TAB_R,   KC_NO,     KC_LPRN,       KC_LBRC, KC_RBRC, KC_RPRN, KC_BSPC,
                              OS_CTRL,     OS_ALT,       OS_CMD, OS_SHFT, KC_NO,     KC_LEFT,       KC_DOWN, KC_UP,   KC_RGHT, KC_MEH,
                              KC_TAB,      LSFT(KC_TAB), M_COPY, M_PSTE,  KC_NO,     LALT(KC_BSPC), BR_BCK,  BR_FWD,  KC_HOME, KC_END,
                                                                 KC_TRNS, KC_TRNS,   LAU,           KC_TRNS
                              ),

  [_FUN] = LAYOUT_split_3x5_2(
                              KC_TRNS, KC_VOLD, KC_MUTE, KC_VOLU, KC_LPRN,        KC_RPRN, KC_F7,  KC_F8, KC_F9, KC_F10,
                              OS_CTRL, OS_ALT,  OS_CMD,  OS_SHFT, KC_LCBR,        KC_RCBR, KC_F4,  KC_F5, KC_F6, KC_F11,
                              KC_COLN, KC_F14,  KC_MPLY, KC_F15,  KC_LBRC,        KC_RBRC, KC_F1,  KC_F2, KC_F3, KC_F12,
                                                         KC_TRNS, KC_TRNS,        KC_TRNS, KC_TRNS
                              )
};

// caps word setup
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_MINS:
        case KC_DEL:
        case KC_UNDS:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case NAV:
    case NUM:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case NAV:
    case NUM:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
        return true;
    default:
        return false;
    }
}


bool sw_app_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t* record) {

  update_swapper(
                 &sw_app_active, KC_LGUI, KC_TAB, SW_APP,
                 keycode, record
                 );

  update_oneshot(
                 &os_shft_state, KC_LSFT, OS_SHFT,
                 keycode, record
                 );

  update_oneshot(
                 &os_ctrl_state, KC_LCTL, OS_CTRL,
                 keycode, record
                 );

  update_oneshot(
                 &os_alt_state, KC_LALT, OS_ALT,
                 keycode, record
                 );

  update_oneshot(
                 &os_cmd_state, KC_LCMD, OS_CMD,
                 keycode, record
                 );

  if (!process_layer_lock(keycode, record, LLOCK)) { return false; }

  return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _NUM, _NAV, _FUN);
    state = update_tri_layer_state(state, _NAV, _NUM, _FUN);
    return state;
}
