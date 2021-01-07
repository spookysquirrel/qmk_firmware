/* Copyright 2020 Spooky Squirrel <one.spooky.squirrel@gmail.com>
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

enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

enum layer_names {
    _DEFAULT,
    _MOUSE,
    _LULZ,
    _SPECIAL,
};

enum custom_keycodes {
    SCREENSHOT = SAFE_RANGE,
    PASTE_SPECIAL,
    EMOTE_HAPPY,
    EMOTE_MAD,
    EMOTE_SAD,
    EMOTE_SHRUG,
    EMOTE_IDK,
    EMOTE_IGNORE,
};

enum {
    TD_TOGGLE_LAYER = 0,
};


void tap_dance_layer_switch(qk_tap_dance_state_t *state, void *user_data) {
    if ( state->count == (_LULZ+1) ) {
        layer_off(_MOUSE);
        layer_on(_LULZ);
    } else if ( state->count == (_MOUSE+1) ) {
        layer_off(_LULZ);
        layer_on(_MOUSE);
    } else {
        layer_off(_MOUSE);
        layer_on(_DEFAULT);
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_TOGGLE_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tap_dance_layer_switch, NULL),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        DEFAULT
        | Knob 1: Vol Dn/Up     | Knob 2: Fast Fwd/Rewind           | Knob 3:               |

        | Knob 1::Press: N/A    | Knob 2::Press: N/A                | Knob 3::Press: N/A    |
        | Media Previous        | Media Play/Pause                  | Media Next            |
        | Modifier              | Esc                               | Tap Party: Layer      |
    */
    [_DEFAULT] = LAYOUT(
        KC_NO, KC_NO, KC_NO,
        KC_MPRV, KC_MPLY , KC_MNXT,
        MO(_SPECIAL), KC_ESC, TD(TD_TOGGLE_LAYER)
    ),
    /*
        MOUSE
        | Knob 1: Mouse Up/Down | Knob 2: Mouse Wheel   | Knob 3: Mouse Left/Right  |

        | Knob 1::Press: N/A    | Knob 2::Press: N/A    | Knob 3::Press: N/A        |
        | Mouse Left Click      | Mouse Middle Click    | Mouse Right Click         |
        | Modifier              | N/A                   | Tap Party: Layer          |
    */
    [_MOUSE] = LAYOUT(
        KC_NO  , KC_NO, KC_NO,
        KC_MS_BTN1, KC_MS_BTN3, KC_MS_BTN2,
        _______, KC_NO , TD(TD_TOGGLE_LAYER)
    ),
    /*
        LULZ
        | Knob 1: Emoticons Happy   |   Knob 2: Emoticons Mad       | Knob 3: Emoticons Sad |

        | Knob 1::Press: N/A        | Knob 2::Press: N/A            | Knob 3::Press: N/A    |
        | Emote:Shrug               | Emote:Idk                     | Emote:Ignore          |
        | Modifier                  | Undo                          | Tap Party: Layer      |
    */
    [_LULZ] = LAYOUT(
        KC_NO, KC_NO, KC_NO,
        EMOTE_SHRUG, EMOTE_IDK, EMOTE_IGNORE,
        _______, KC_UNDO, TD(TD_TOGGLE_LAYER)
    ),
    /*
        SPECIAL
        | Knob 1: N/A           |   Knob 2: N/A         | Knob 3: N/A           |

        | Knob 1::Press: Reset  | Knob 2::Press: Sleep  | Knob 3::Press: Eject  |
        | N/A                   | Screenshot            | Paste Special         |
        | Modifier              | N/A                   | To Default Layer      |
     */
    [_SPECIAL] = LAYOUT(
        RESET, KC_SYSTEM_SLEEP, KC_AUDIO_MUTE,
        KC_NO, SCREENSHOT, PASTE_SPECIAL,
        _______, KC_NO, TO(0)
    ),
};

void encoder_update_user( uint8_t index, bool clockwise ) {
    if ( index == _LEFT ) { /* LEFT KNOB */
        if ( IS_LAYER_ON(_LULZ) ) {
            if ( clockwise ) {
                process_record_user(EMOTE_HAPPY, NULL);
            } else {
                // TODO: Reverse function
                process_record_user(EMOTE_HAPPY, NULL);
            }
        } else if ( IS_LAYER_ON(_MOUSE) ) {
            if ( clockwise ) {
                tap_code(KC_MS_UP);
            } else {
                tap_code(KC_MS_DOWN);
            }
        } else { /*DEFAULT LAYER */
            if ( clockwise ) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        }
    }
    else if ( index == _MIDDLE ) { /* MIDDLE KNOB */
        if ( IS_LAYER_ON(_LULZ) ) {
            if ( clockwise ) {
                process_record_user(EMOTE_MAD, NULL);
            } else {
                // TODO: Reverse
                process_record_user(EMOTE_MAD, NULL);
            }
        } else if (IS_LAYER_ON(_MOUSE)) {
            if ( clockwise ) {
                tap_code(KC_MS_WH_UP);
            } else {
                tap_code(KC_MS_WH_DOWN);
            }
        } else { /* DEFAULT LAYER */
            if ( clockwise ) {
                tap_code(KC_MEDIA_FAST_FORWARD);
            } else {
                tap_code(KC_MEDIA_REWIND);
            }
        }
    }
    else if ( index == _RIGHT ) { /* RIGHT KNOB */
        if ( IS_LAYER_ON(_LULZ) ) {
            if ( clockwise ) {
                process_record_user(EMOTE_SAD, NULL);
            } else {
                // TODO: Reverse
                process_record_user(EMOTE_SAD, NULL);
            }
        } else if ( IS_LAYER_ON(_MOUSE) ) {
            if ( clockwise ) {
                tap_code(KC_MS_RIGHT);
            } else {
                tap_code(KC_MS_LEFT);
            }
        } else { /* DEFAULT LAYER */
            if ( clockwise ) {
                tap_code(KC_AGAIN);
            } else {
                tap_code(KC_UNDO);
            }
        }
    }
}

