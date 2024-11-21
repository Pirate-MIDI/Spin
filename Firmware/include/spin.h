/*
 *  click.h
 *
 *  Created on: Dec 8, 2022
 *  Author: samspencer
 */
#ifndef ULOOP_H_
#define ULOOP_H_

#include "stdint.h"
//#include <Arduino.h>
#include <EEPROM.h>
#include "midi.h"
#include <Adafruit_TinyUSB.h>
#include "led_bar.h"
#include "mcp3008.h"
#include "continuous_pot.h"

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

/* HARDWARE DEFINITIONS */
// PINS
#define LED_DATA_PIN					15

#define MIDI_OUT_PIN        		43
#define MIDI_IN_PIN             	44

#define NUM_LEDS              	168
#define NUM_LEDS_PER_RING			21
#define NUM_POTS              	8

#define DEFAULT_LED_BRIGHTNESS	20

#define SPI_SCK_PIN			8
#define SPI_MOSI_PIN			10
#define SPI_MISO_PIN			46

#define ADC_CS1_PIN           	0
#define ADC_CS2_PIN           	3

#define POT1A_CHANNEL   2
#define POT1B_CHANNEL   3
#define POT2A_CHANNEL   4
#define POT2B_CHANNEL   5
#define POT3A_CHANNEL   2
#define POT3B_CHANNEL   3
#define POT4A_CHANNEL   4
#define POT4B_CHANNEL   5
#define POT5A_CHANNEL   0
#define POT5B_CHANNEL   1
#define POT6A_CHANNEL   6
#define POT6B_CHANNEL   7
#define POT7A_CHANNEL   0
#define POT7B_CHANNEL   1
#define POT8A_CHANNEL   6
#define POT8B_CHANNEL   7


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
  	uint8_t currentPreset;
	uint32_t profileId;
	uint8_t ledBrightness;
	uint8_t blank[100];
} Config;

extern Preset presets[];
extern Config globalConfig;

extern char cdcRxBuf[];


// Function prototypes
//-------------------- SYSTEM & EVENT HANDLERS --------------------//
void bootCheck();
void newDeviceConfig();
void standardBoot();

void getFlashUid(char* str);

void initPins();
//-------------------- PRESET MANAGEMENT --------------------//
void readCurrentPreset();
void saveCurrentPreset();
void readGlobalConfig();
void saveGlobalConfig();

//-------------------- MIDI CALLBACK HANDLERS --------------------//
void assignMidiCallbacks();

#endif // ULOOP_H_ //