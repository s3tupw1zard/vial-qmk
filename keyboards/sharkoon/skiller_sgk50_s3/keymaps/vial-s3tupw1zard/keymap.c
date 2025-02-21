// Copyright 2025 s3tupw1zard (@s3tupw1zard)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "eeconfig.h"
#include "rgb_matrix.h"

/**
 *  Enum for lighting profiles
 */ 

enum lighting_profiles {
    PROFILE_WINDOWS = 0,
    PROFILE_MINECRAFT,
    PROFILE_NMS,
    PROFILE_ASKA,
    PROFILE_ELDEN_RING,
    PROFILE_PALWORLD,
};

/**
 *  Enum for custom keycodes
 */ 

enum custom_keycodes {
    KC_PROFILE_WINDOWS = SAFE_RANGE,
    KC_PROFILE_MINECRAFT,
    KC_PROFILE_NMS,
    KC_PROFILE_ASKA,
    KC_PROFILE_ELDEN_RING,
    KC_PROFILE_PALWORLD,

    // End and Home keys missing a function
};

/**
 *  Enum for config types (Need to implement something to combine this enum with profile management)
 */ 

enum config_types {
    LAST_PRESSED_F13_F18,
    LAST_PRESSED_F19_F24
};

static enum config_types config_type;

/**
 *  Struct for storing values which are saved into EEPROM if needed
 */ 

typedef union {
    uint32_t raw;
    struct {
        uint8_t current_profile;
        uint8_t last_pressed_f13_f18;
        uint8_t last_pressed_f19_f24;
        uint8_t last_pressed_layer2;
    };
} user_config_t;

user_config_t user_config;


/**
 *  Global variables for storing the last pressed keys and sound status
 * 
 *  First two variables save last pressed led ids for custom mappings on layer 1 and 2
 * 
 *  Last two variables store the status of if the audio is muted and if music is playing
 */ 

static int last_pressed_f13_f18;
static int last_pressed_f19_f24;

static bool is_muted = false; // Status of KC_MUTE (Mute Sound Toggle)
static bool music_active = false; // Status of KC_MPLY (Music Toggle)


/**
 *  My custom keymap with additionally mapped F13 to F24 keys and some custom keycodes
 */ 

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_all(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_INS,  KC_HOME, KC_DEL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_PGUP,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_PGDN,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_END,
        KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   MO(1),
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(2),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [1] = LAYOUT_all(
        EE_CLR,  KC_MPLY, KC_MPRV, KC_MNXT, KC_MUTE,  KC_VOLD, KC_VOLU, _______,  _______,  _______, KC_PSCR, KC_SCRL, KC_PAUS, RM_VALD, RM_VALU, _______,
        _______, KC_F13,  KC_F14,  KC_F15,  KC_F16,   KC_F17,  KC_F18,  _______,  _______,  _______, _______, RM_SPDD, RM_SPDU, _______,          RM_SATU,
        _______, _______, _______, _______, _______,  KC_F23,  _______, _______,  _______,  KC_F24,  _______, _______, _______, _______,          RM_SATD,
        _______, _______, KC_F21,  KC_F22,  _______,  _______, _______, _______,  _______,  _______, _______, _______,          _______,          _______,
        _______, _______, _______, _______, KC_F19,   _______, _______, _______,  KC_F20,   _______, _______, _______,          _______, RM_VALU, _______,
        _______, _______, _______,                             KC_MPLY,                               _______, _______, _______, RM_HUEU, RM_VALD, RM_NEXT
    ),

    [2] = LAYOUT_all(
        _______,  _______, _______, _______, _______,  _______, _______, _______,  _______,  _______, _______, _______, _______, _______, _______, _______,
        _______, _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______, KC_PROFILE_WINDOWS, _______, _______, _______,          _______,
        _______, _______, _______, KC_PROFILE_ELDEN_RING, _______,  _______,  _______, _______,  _______,  _______,  KC_PROFILE_PALWORLD, _______, _______, _______,          _______,
        _______, KC_PROFILE_ASKA, _______,  _______,  _______,  _______, _______, _______,  _______,  _______, _______, _______,          _______,          _______,
        _______, _______, _______, _______, _______,   _______, _______, KC_PROFILE_NMS,  KC_PROFILE_MINECRAFT,   _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                             _______,                               _______, _______, _______, _______, _______, _______
    ),

};