// Happy emotes in rows of 5s
char* emotes_happy[11][40] = {
    {"(>‿◠)✌"}, {"٩(˘◡˘)۶"}, {" (•◡•) /"}, {"(─‿‿─)"}, {"(`▿ᕙ´)ᕗ"},
    {"☜(ˆ▿ˆc)"}, {"✍(◔◡◔)"}, {"(っ＾▿＾)۶🍸🌟🍺٩(˘◡˘ )"}, {"♪┏(・o･)┛♪┗ ( ･o･) ┓"}, {"ヽ(´▽`)/"},
    {"(☞ﾟヮﾟ)☞"},
};
int emotes_happy_size = sizeof(emotes_happy) / sizeof(emotes_happy[0]);
int emotes_happy_i = 0;

// Mad emotes
// TODO: Maybe create/move "(￣ー￣)" to unimpressed
char* emotes_mad[10][40] = {
    {"(╯°□°）╯︵ ┻━┻"}, {"ヽ(`Д´)ﾉ"}, {"(╬ ಠ益ಠ)"}, {"(ㆆ_ㆆ)"}, {"凸 ( ͡╯ . ͡╰)凸"},
    {"┻━┻ ︵ヽ(`▭´)ﾉ︵ ┻━┻"}, {"ᕙ( ︡'︡益'︠)ง"}, {"(≖_≖ )"}, {"t(>.<t)"}, {"(￣ー￣)"},
};
int emotes_mad_size = sizeof(emotes_mad) / sizeof(emotes_mad[0]);
int emotes_mad_i = 0;

// Sad emotes
// TODO: maybe create/move "(((( ；ﾟДﾟ)))" to scared/spooked
char* emotes_sad[6][40] = {
    {"( ˘︹˘ )"}, {"(つ︣﹏╰）"}, {"(╥﹏╥)"}, {"ಥ_ಥ"}, {"ε=ε=ε=┌(;*´Д`)ﾉ"}, 
    {"(((( ；ﾟДﾟ)))"},
};
int emotes_sad_size = sizeof(emotes_sad) / sizeof(emotes_sad[0]);
int emotes_sad_i = 0;


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    int this_emote_size = sizeof(emotes_mad[emotes_mad_i]) / sizeof(emotes_mad[emotes_mad_i][0]);

    switch (keycode) {
        case SCREENSHOT:
            // TODO
            // when keycode SCREENSHOT is pressed
            /*
            register_code(KC_LGUI);
            register_code(KC_LSHIFT);
            register_code(KC_F3);
            SEND_STRING("screenshot");
        
            unregister_code(KC_LGUI);
            unregister_code(KC_LSHIFT);
            unregister_code(KC_F3);
            */
            break;
        case PASTE_SPECIAL:
            // TODO
            // paste without formatting
            /*
            register_code(KC_LGUI);
            register_code(KC_LSHIFT);
            register_code(KC_LALT);
            register_code(KC_V);
            SEND_STRING("paste special");
        
            unregister_code(KC_LGUI);
            unregister_code(KC_LSHIFT);
            unregister_code(KC_LALT);
            unregister_code(KC_V);
            */
            break;
        case EMOTE_HAPPY:
            /* 
            Loop through each character at emotes_happy_index of array emotes_happy
            Then increment index
            */
            for(int i = 0; i < this_emote_size; i++) {
                send_unicode_string( emotes_happy[emotes_happy_i][i] );
            }
            if ( emotes_happy_i < (emotes_happy_size - 1) ) {
                emotes_happy_i += 1;
            } else {
                emotes_happy_i = 0;
            }
            break;
        case EMOTE_MAD:
            /* 
            Loop through each character at emotes_mad_index of array emotes_mad
            Then increment index
            */
            for(int i = 0; i < this_emote_size; i++) {
                send_unicode_string( emotes_mad[emotes_mad_i][i] );
            }
            if ( emotes_mad_i < (emotes_mad_size - 1) ) {
                emotes_mad_i += 1;
            } else {
                emotes_mad_i = 0;
            }
            break;
        case EMOTE_SAD:
            /* 
            Loop through each character at emotes_sad_index of array emotes_sad
            Then increment index
            */
            for(int i = 0; i < this_emote_size; i++) {
                send_unicode_string( emotes_sad[emotes_sad_i][i] );
            }
            if ( emotes_sad_i < (emotes_sad_size - 1) ) {
                emotes_sad_i += 1;
            } else {
                emotes_sad_i = 0;
            }
            break;
        case EMOTE_SHRUG:
            if (record->event.pressed) {
                send_unicode_string("¯\\_(ツ)_/¯");
            }
            break;
        case EMOTE_IDK:
            if (record->event.pressed) {
                send_unicode_string("┐('～`；)┌");
            }
            break;
        case EMOTE_IGNORE:
            if (record->event.pressed) {
                send_unicode_string("(ΘεΘ;)");
            }
            break;
        break;
    }
    return true;
};