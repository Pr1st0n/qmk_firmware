// Copyright 2022 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// clang-format off

#define LAYOUT_via(             \
      k17,     k15,    k16,     \
    k23,k22, k19,k18, k21,k20,  \
     k00, k01, k02, k03, k04,   \
     k05, k06, k07, k08, k09,   \
     k10, k11, k12, k13, k14    \
) { \
    { k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23 } \
}

#define U_LTESC LT(1, KC_ESC)

enum winry315_layers {
    BASE = 0,
    ONE,
    TWO,
    ADJUST
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_via(
            KC_HOME,           KC_MUTE,           LCTL(KC_UP),
        KC_TRNS, KC_TRNS,  KC_VOLD, KC_VOLU,  LCTL(KC_LEFT), LCTL(KC_RIGHT),
             KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
             KC_6,    KC_7,    KC_8,    KC_9,    LAG(KC_ESC),
             U_LTESC, KC_TAB,  KC_SPC,  RGB_TOG, HYPR(KC_A)
    ),
    [ONE] = LAYOUT_via(
            KC_TRNS,           KC_MUTE,           KC_TRNS,
        KC_TRNS, KC_TRNS,  KC_VOLD, KC_VOLU,  KC_TRNS, KC_TRNS,
             HYPR(KC_P), HYPR(KC_S), HYPR(KC_C), HYPR(KC_G), HYPR(KC_W),
             HYPR(KC_I), HYPR(KC_N), HYPR(KC_LBRC), HYPR(KC_X), HYPR(KC_O),
             HYPR(KC_F), HYPR(KC_M), HYPR(KC_RBRC), HYPR(KC_V), HYPR(KC_T)
    ),
    [TWO] = LAYOUT_via(
            KC_SLEP,           KC_MUTE,           BL_TOGG,
        KC_TRNS, KC_TRNS,  KC_VOLD, KC_VOLU,  KC_TRNS, KC_TRNS,
             KC_SLEP, KC_TRNS, KC_TRNS, KC_TRNS, BL_TOGG,
             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, HYPR(KC_2),
             HYPR(KC_X), KC_TRNS, KC_TRNS, KC_TRNS, HYPR(KC_1)
    ),
    [ADJUST] = LAYOUT_via(
            KC_SLEP,           KC_MUTE,           BL_TOGG,
        KC_TRNS, KC_TRNS,  KC_VOLD, KC_VOLU,  KC_TRNS, KC_TRNS,
             KC_SLEP, KC_TRNS, KC_TRNS, KC_TRNS, BL_TOGG,
             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, HYPR(KC_2),
             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, HYPR(KC_1)
    ),
};

// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case BASE:
        rgb_matrix_mode(RGB_MATRIX_RAINBOW_BEACON);
        break;
    case ONE:
        rgb_matrix_mode(RGB_MATRIX_GRADIENT_UP_DOWN);
        break;
    case TWO:
        rgb_matrix_mode(RGB_MATRIX_ALPHAS_MODS);
        break;
    case ADJUST:
        rgb_matrix_mode(RGB_MATRIX_PIXEL_RAIN);
        break;
    default: //  for any other layers, or the default layer
        rgblight_setrgb (0x00,  0xFF, 0xFF);
        break;
    }
  return state;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    switch (index) {
        case 0:
            // Left encoder
            if (clockwise && layer == ADJUST) {
              layer_move(BASE);
            } else if (!clockwise && layer == BASE) {
              layer_move(ADJUST);
            } else {
              layer_move((clockwise ? 1 : -1) + layer);
            }
            break;

        case 1:
            // Center encoder
            if (layer == ONE) {
                if (clockwise) {
                    rgblight_increase_sat();
                } else {
                    rgblight_decrease_sat();
                }
            } else {
                tap_code_delay(clockwise ? KC_VOLU : KC_VOLD, 10);
            }
            break;

        case 2:
            // Right encoder
            if (layer == ONE) {
                if (clockwise) {
                    rgblight_increase_val();
                } else {
                    rgblight_decrease_val();
                }
            } else {
                tap_code_delay(clockwise ? KC_MNXT : KC_MPRV, 10);
            }
            break;
    }
    return false;
}