/**
 *  Windows profile
 */ 

void profile_windows(void) {
    rgb_matrix_set_color(18, 0, 255, 0); // X
    rgb_matrix_set_color(17, 0, 255, 0); // C
    rgb_matrix_set_color(16, 0, 255, 0); // V
    rgb_matrix_set_color(20, 255, 0, 0); // A
    rgb_matrix_set_color(21, 255, 0, 0); // S
    rgb_matrix_set_color(85, 0, 0, 255); // WIN
    rgb_matrix_set_color(84, 0, 0, 255); // Lcontrol
    rgb_matrix_set_color(86, 0, 0, 255); // Alt
    rgb_matrix_set_color(82, 0, 0, 255); // LShift
    rgb_matrix_set_color(80, 0, 255, 0); // TAB
    rgb_matrix_set_color(35, 255, 0, 0); // Enter up
    rgb_matrix_set_color(36, 255, 0, 0); // Enter down
    rgb_matrix_set_color(61, 255, 0, 0); // Backspace
    rgb_matrix_set_color(4, 0, 0, 255);  // Left
    rgb_matrix_set_color(5, 0, 0, 255);  // Down
    rgb_matrix_set_color(6, 0, 0, 255);  // Right
    rgb_matrix_set_color(8, 0, 0, 255);  // Up
    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(81, 255, 0, 0);  // Set caps lock to red when locked
    } else {
        rgb_matrix_set_color(81, 0, 255, 0); // Set caps lock to blue when not locked
    }
}


/**
 *  Minecraft profile
 */ 

void profile_minecraft(void) {
    rgb_matrix_set_color(47, 255, 0, 0); // W
    rgb_matrix_set_color(20, 255, 0, 0); // A
    rgb_matrix_set_color(21, 255, 0, 0); // S
    rgb_matrix_set_color(22, 255, 0, 0); // D

    rgb_matrix_set_color(46, 0, 0, 255); // E
    rgb_matrix_set_color(23, 0, 0, 255); // F
    rgb_matrix_set_color(17, 0, 0, 255); // C

    rgb_matrix_set_color(48, 255, 255, 0); // Q

    rgb_matrix_set_color(28, 255, 255, 0); // L

    rgb_matrix_set_color(0, 0, 255, 0); // Space
    rgb_matrix_set_color(84, 0, 255, 0); // Lcontrol
    rgb_matrix_set_color(82, 0, 255, 0); // LShift

    rgb_matrix_set_color(35, 255, 0, 0); // Enter up
    rgb_matrix_set_color(36, 255, 0, 0); // Enter down
}


/**
 *  No Man's Sky profile
 */ 

void profile_nms(void) {
    rgb_matrix_set_color(47, 0, 0, 255); // W
    rgb_matrix_set_color(20, 0, 0, 255); // A
    rgb_matrix_set_color(21, 0, 0, 255); // S
    rgb_matrix_set_color(22, 0, 0, 255); // D

    rgb_matrix_set_color(46, 255, 0, 0); // E
    rgb_matrix_set_color(17, 255, 0, 0); // C
    rgb_matrix_set_color(23, 255, 0, 0); // F
    rgb_matrix_set_color(45, 255, 0, 0); // R
    rgb_matrix_set_color(18, 255, 0, 0); // X

    rgb_matrix_set_color(0, 0, 0, 255); // Space
    rgb_matrix_set_color(84, 0, 255, 0); // Lcontrol
    rgb_matrix_set_color(82, 0, 255, 0); // LShift

    rgb_matrix_set_color(48, 255, 255, 0); // Q

    rgb_matrix_set_color(80, 255, 0, 0); // TAB
    rgb_matrix_set_color(44, 0, 255, 0); // T
    rgb_matrix_set_color(24, 0, 255, 0); // G
    rgb_matrix_set_color(43, 0, 255, 0); // de: Z, us: Y
    rgb_matrix_set_color(25, 0, 255, 0); // H

    rgb_matrix_set_color(13, 0, 0, 255); // M
}


