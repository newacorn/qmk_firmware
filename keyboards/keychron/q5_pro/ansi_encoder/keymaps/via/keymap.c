/* Copyright 2023 @ Keychron (https://www.keychron.com)
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
#include QMK_KEYBOARD_H
/*
#define TAP_DANCE_ENABLE
#define SEND_STRING_ENABLE
#include "print.h"
#include "quantum_keycodes.h"
#include "q5_pro.h"
#include "obj_keychron_q5_pro_ansi_encoder/src/info_config.h"
#include "obj_keychron_q5_pro_ansi_encoder/src/default_keyboard.h"
#include "rgb_matrix.h"
#include "leader.h"
#include "quantum.h"
#include "send_string_keycodes.h"
#include "process_tap_dance.h"
 */
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;


td_state_t cur_dance(tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void x_finished(tap_dance_state_t *state, void *user_data);
void x_reset(tap_dance_state_t *state, void *user_data);
enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
    CUSTOM_FN4,
    CUSTOM_FN5,
    CUSTOM_FN6,
};
// Tap Dance declarations
enum tap_dance_codes {
    TD_ESC_CAPS,
    X_CTL,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MAC_BASE] = LAYOUT_102_ansi(
KC_ESC,             KC_F1,  KC_F2,      KC_F3,    KC_F4,    KC_F5,  KC_F6,     KC_F7,     KC_F8,    KC_F9,    KC_F10,     KC_F11,    KC_F12,            KC_HOME,  KC_DEL,   KC_INS,   KC_PRINT_SCREEN,   0x66,
KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_END,  KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGUP,  KC_P7,    KC_P8,    KC_P9,      KC_PPLS,
MT(MOD_LCTL,TO(CUSTOM_FN4)),  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_PGDN,  KC_P4,    KC_P5,    KC_P6,
MT(MOD_LSFT,TO(CUSTOM_FN5)),            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              KC_P1,    KC_P2,    KC_P3,      KC_PENT,
KC_APP,  KC_LEFT_ALT, MT(MOD_LGUI,TO(MAC_FN)),                               KC_SPC,                                TD(X_CTL), KC_RGUI, KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT           ),
[MAC_FN] = LAYOUT_102_ansi(
    _______,            KC_MISSION_CONTROL,    KC_LAUNCHPAD,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,   _______,             _______,  _______,  _______,  _______,    _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  RGUI(KC_BSPC),            _______,  _______,  _______,  _______,    _______,
    KC_HOME,  KC_END,  KC_PAGE_UP,  KC_PAGE_DOWN,  DM_PLY1,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,    _______,
    KC_CAPS,  DM_REC1, DM_RSTP,  _______,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,  KC_RIGHT,  QK_LEAD,  _______,              _______,            _______,  _______,  _______,  _______,
    _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,  _______,            _______,  _______,  _______,    _______,
    _______,  _______,  _______,                                _______,                                LT(CUSTOM_FN6,TO(CUSTOM_FN6)),  _______,    _______,  _______,  _______,  _______,  _______,            _______           ),
[WIN_BASE] = LAYOUT_102_ansi(
KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,   KC_F12,             KC_DEL,   KC_F13,   KC_F14,   KC_F15,     0x66,
KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,  KC_P7,    KC_P8,    KC_P9,      KC_PPLS,
KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,  KC_P4,    KC_P5,    KC_P6,
KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              KC_P1,    KC_P2,    KC_P3,      KC_PENT,
KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 LT(WIN_FN,TO(CUSTOM_FN6)),KC_RALT, KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT           ),
[WIN_FN] = LAYOUT_102_ansi(
    _______,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,    KC_VOLD,  KC_VOLU,            _______,  _______,  _______,  _______,    RGB_TOG,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,    _______,
    RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,    _______,
    _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,  _______,  _______,  _______,
    _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,            _______,  _______,  _______,    _______,
    _______,  _______,  _______,                                _______,                                LT(CUSTOM_FN6,TO(CUSTOM_FN6)),  _______,    _______,  _______,  _______,  _______,  _______,            _______           ),
