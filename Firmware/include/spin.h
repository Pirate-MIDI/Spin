/*
 *  spin.h
 *
 *  Created on: Nov 20, 2024
 *  Author: samspencer
 */
#ifndef SPIN_H_
#define SPIN_H_

#include "stdint.h"
//#include <Arduino.h>
#include <EEPROM.h>
#include "midi.h"
#include <Adafruit_TinyUSB.h>
#include "led_bar.h"
#include "mcp3008.h"
#include "continuous_pot.h"
#include "esp32_manager.h"
#include "hardware_def.h"

// Version
#define XSTR(s) STR(s)
#define STR(s) #s

#define EEPROM_CONFIG_SIZE 16
#define EEPROM_PRESET_SIZE 2
#define NUM_PRESETS 128
#define EEPROM_SIZE NUM_PRESETS *EEPROM_PRESET_SIZE + EEPROM_CONFIG_SIZE

#define DEVICE_CONFIGURED_VALUE 114

#define TIP_BUTTON_INDEX  0
#define RING_BUTTON_INDEX 1

#define DEFAULT_LED_BRIGHTNESS	20




// MIDI DEFINITIONS //
// Control Change Numbers

#define SAVE_CURRENT_PRESET_CC  23
#define PRESET_UP_CC            24
#define PRESET_DOWN_CC          25
#define GO_TO_PRESET_CC         26

// Control Change Values


#define USB_DEVICE_MODEL_RESPONSE_STRING "Spin"


// States
typedef struct
{
	uint32_t presetId;
} Preset;

typedef struct
{
	uint8_t bootState;
  	uint8_t midiInChannel;
  	uint8_t currentBank;
	uint32_t profileId;
	uint8_t ledBrightness;
	Esp32Config esp32Config;
	uint8_t blank[100];
} GlobalSettings;

extern Preset presets[];
extern GlobalSettings globalSettings;

extern char cdcRxBuf[];

extern LEDBar ledBars[];
extern SPIClass * hspi;

extern CONTPOT pots[];


// Function prototypes
//-------------------- SYSTEM & EVENT HANDLERS --------------------//
void bootCheck();
void newDeviceConfig();
void standardBoot();

void getFlashUid(char* str);

void initPins();
void initLedBars();
//-------------------- PRESET MANAGEMENT --------------------//
void readCurrentPreset();
void saveCurrentPreset();
void readGlobalConfig();
void saveGlobalConfig();


//-------------------- CONTROL INTERFACE --------------------//
void readPots();


//-------------------- MIDI CALLBACK HANDLERS --------------------//
void assignMidiCallbacks();

#endif // SPIN_H_ //