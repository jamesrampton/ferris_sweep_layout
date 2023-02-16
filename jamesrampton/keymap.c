#include QMK_KEYBOARD_H

// Callum swapper
// https://github.com/callum-oakley/qmk_firmware/blob/master/users/callum
#include "oneshot.h"
#include "swapper.h"
#include "keycodes.h"
#include "layer_lock.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Colemak Mod-DH layout
  [_CDH] = LAYOUT_split_3x5_2(
      KC_G, KC_C, KC_M, KC_N, KC_Z,            KC_X, KC_U, KC_O, KC_Y, KC_Q,
      KC_D, KC_S, KC_T, KC_R, KC_B,        KC_SLSH, KC_A, KC_E, KC_I, KC_H, 
      KC_W, KC_F, KC_L, KC_P, KC_V,        KC_QUOT, KC_K, KC_DOT, KC_COMM, KC_J,
                       KC_SPC, NUM,        OSM(MOD_LSFT), NAV
                              ),


  [_NUM] = LAYOUT_split_3x5_2(
    KC_NO,   GBPD,    PMIN,    KC_NO,   KC_BSPC,      KC_RBRC, KC_9, KC_8,KC_7,  KC_LBRC,        
    KC_NO,   OS_SHFT, OS_CMD,  OS_ALT,  OS_CTRL,      KC_EQL,  KC_6, KC_5,KC_4,  KC_SCLN,        
    KC_NO,   LLOCK,   KC_NO,   KC_BSLS, KC_ENT,       KC_MINS, KC_3, KC_2,KC_1,  KC_GRV,         
    KC_SPC,  KC_TRNS,                                 KC_0, KC_DOT        
                              ),

  [_NAV] = LAYOUT_split_3x5_2(
                          KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_BSPC,        KC_TAB,  TAB_R,  TAB_L, KC_ESC, SW_APP,       
                          KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_MEH,        M_SAVE,  OS_SHFT, OS_CMD, OS_ALT, OS_CTRL,      
                          KC_NO,   BR_BCK,  BR_FWD,  KC_NO,   KC_ENT,        M_PSTE,   LLOCK, M_COPY, M_CUT,  M_UNDO,       
                          LAU, KC_TRNS,                                                               KC_TRNS, KC_TRNS     
                              ),

  [_FUN] = LAYOUT_split_3x5_2(
                KC_F12,  KC_F7, KC_F8, KC_F9, CMD_DOT,  KC_TRNS,     KC_CAPS, KC_VOLU, KC_MUTE,  KC_VOLD,      
                KC_F11,  KC_F4, KC_F5, KC_F6, CMD_COMM, OS_CTRL,     KC_NO,   KC_MNXT, KC_MPLY,  KC_MPRV,        
                KC_F10,  KC_F1, KC_F2, KC_F3, CMD_ENT,  KC_NO,       KC_NO,   KC_BRIU, KC_SLEP,  KC_BRID,          
                KC_SPC,  KC_TRNS,                                                      KC_TRNS,  KC_TRNS          
                              )
};

enum combo_events {
   // punctuation combos
   MINS_COMBO,
   USCR_COMBO,
   SCLN_COMBO,
   // braces
   LPRN_COMBO,
   LCBR_COMBO,
   LBRC_COMBO,
   RPRN_COMBO,
   RCBR_COMBO,
   RBRC_COMBO,
   // modifier combos
   CTL_C_COMBO,
   CTL_B_COMBO,
   CTL_D_COMBO,
   CTL_R_COMBO,
   // Other combos...
   COMBO_LENGTH
 };
 uint16_t COMBO_LEN = COMBO_LENGTH;
// punctuation combos
 const uint16_t PROGMEM mins_combo[] = {KC_DOT, KC_K, COMBO_END};
 const uint16_t PROGMEM uscr_combo[] = {KC_F, KC_L, COMBO_END};
 const uint16_t PROGMEM scln_combo[] = {KC_B, KC_V, COMBO_END};
 // braces combos
 const uint16_t PROGMEM lprn_combo[] = {KC_U, KC_X, COMBO_END};
 const uint16_t PROGMEM lcbr_combo[] = {KC_A, KC_SLSH, COMBO_END};
 const uint16_t PROGMEM lbrc_combo[] = {KC_K, KC_QUOT, COMBO_END};
 const uint16_t PROGMEM rprn_combo[] = {KC_G, KC_C, COMBO_END};
 const uint16_t PROGMEM rcbr_combo[] = {KC_D, KC_S, COMBO_END};
 const uint16_t PROGMEM rbrc_combo[] = {KC_W, KC_F, COMBO_END};
 // modifier combos
 const uint16_t PROGMEM ctl_c_combo[] = {KC_S, KC_T, KC_R, COMBO_END};
 const uint16_t PROGMEM ctl_b_combo[] = {KC_I, KC_E, KC_A, COMBO_END};
 const uint16_t PROGMEM ctl_d_combo[] = {KC_C, KC_M, KC_N, COMBO_END};
 const uint16_t PROGMEM ctl_r_combo[] = {KC_F, KC_L, KC_P, COMBO_END};

 combo_t key_combos[] = {
   [MINS_COMBO] = COMBO(mins_combo, KC_MINS),
   [USCR_COMBO] = COMBO(uscr_combo, LSFT(KC_MINS)),
   [SCLN_COMBO] = COMBO(scln_combo, KC_SCLN),
   [LPRN_COMBO] = COMBO(lprn_combo, KC_LPRN),
   [LCBR_COMBO] = COMBO(lcbr_combo, KC_LCBR),
   [LBRC_COMBO] = COMBO(lbrc_combo, KC_LBRC),
   [RPRN_COMBO] = COMBO(rprn_combo, KC_RPRN),
   [RCBR_COMBO] = COMBO(rcbr_combo, KC_RCBR),
   [RBRC_COMBO] = COMBO(rbrc_combo, KC_RBRC),
   [CTL_C_COMBO] = COMBO(ctl_c_combo, LCTL(KC_C)),
   [CTL_B_COMBO] = COMBO(ctl_b_combo, LCTL(KC_B)),
   [CTL_D_COMBO] = COMBO(ctl_d_combo, LCTL(KC_D)),
   [CTL_R_COMBO] = COMBO(ctl_r_combo, LCTL(KC_R)),
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
    return update_tri_layer_state(state, _NUM, _NAV, _FUN);
}