[CUSTOM_FN4] = LAYOUT_102_ansi(
    _______,            _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,   _______,             _______,  _______,  _______,  _______,    _______,
    KC_ESC,  KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10,  KC_F11,    KC_F12,  _______,            _______,  _______,  _______,  _______,    _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,    _______,
    _______,  _______, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,  _______,  _______,  _______,
    _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,  _______,            _______,  _______,  _______,    _______,
    _______,  _______,  _______,                                _______,                                LT(CUSTOM_FN6,TO(CUSTOM_FN6)),  _______,    _______,  _______,  _______,  _______,  _______,            _______           ),
[CUSTOM_FN5] = LAYOUT_102_ansi(
    _______,            _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,   _______,             _______,  _______,  _______,  _______,    _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,    _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,    _______,
    _______,  _______, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,  _______,  _______,  _______,
    _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,  _______,            _______,  _______,  _______,    _______,
    _______,  _______,  _______,                                _______,                                LT(CUSTOM_FN6,TO(CUSTOM_FN6)),  _______,    _______,  _______,  _______,  _______,  _______,            _______           ),
[CUSTOM_FN6] = LAYOUT_102_ansi(
    _______,            KC_BRID,    KC_BRIU,    C(KC_DOWN),    C(KC_UP),    RGB_VAD,    RGB_VAI,    KC_MPRV,    KC_MPLY,    KC_MNXT,    KC_MUTE,     KC_VOLD,   KC_VOLU,             _______,  _______,  _______,  _______,    _______,
    TO(MAC_BASE),  TO(MAC_FN),  TO(WIN_BASE),  TO(WIN_FN),  TO(CUSTOM_FN4),  TO(CUSTOM_FN5),  TO(CUSTOM_FN6),  _______,  _______,  _______,  _______,  _______,    _______,  BAT_LVL,            _______,  _______,  _______,  _______,    _______,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  KC_MS_WH_UP,  KC_MS_WH_DOWN,  KC_MS_WH_LEFT,  KC_MS_WH_RIGHT,    _______,  _______,            _______,  _______,  _______,  _______,    _______,
    KC_CAPS,  _______, KC_ACL0,  KC_ACL1,  KC_ACL2,  _______,  _______,  KC_MS_BTN1,  KC_MS_UP,  KC_MS_DOWN,  KC_MS_LEFT,  KC_MS_RIGHT,             KC_MS_BTN2,            _______,  _______,  _______,  _______,
    _______,           _______,  RGB_TOG,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,  _______,            _______,  _______,  _______,    _______,
    _______,  _______,  _______,                                _______,                              LT(CUSTOM_FN6,TO(CUSTOM_FN6)), _______,    _______,  _______,  _______,  _______,  _______,            _______           ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN]   = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_FN]   = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [CUSTOM_FN4]   = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [CUSTOM_FN5]   = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [CUSTOM_FN6]   = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
};
#endif // ENCODER_MAP_ENABLE
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
            // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}
