#pragma once

#define _SETTINGS_FLASH_RAW {                                                                \
                              0,                                                             \
                              0,                                                             \
                              0,                                                             \
                              0,                                                             \
                              0,                                                             \
                              0,                                                             \
                              26, 0x03,                                                      \
                              'E', 0, '8', 0, '1', 0, 'C', 0,                                \
                              '1', 0, '9', 0, '7', 0, '9', 0,                                \
                              '2', 0, '8', 0, '0', 0, '1', 0                                 \
                            }  // reserving space because erase block is block 32-word (32-bytes as only low bytes are used)
#define _SETTINGS_FLASH_LOCATION 0x1FE0
const uint8_t _SETTINGS_PROGRAM[] __at(_SETTINGS_FLASH_LOCATION) = _SETTINGS_FLASH_RAW;

typedef struct {
    uint8_t Reserved[6];
    uint8_t UsbSerialLength;
    uint8_t UsbSerialType;
    uint8_t UsbSerialValue[24];
} SettingsRecord;

SettingsRecord Settings;


/** Initializes settings. */
void settings_init();

/** Saves settings to EEPROM. */
void settings_save();