/**
 *  ASKA profile
 */ 

void profile_aska(void) {
    rgb_matrix_set_color(47, 255, 187, 0); // W
    rgb_matrix_set_color(20, 255, 187, 0); // A
    rgb_matrix_set_color(21, 255, 187, 0); // S
    rgb_matrix_set_color(22, 255, 187, 0); // D

    rgb_matrix_set_color(48, 0, 0, 255); // Q

    rgb_matrix_set_color(46, 0, 0, 255); // E
    rgb_matrix_set_color(45, 0, 0, 255); // R
    rgb_matrix_set_color(23, 0, 0, 255); // F
    rgb_matrix_set_color(17, 0, 0, 255); // C
    rgb_matrix_set_color(18, 0, 0, 255); // X

    rgb_matrix_set_color(44, 0, 255, 0); // T
    rgb_matrix_set_color(24, 0, 255, 0); // G
    rgb_matrix_set_color(43, 0, 255, 0); // de: Z, us: Y
    rgb_matrix_set_color(25, 0, 255, 0); // H

    rgb_matrix_set_color(16, 255, 0, 0); // V

    rgb_matrix_set_color(41, 255, 0, 0); // I

    rgb_matrix_set_color(13, 255, 0, 0); // M

    rgb_matrix_set_color(0, 255, 0, 0); // Space
    rgb_matrix_set_color(84, 0, 0, 255); // Lcontrol
    rgb_matrix_set_color(82, 0, 0, 255); // LShift

    rgb_matrix_set_color(80, 255, 0, 0); // TAB
}

/**
 *  Elden Ring profile
 */ 

void profile_elden_ring(void) {
    rgb_matrix_set_color(47, 0, 255, 0); // W
    rgb_matrix_set_color(20, 0, 255, 0); // A
    rgb_matrix_set_color(21, 0, 255, 0); // S
    rgb_matrix_set_color(22, 0, 255, 0); // D

    rgb_matrix_set_color(46, 0, 0, 255); // E
    rgb_matrix_set_color(45, 0, 0, 255); // R
    rgb_matrix_set_color(23, 0, 0, 255); // F
    rgb_matrix_set_color(24, 0, 0, 255); // G

    rgb_matrix_set_color(48, 255, 0, 0); // Q

    rgb_matrix_set_color(18, 255, 0, 0); // X

    rgb_matrix_set_color(0, 255, 0, 0); // Space

    rgb_matrix_set_color(82, 255, 0, 0); // LShift

    rgb_matrix_set_color(86, 255, 0, 0); // Alt
}


/**
 *  Palworld profile
 */ 

void profile_palworld(void) {
    // Palworld
    rgb_matrix_set_color(47, 0, 0, 255); // W
    rgb_matrix_set_color(20, 0, 0, 255); // A
    rgb_matrix_set_color(21, 0, 0, 255); // S
    rgb_matrix_set_color(22, 0, 0, 255); // D

    rgb_matrix_set_color(0, 255, 0, 0); // Space

    rgb_matrix_set_color(46, 255, 0, 0); // E
    rgb_matrix_set_color(45, 255, 0, 0); // R
    rgb_matrix_set_color(23, 255, 0, 0); // F
    rgb_matrix_set_color(17, 255, 0, 0); // C

    rgb_matrix_set_color(48, 255, 187, 0); // Q


    rgb_matrix_set_color(84, 255, 0, 0); // Lcontrol
    rgb_matrix_set_color(82, 255, 0, 0); // LShift

    rgb_matrix_set_color(49, 0, 255, 0); // 1
    rgb_matrix_set_color(50, 255, 0, 0); // 2
    rgb_matrix_set_color(51, 0, 255, 0); // 3
    rgb_matrix_set_color(52, 0, 0, 255); // 4
}