bool rgb_matrix_indicators_user(void) {
    uint8_t current_layer = get_highest_layer(layer_state);
    rgb_matrix_set_color_all(0x0,0x00,0x00);
//    if (host_keyboard_led_state().caps_lock) {
//    }
    switch (current_layer) {
        case MAC_BASE:
//            rgb_matrix_set_color(17,0xFF,0x00,0x00);
            break;
        case MAC_FN:
            rgb_matrix_set_color(18,RGB_GREEN);
            break;
        case WIN_BASE:
            rgb_matrix_set_color(19,RGB_GREEN);
            break;
        case WIN_FN:
            rgb_matrix_set_color(20,RGB_GREEN);
            break;
        case CUSTOM_FN4:
            rgb_matrix_set_color(21,RGB_GREEN);
            break;
        case CUSTOM_FN5:
            rgb_matrix_set_color(22,RGB_GREEN);
            break;
        case CUSTOM_FN6:
            rgb_matrix_set_color(23,RGB_GREEN);
            break;
        default:
            break;
    }
    return true;
}
layer_state_t layer_state_set_user(layer_state_t state) {
    /*
    rgb_matrix_enable_noeeprom();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_set_color_all(0x0,0x00,0x00);
    */
    return state;
}
void keyboard_pre_init_user(void){
    rgb_matrix_enable_noeeprom();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_set_color_all(0x0,0x00,0x00);
}
/*
void keyboard_post_init_user(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(HSV_OFF);
}
*/

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(CUSTOM_FN6,TO(CUSTOM_FN6)):
        case LT(MAC_FN,TO(CUSTOM_FN6)):
        case LT(WIN_FN,TO(CUSTOM_FN6)):
            if (record->tap.count && record->event.pressed) {
                if(layer_state_is(CUSTOM_FN6)){
                    layer_off(CUSTOM_FN6);
                }else{
                    layer_move(CUSTOM_FN6);
                }
//                tap_code16(KC_DQUO); // Send KC_DQUO on tap
                return false;        // Return false to ignore further processing of key
            }
            break;
        case MT(MOD_LCTL,TO(CUSTOM_FN4)):
            if (record->tap.count && record->event.pressed) {
                if(layer_state_is(CUSTOM_FN4)){
                    layer_off(CUSTOM_FN4);
                }else{
                    layer_move(CUSTOM_FN4);
                }
                tap_code16(KC_LCTL);
                return  false;
            }
            break;
        case MT(MOD_LSFT,TO(CUSTOM_FN5)):
            if (record->tap.count && record->event.pressed) {
                if(layer_state_is(CUSTOM_FN5)){
                    layer_off(CUSTOM_FN5);
                }else{
                    layer_move(CUSTOM_FN5);
                }
                tap_code16(KC_LSFT);
                return  false;
            }
            break;
        case MT(MOD_LGUI,TO(MAC_FN)):
            if (record->tap.count && record->event.pressed) {
                if(layer_state_is(MAC_FN)){
                    layer_off(MAC_FN);
                }else{
                    layer_move(MAC_FN);
                }
                tap_code16(KC_LGUI);
                return  false;
            }
            break;
        case TD(X_CTL):
            break;
    }
    // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %d, time: %u, interrupt: %d, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
    return true;
}

void keyboard_post_init_user(void) {
    // 通过调整这些值可以改变其表现
    debug_enable=true;
    debug_matrix=true;
    debug_keyboard=true;
}


void leader_end_user(void) {
    if (leader_sequence_one_key(KC_F)) {
        // Leader, f => Types the below string
        SEND_STRING("QMK is awesome.");
    } else if (leader_sequence_two_keys(KC_D, KC_D)) {
        // Leader, d, d => Ctrl+A, Ctrl+C
        SEND_STRING(SS_LCTL("a") SS_LCTL("c"));
    } else if (leader_sequence_three_keys(KC_D, KC_D, KC_S)) {
        // Leader, d, d, s => Types the below string
        SEND_STRING("https://start.duckduckgo.com\n");
    }
}


// Set a long-ish tapping term for tap-dance keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 250;
        default:
            return TAPPING_TERM;
    }
}




// Create an instance of 'td_tap_t' for the 'x' tap dance.
static td_tap_t xtap_state = {
        .is_press_action = true,
        .state = TD_NONE
};
void x_each(tap_dance_state_t *state, void *user_data){
    uprintf("pressed:%d\n",state->pressed);
}
void x_finished(tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case TD_SINGLE_TAP:
            if(layer_state_is(CUSTOM_FN6)){
                layer_off(CUSTOM_FN6);
            }else{
                layer_move(CUSTOM_FN6);
            }
            break;
        case TD_SINGLE_HOLD:
            layer_move(MAC_FN);
            break;
        case TD_DOUBLE_TAP:
            tap_code16(KC_RIGHT_GUI);tap_code16(KC_RIGHT_GUI); break;
        case TD_DOUBLE_HOLD: register_code(KC_RGUI); break;
            // Last case is for fast typing. Assuming your key is `f`:
            // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
            // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
//        case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X); break;
        default: break;
    }
}
void x_reset(tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
//        case TD_SINGLE_TAP: unregister_code(KC_RGUI); break;
        case TD_SINGLE_HOLD:  layer_off(MAC_FN); break;
//        case TD_DOUBLE_TAP: unregister_code(KC_ESC); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_RGUI); break;
//        case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_X); break;
        default: break;
    }
    xtap_state.state = TD_NONE;
}
// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
        // Tap once for Escape, twice for Caps Lock
        [TD_ESC_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS),
        [X_CTL] = ACTION_TAP_DANCE_FN_ADVANCED(x_each, x_finished, x_reset)
};