// Copyright 2025 s3tupw1zard (@s3tupw1zard)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "eeconfig.h"
#include "rgb_matrix.h"

enum lighting_profiles {
    PROFILE_WINDOWS = 0,
    PROFILE_MINECRAFT,
    PROFILE_NMS,
    PROFILE_ASKA,
    PROFILE_ELDEN_RING,
    PROFILE_PALWORLD,
};

enum custom_keycodes {
    KC_PROFILE_WINDOWS = SAFE_RANGE,
    KC_PROFILE_MINECRAFT,
    KC_PROFILE_NMS,
    KC_PROFILE_ASKA,
    KC_PROFILE_ELDEN_RING,
    KC_PROFILE_PALWORLD,
    KC_SAVE_USER_SETTINGS,
    KC_LOAD_USER_SETTINGS
};

enum config_types {
    CURRENT_RGB_BRIGHTNESS,
    CURRENT_RGB_SPEED,
    NEW_PROFILE,
    LAST_PRESSED_F13_F18,
    LAST_PRESSED_F19_F24,
    LAST_PRESSED_LAYER2
};

static enum lighting_profiles current_profile = PROFILE_WINDOWS;

static enum config_types config_type;


typedef struct {
    uint8_t current_rgb_brightness;
    uint8_t current_rgb_speed;
    uint8_t current_profile;
    uint8_t last_pressed_f13_f18;
    uint8_t last_pressed_f19_f24;
    uint8_t last_pressed_layer2;
} keyboard_config_t;

keyboard_config_t keyboard_config;


// Global variables for storing the last pressed keys
static uint8_t last_pressed_f13_f18;
static uint8_t last_pressed_f19_f24;
static uint8_t last_pressed_layer2;
static uint8_t current_rgb_brightness;
static uint8_t current_rgb_speed;

static int profile_count = 6; // When using this, make sure to subtract 1 from the actual count, since we are counting from 0

static bool is_muted = false; // Status of KC_MUTE (Mute Sound Toggle)
static bool music_active = false; // Status of KC_MPLY (Music Toggle)

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


// Windows Profile

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


// Minecraft Profile

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


// NMS Profile

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


// ASKA Profile

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

// Elden Ring Profile

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


// Palworld Profile

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

// Update EEPROM with current values

void update_config(void) {
    eeprom_update_block(&keyboard_config, (void*)EECONFIG_USER, sizeof(keyboard_config_t));
}


// Initialize EEPROM

void init_config(void) {
    keyboard_config.current_rgb_brightness = 110;
    keyboard_config.current_rgb_speed = 110;
    keyboard_config.current_profile = PROFILE_WINDOWS;
    keyboard_config.last_pressed_f13_f18 = 255;
    keyboard_config.last_pressed_f19_f24 = 255;
    keyboard_config.last_pressed_layer2 = 58;

    update_config();
}


// Validate EEPROM

bool validate_eeprom(void) {
    if (keyboard_config.last_pressed_f13_f18 == 0xFF) {
        return false;
    }
    return true;
}

// Load user settings from EEPROM

void read_config(void) {
    if (validate_eeprom()) {
        eeprom_read_block(&keyboard_config, (void*)EECONFIG_USER, sizeof(keyboard_config_t));
    } else {
        init_config();
    }
}


// Check if config values need to be updated and update them if needed

void update_config_if_needed(int config_type, int config_value) {

    /*
    config types:
    1: current_rgb_brightness
    2: current_rgb_speed
    3: last_profile
    4: last_pressed_function_01_06
    5: last_pressed_function_07_12
    6: last_pressed_layer2
    */

    switch (config_type) {

        case CURRENT_RGB_BRIGHTNESS:
            if (config_value != keyboard_config.current_rgb_brightness) {
                keyboard_config.current_rgb_brightness = config_value;
                eeprom_update_byte((uint8_t*) (EECONFIG_USER + offsetof(keyboard_config_t, current_rgb_brightness)), keyboard_config.current_rgb_brightness);
            }
            break;

        case CURRENT_RGB_SPEED:
            if (config_value != keyboard_config.current_rgb_speed) {
                keyboard_config.current_rgb_speed = config_value;
                eeprom_update_byte((uint8_t*) (EECONFIG_USER + offsetof(keyboard_config_t, current_rgb_speed)), keyboard_config.current_rgb_speed);
            }
            break;

        case NEW_PROFILE:
            if (config_value != keyboard_config.current_profile) {
                keyboard_config.current_profile = config_value;
                eeprom_update_byte((uint8_t*) (EECONFIG_USER + offsetof(keyboard_config_t, current_profile)), keyboard_config.current_profile);
            }
        break;

        case LAST_PRESSED_F13_F18:
            if (config_value != keyboard_config.last_pressed_f13_f18) {
                keyboard_config.last_pressed_f13_f18 = config_value;
                eeprom_update_byte((uint8_t*) (EECONFIG_USER + offsetof(keyboard_config_t, last_pressed_f13_f18)), keyboard_config.last_pressed_f13_f18);
            }
            break;

        case LAST_PRESSED_F19_F24:
            if (config_value != keyboard_config.last_pressed_f19_f24) {
                keyboard_config.last_pressed_f19_f24 = config_value;
                eeprom_update_byte((uint8_t*) (EECONFIG_USER + offsetof(keyboard_config_t, last_pressed_f19_f24)), keyboard_config.last_pressed_f19_f24);
            }
            break;

        case LAST_PRESSED_LAYER2:
            if (config_value != keyboard_config.last_pressed_layer2) {
                keyboard_config.last_pressed_layer2 = config_value;
                eeprom_update_byte((uint8_t*) (EECONFIG_USER + offsetof(keyboard_config_t, last_pressed_layer2)), keyboard_config.last_pressed_layer2);
            }
            break;
    }
}