/**
 *  Profile Manager for comparing existing and new profile specific values and writing them to eeprom if needed
 */ 

void profile_manager(int layer2_key, enum lighting_profiles lighting_profile) {

    user_config.raw = eeconfig_read_user();

    switch (lighting_profile) {

        case PROFILE_WINDOWS:
            if (layer2_key != user_config.last_pressed_layer2) {
                user_config.last_pressed_layer2 = layer2_key;
                eeconfig_update_user(user_config.raw);
            }
            if (lighting_profile != user_config.current_profile) {
                user_config.current_profile = lighting_profile;
                eeconfig_update_user(user_config.raw);
            }
            break;

        case PROFILE_MINECRAFT:
            if (layer2_key != user_config.last_pressed_layer2) {
                user_config.last_pressed_layer2 = layer2_key;
                eeconfig_update_user(user_config.raw);
            }
            if (lighting_profile != user_config.current_profile) {
                user_config.current_profile = lighting_profile;
                eeconfig_update_user(user_config.raw);
            }
            break;

        case PROFILE_NMS:
            if (layer2_key != user_config.last_pressed_layer2) {
                user_config.last_pressed_layer2 = layer2_key;
                eeconfig_update_user(user_config.raw);
            }
            if (lighting_profile != user_config.current_profile) {
                user_config.current_profile = lighting_profile;
                eeconfig_update_user(user_config.raw);
            }
            break;

        case PROFILE_ASKA:
            if (layer2_key != user_config.last_pressed_layer2) {
                user_config.last_pressed_layer2 = layer2_key;
                eeconfig_update_user(user_config.raw);
            }
            if (lighting_profile != user_config.current_profile) {
                user_config.current_profile = lighting_profile;
                eeconfig_update_user(user_config.raw);
            }
            break;

        case PROFILE_ELDEN_RING:
            if (layer2_key != user_config.last_pressed_layer2) {
                user_config.last_pressed_layer2 = layer2_key;
                eeconfig_update_user(user_config.raw);
            }
            if (lighting_profile != user_config.current_profile) {
                user_config.current_profile = lighting_profile;
                eeconfig_update_user(user_config.raw);
            }
            break;
        
        case PROFILE_PALWORLD:
            if (layer2_key != user_config.last_pressed_layer2) {
                user_config.last_pressed_layer2 = layer2_key;
                eeconfig_update_user(user_config.raw);
            }
            if (lighting_profile != user_config.current_profile) {
                user_config.current_profile = lighting_profile;
                eeconfig_update_user(user_config.raw);
            }
            break;
    }
}

/**
 *  Config Updater for comparing existing and new config values and writing them to eeprom if needed
 */ 

void update_config(enum config_types config_type, int config_value) {

    user_config.raw = eeconfig_read_user();

    switch (config_type) {

        case 0:
            // Last Pressed F13 F18
            if (config_value != user_config.last_pressed_f13_f18) {
                user_config.last_pressed_f13_f18 = config_value;
                eeconfig_update_user(user_config.raw);
            }
            break;

        case 1:
            // Last Pressed F19 F24
            if (config_value != user_config.last_pressed_f19_f24) {
                user_config.last_pressed_f19_f24 = config_value;
                eeconfig_update_user(user_config.raw);
            }
            break;
    }
}


/**
 *  Set colors based on layer state and game / general profile chosen
 */

bool rgb_matrix_indicators_user(void) {

    user_config.raw = eeconfig_read_user();

    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(81, 255, 0, 0);  // Set caps lock to red
    }

    switch (biton32(layer_state)) {


        /**
        *  Layer 0 (main layer)
        */
        
        case 0:
            user_config.raw = eeconfig_read_user();
            // Set ESC to red
            rgb_matrix_set_color(78, 255, 0, 0);
        switch (user_config.current_profile) {
            case PROFILE_WINDOWS:
                profile_windows();
                break;

            case PROFILE_MINECRAFT:
                profile_minecraft();
                break;

            case PROFILE_NMS:
                profile_nms();
                break;

            case PROFILE_ASKA:
                profile_aska();
                break;

            case PROFILE_ELDEN_RING:
                profile_elden_ring();
                break;

            case PROFILE_PALWORLD:
                profile_palworld();
                break;

            default:
                break;
        }
        break;


        /**
        *  Layer 1 (Status and Control of some things alongside F13 to F24 for macros)
        */

        case 1:
            user_config.raw = eeconfig_read_user();
            // Set Insert and Pos1 Key Color (Mapped to RM_VALD and RM_VALU for changing rgb lighting brightness)
            rgb_matrix_set_color(65, 0, 255, 0);
            rgb_matrix_set_color(64, 0, 255, 0);

            // Set ESC to red (Mapped to EE_CLR, can be used to go to bootloader when long pressed)
            rgb_matrix_set_color(78, 255, 0, 0);

            // Set colors for F1 to F6 (Media Control)
            rgb_matrix_set_color(77, 255, 0, 0);
            rgb_matrix_set_color(76, 0, 255, 0);
            rgb_matrix_set_color(75, 0, 255, 0);
            rgb_matrix_set_color(74, 255, 0, 0);
            rgb_matrix_set_color(73, 0, 0, 255);
            rgb_matrix_set_color(72, 0, 0, 255);

            // Set all F13-F18 to red (rgb ids are the keys printed on the keyboard, so not F13-F18 but 1-6 in my case)
            rgb_matrix_set_color(49, 255, 0, 0);
            rgb_matrix_set_color(50, 255, 0, 0);
            rgb_matrix_set_color(51, 255, 0, 0);
            rgb_matrix_set_color(52, 255, 0, 0);
            rgb_matrix_set_color(53, 255, 0, 0);
            rgb_matrix_set_color(54, 255, 0, 0);

            // Set all F19-F24 to blue (set to some letters on the keyboard used for macros)
            rgb_matrix_set_color(17, 0, 0, 255);
            rgb_matrix_set_color(13, 0, 0, 255);
            rgb_matrix_set_color(21, 0, 0, 255);
            rgb_matrix_set_color(22, 0, 0, 255);
            rgb_matrix_set_color(44, 0, 0, 255);
            rgb_matrix_set_color(40, 0, 0, 255);

            // Set color for last pressed key

            if (user_config.last_pressed_f13_f18 != 255) {
                rgb_matrix_set_color(user_config.last_pressed_f13_f18, 0, 255, 0);
            }

            if (user_config.last_pressed_f19_f24 != 255) {
                rgb_matrix_set_color(user_config.last_pressed_f19_f24, 0, 255, 0);
            }

            rgb_matrix_set_color(74, is_muted ? 0 : 255, is_muted ? 255 : 0, is_muted ? 0 : 0);

            // Color spacebar based on music status
            rgb_matrix_set_color(0, music_active ? 0 : 255, music_active ? 255 : 0, music_active ? 0 : 0);
            rgb_matrix_set_color(77, music_active ? 0 : 255, music_active ? 255 : 0, music_active ? 0 : 0);
            break;


        /**
        *  Layer 2 (Layer to switch between game and general profiles)
        */

        case 2:
            user_config.raw = eeconfig_read_user();
            rgb_matrix_set_color(13, 0, 0, 255); // Key: N
            rgb_matrix_set_color(14, 0, 0, 255); // Key: M
            rgb_matrix_set_color(20, 0, 0, 255); // Key: A
            rgb_matrix_set_color(46, 0, 0, 255); // Key: E
            rgb_matrix_set_color(39, 0, 0, 255); // Key: P
            rgb_matrix_set_color(58, 0, 0, 255); // Key: 0


            // Highlight last pressed key on Layer 2 (Red)
            if (user_config.last_pressed_layer2 != 255) {
                rgb_matrix_set_color(user_config.last_pressed_layer2, 255, 0, 0);
            } else {
                user_config.last_pressed_layer2 = 58;
                rgb_matrix_set_color(user_config.last_pressed_layer2, 255, 0, 0);
            }

            break;
    }
    return false;
}