// Set default user profile if profile is not yet set
void init_user_profile(void) {
    if (keyboard_config.current_profile == 0 || keyboard_config.current_profile < (profile_count - 1)) {
        current_profile = PROFILE_WINDOWS;
        config_type = NEW_PROFILE;
        update_config_if_needed(config_type, current_profile);
        profile_windows();
    }
}

/**
 * Adjust RGB matrix colors for special functions
 */
bool rgb_matrix_indicators_user(void) {

    read_config();

    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(81, 255, 0, 0);  // Set caps lock to red
    }

    switch (biton32(layer_state)) {
        case 0:
            // Set ESC to red
            rgb_matrix_set_color(78, 255, 0, 0);
        switch (current_profile) {
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
                init_user_profile();
                break;
        }
        break;

        case 1:
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
            if (keyboard_config.last_pressed_f13_f18 != 255) {
                rgb_matrix_set_color(keyboard_config.last_pressed_f13_f18, 0, 255, 0);
            }
            if (keyboard_config.last_pressed_f19_f24 != 255) {
                rgb_matrix_set_color(keyboard_config.last_pressed_f19_f24, 0, 255, 0);
            }

            rgb_matrix_set_color(74, is_muted ? 0 : 255, is_muted ? 255 : 0, is_muted ? 0 : 0);

            // Color spacebar based on music status
            rgb_matrix_set_color(0, music_active ? 0 : 255, music_active ? 255 : 0, music_active ? 0 : 0);
            rgb_matrix_set_color(77, music_active ? 0 : 255, music_active ? 255 : 0, music_active ? 0 : 0);
            break;
        case 2:
            rgb_matrix_set_color(13, 0, 0, 255); // Key: N
            rgb_matrix_set_color(14, 0, 0, 255); // Key: M
            rgb_matrix_set_color(20, 0, 0, 255); // Key: A
            rgb_matrix_set_color(46, 0, 0, 255); // Key: E
            rgb_matrix_set_color(39, 0, 0, 255); // Key: P
            rgb_matrix_set_color(58, 0, 0, 255); // Key: 0

            // Highlight last pressed key on Layer 2 (Red)
            if (keyboard_config.last_pressed_layer2 != 255) {
                rgb_matrix_set_color(keyboard_config.last_pressed_layer2, 255, 0, 0);
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
            case KC_F13:
                config_type = LAST_PRESSED_F13_F18;
                last_pressed_f13_f18 = 49;
                update_config_if_needed(config_type, last_pressed_f13_f18);
                read_config();
                break;
            case KC_F14:
                config_type = LAST_PRESSED_F13_F18;
                last_pressed_f13_f18 = 50;
                update_config_if_needed(config_type, last_pressed_f13_f18);
                read_config();
                break;
            case KC_F15:
                config_type = LAST_PRESSED_F13_F18;
                last_pressed_f13_f18 = 51;
                update_config_if_needed(config_type, last_pressed_f13_f18);
                read_config();
                break;
            case KC_F16:
                config_type = LAST_PRESSED_F13_F18;
                last_pressed_f13_f18 = 52;
                update_config_if_needed(config_type, last_pressed_f13_f18);
                read_config();
                break;
            case KC_F17:
                config_type = LAST_PRESSED_F13_F18;
                last_pressed_f13_f18 = 53;
                update_config_if_needed(config_type, last_pressed_f13_f18);
                read_config();
                break;
            case KC_F18:
                config_type = LAST_PRESSED_F13_F18;
                last_pressed_f13_f18 = 54;
                update_config_if_needed(config_type, last_pressed_f13_f18);
                read_config();
                break;

            case KC_F19:
                config_type = LAST_PRESSED_F19_F24;
                last_pressed_f19_f24 = 17;
                update_config_if_needed(config_type, last_pressed_f19_f24);
                read_config();
                break;
            case KC_F20:
                config_type = LAST_PRESSED_F19_F24;
                last_pressed_f19_f24 = 13;
                update_config_if_needed(config_type, last_pressed_f19_f24);
                read_config();
                break;
            case KC_F21:
                config_type = LAST_PRESSED_F19_F24;
                last_pressed_f19_f24 = 21;
                update_config_if_needed(config_type, last_pressed_f19_f24);
                read_config();
                break;
            case KC_F22:
                config_type = LAST_PRESSED_F19_F24;
                last_pressed_f19_f24 = 22;
                update_config_if_needed(config_type, last_pressed_f19_f24);
                read_config();
                break;
            case KC_F23:
                config_type = LAST_PRESSED_F19_F24;
                last_pressed_f19_f24 = 44;
                update_config_if_needed(config_type, last_pressed_f19_f24);
                read_config();
                break;
            case KC_F24:
                config_type = LAST_PRESSED_F19_F24;
                last_pressed_f19_f24 = 40;
                update_config_if_needed(config_type, last_pressed_f19_f24);
                read_config();
                break;

            case KC_PROFILE_MINECRAFT:
                config_type = NEW_PROFILE;
                current_profile = PROFILE_MINECRAFT;
                update_config_if_needed(config_type, current_profile);
                config_type = LAST_PRESSED_LAYER2;
                last_pressed_layer2 = 13;
                update_config_if_needed(config_type, last_pressed_layer2);
                read_config();
                break;

            case KC_PROFILE_NMS:
                config_type = NEW_PROFILE;
                current_profile = PROFILE_NMS;
                update_config_if_needed(config_type, current_profile);
                config_type = LAST_PRESSED_LAYER2;
                last_pressed_layer2 = 14;
                update_config_if_needed(config_type, last_pressed_layer2);
                read_config();
                break;

            case KC_PROFILE_ASKA:
                config_type = NEW_PROFILE;
                current_profile = PROFILE_ASKA;
                update_config_if_needed(config_type, current_profile);
                config_type = LAST_PRESSED_LAYER2;
                last_pressed_layer2 = 20;
                update_config_if_needed(config_type, last_pressed_layer2);
                read_config();
                break;

            case KC_PROFILE_ELDEN_RING:
                config_type = NEW_PROFILE;
                current_profile = PROFILE_ELDEN_RING;
                update_config_if_needed(config_type, current_profile);
                config_type = LAST_PRESSED_LAYER2;
                last_pressed_layer2 = 46;
                update_config_if_needed(config_type, last_pressed_layer2);
                read_config();
                break;

            case KC_PROFILE_PALWORLD:
                config_type = NEW_PROFILE;
                current_profile = PROFILE_PALWORLD;
                update_config_if_needed(config_type, current_profile);
                config_type = LAST_PRESSED_LAYER2;
                last_pressed_layer2 = 39;
                update_config_if_needed(config_type, last_pressed_layer2);
                read_config();
                break;

            case KC_PROFILE_WINDOWS:
                config_type = NEW_PROFILE;
                current_profile = PROFILE_WINDOWS;
                update_config_if_needed(config_type, current_profile);
                config_type = LAST_PRESSED_LAYER2;
                last_pressed_layer2 = 58;
                update_config_if_needed(config_type, last_pressed_layer2);
                read_config();
                break;

            case KC_SAVE_USER_SETTINGS:
                current_rgb_brightness = rgb_matrix_get_val();
                current_rgb_speed = rgb_matrix_get_speed();

                update_config_if_needed(CURRENT_RGB_BRIGHTNESS, current_rgb_brightness);
                update_config_if_needed(CURRENT_RGB_SPEED, current_rgb_speed);
                break;

            case KC_LOAD_USER_SETTINGS:
                read_config();
                break;

        }

        // Toggle music status when KC_MPLY is pressed
        if (keycode == KC_MPLY) {
            music_active = !music_active;
        }

        if (keycode == KC_MUTE) {
            is_muted = !is_muted;
        }
    }
    return true;
}

/*
void keyboard_post_init_user(void) {
    // rgb_matrix_mode(RGB_MATRIX_RAINBOW_BEACON); // Set the effect to Rainbow Beacon
    // rgb_matrix_set_speed(80); // Set speed to 100 (0 = slow, 255 = fast)

    // hsv_t hsv = rgb_matrix_get_hsv();
    // hsv.v = 130;  // Set brightness
    // rgb_matrix_sethsv(hsv.h, hsv.s, hsv.v);
}
*/