/**
 * Saves the last pressed F-key & music status
 */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    dprintf("Key pressed: row=%d, col=%d, keycode=%d\n",
        record->event.key.row, record->event.key.col, keycode);

    if (record->event.pressed) {

        switch (keycode) {

            /**
            *  F13 to F24 keys
            */
            case KC_F13:
                config_type = LAST_PRESSED_F13_F18;
                last_pressed_f13_f18 = 49;
                update_config(config_type, last_pressed_f13_f18);
                break;
            case KC_F14:
                config_type = LAST_PRESSED_F13_F18;
                last_pressed_f13_f18 = 50;
                update_config(config_type, last_pressed_f13_f18);
                break;
            case KC_F15:
                config_type = LAST_PRESSED_F13_F18;
                last_pressed_f13_f18 = 51;
                update_config(config_type, last_pressed_f13_f18);
                break;
            case KC_F16:
                config_type = LAST_PRESSED_F13_F18;
                last_pressed_f13_f18 = 52;
                update_config(config_type, last_pressed_f13_f18);
                break;
            case KC_F17:
                config_type = LAST_PRESSED_F13_F18;
                last_pressed_f13_f18 = 53;
                update_config(config_type, last_pressed_f13_f18);
                break;
            case KC_F18:
                config_type = LAST_PRESSED_F13_F18;
                last_pressed_f13_f18 = 54;
                update_config(config_type, last_pressed_f13_f18);
                break;

            case KC_F19:
                config_type = LAST_PRESSED_F19_F24;
                last_pressed_f19_f24 = 17;
                update_config(config_type, last_pressed_f19_f24);
                break;
            case KC_F20:
                config_type = LAST_PRESSED_F19_F24;
                last_pressed_f19_f24 = 13;
                update_config(config_type, last_pressed_f19_f24);
                break;
            case KC_F21:
                config_type = LAST_PRESSED_F19_F24;
                last_pressed_f19_f24 = 21;
                update_config(config_type, last_pressed_f19_f24);
                break;
            case KC_F22:
                config_type = LAST_PRESSED_F19_F24;
                last_pressed_f19_f24 = 22;
                update_config(config_type, last_pressed_f19_f24);
                break;
            case KC_F23:
                config_type = LAST_PRESSED_F19_F24;
                last_pressed_f19_f24 = 44;
                update_config(config_type, last_pressed_f19_f24);
                break;
            case KC_F24:
                config_type = LAST_PRESSED_F19_F24;
                last_pressed_f19_f24 = 40;
                update_config(config_type, last_pressed_f19_f24);
                break;

            /**
            *  Game and general profile keys
            */
            case KC_PROFILE_MINECRAFT:
                profile_manager(13, PROFILE_MINECRAFT);
                break;

            case KC_PROFILE_NMS:
                profile_manager(14, PROFILE_NMS);
                break;

            case KC_PROFILE_ASKA:
                profile_manager(20, PROFILE_ASKA);
                break;

            case KC_PROFILE_ELDEN_RING:
                profile_manager(46, PROFILE_ELDEN_RING);
                break;

            case KC_PROFILE_PALWORLD:
                profile_manager(39, PROFILE_PALWORLD);
                break;

            case KC_PROFILE_WINDOWS:
                profile_manager(58, PROFILE_WINDOWS);
                break;

        }


        /**
        *  Media Play and Mute keys
        */
        
        if (keycode == KC_MPLY) {
            music_active = !music_active;
        }

        if (keycode == KC_MUTE) {
            is_muted = !is_muted;
        }
    }
    return true;
}

/**
*  Function for initializing EEPROM
*/

void eeconfig_init_user(void) {
    user_config.raw = 0;
    user_config.current_profile = PROFILE_WINDOWS;
    user_config.last_pressed_f13_f18 = 255;
    user_config.last_pressed_f19_f24 = 255;
    user_config.last_pressed_layer2 = 58;
}
