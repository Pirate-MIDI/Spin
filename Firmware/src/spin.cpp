/*
 *  spin.c
 *
 *  Created on: Dec 8, 2022
 *  Author: samspencer
 */
#include "spin.h"
#include "string.h"
#include "midi_handling.h"
#include <Preferences.h>
#include <nvs_flash.h>
#include "led_bar.h"

CRGB leds[NUM_LEDS];

Preset presets[NUM_PRESETS];
Config globalConfig;
Preset* currentPresetPtr = &presets[globalConfig.currentPreset];
Preferences storage;

void softwareReset();

void setChannel(uint8_t channel, uint8_t channelState);

void handleControlChange(MidiInterfaceType interface, uint8_t channel, uint8_t number, uint8_t value);
void handleProgramChange(MidiInterfaceType interface, uint8_t channel, uint8_t number);
void handleSysEx(MidiInterfaceType interface, uint8_t* array, unsigned size);

//--------------------- SYSTEM & EVENT HANDLERS ---------------------//
void bootCheck()
{
	// Check if the device has been configured
	Serial.println("Checking boot state...");
	storage.begin("global", false);
	size_t len = storage.getBytes("config", &globalConfig, sizeof(Config));
	storage.end();
	// Uncomment to force a new device configuration
	globalConfig.bootState = 0;
	if(globalConfig.bootState != DEVICE_CONFIGURED_VALUE || len != sizeof(Config))
	{
		Serial.println("Configuring new device...");
		newDeviceConfig();
	}
	else
	{
		Serial.println("Performing standard boot...");
		standardBoot();
	}
}

// Configures the device to a factory state
void newDeviceConfig()
{
	// Configure default values for global settings
	globalConfig.bootState = DEVICE_CONFIGURED_VALUE;
	globalConfig.currentPreset = 0;
	globalConfig.midiInChannel = MIDI_CHANNEL_OMNI;
	globalConfig.profileId = 0;
	globalConfig.ledBrightness = DEFAULT_LED_BRIGHTNESS;
	FastLED.setBrightness(DEFAULT_LED_BRIGHTNESS);
	
	
	// Erase the NVS partition and initialize
	nvs_flash_erase(); 
	nvs_flash_init();

	// Save the default config to eeprom
	storage.begin("global", false);
	size_t len = storage.putBytes("config", &globalConfig, sizeof(Config));
	Serial.printf("Wrote global config to storage with size %d.\n", len);

	// Save the default presets to eeprom
	storage.begin("presets", false);
	len = 0;
	
	storage.end();
	Serial.printf("Wrote presets (128*%d) to storage with size %d.\n", sizeof(Preset), len);
}

void standardBoot()
{
	FastLED.setBrightness(globalConfig.ledBrightness);
	// Read the preset data into the struct array
	storage.getBytes("presets", presets, sizeof(Preset)*NUM_PRESETS);
}

void softwareReset()
{
  ESP.restart();
}

void cdcRxHandler(uint16_t len)
{
  char *token;
#ifdef CONFIG_DIAGNOSTIC
  diagnosticTest(len);
#endif
}

// Returns the UID of the NOR flash chip
// str must have at least 2*PICO_UNIQUE_BOARD_ID_SIZE_BYTES +1 allocated (17 chars)
void getFlashUid(char* str)
{
  //pico_get_unique_board_id_string(str, 2*PICO_UNIQUE_BOARD_ID_SIZE_BYTES +1);
}

void initPins()
{
	// Configure pins and peripherals

}

//--------------------- PRESET MANAGEMENT ---------------------//
void readCurrentPreset()
{
  EEPROM.get(sizeof(Config) + sizeof(Preset) * globalConfig.currentPreset,
                *currentPresetPtr);
}

void saveCurrentPreset()
{
  EEPROM.put(sizeof(Config) + sizeof(Preset) * globalConfig.currentPreset,
                *currentPresetPtr);
  EEPROM.commit();
  delay(1);
}

void readGlobalConfig()
{
  EEPROM.get(sizeof(Config) + sizeof(Preset) * globalConfig.currentPreset,
                *currentPresetPtr);
}

void saveGlobalConfig()
{
  EEPROM.put(0, globalConfig);
  EEPROM.commit();
}

void presetUp()
{
  // Increment presets
  if(globalConfig.currentPreset >= NUM_PRESETS)
  {
    globalConfig.currentPreset = 0;
  }
  else
  {
    globalConfig.currentPreset++;
  }
  readCurrentPreset();
  saveGlobalConfig();
}

void presetDown()
{
  // Increment presets
  if(globalConfig.currentPreset == 0)
  {
    globalConfig.currentPreset = NUM_PRESETS;
  }
  else
  {
    globalConfig.currentPreset--;
  }
  readCurrentPreset();
  saveGlobalConfig();
}

void goToPreset(uint8_t newPreset)
{
  if(newPreset > NUM_PRESETS)
  {
    return;
  }
  globalConfig.currentPreset = newPreset;
  readCurrentPreset();
  saveGlobalConfig();
}


//--------------------- MIDI CALLBACK HANDLERS ---------------------//
void assignMidiCallbacks()
{
	midi_AssignControlChangeCallback(handleControlChange);
	midi_AssignProgramChangeCallback(handleProgramChange);
	midi_AssignSysemExclusiveCallback(handleSysEx);
}

void handleControlChange(MidiInterfaceType interface, uint8_t channel, uint8_t number, uint8_t value)
{
  	
	// Save current preset
	if (number == SAVE_CURRENT_PRESET_CC)
	{
		saveCurrentPreset();
	}
	// Preset up
	else if (number == PRESET_UP_CC)
	{
		presetUp();
	}
	// Preset down
	else if (number == PRESET_DOWN_CC)
	{
		presetDown();
	}
	// Go to preset
	else if (number == GO_TO_PRESET_CC)
	{
		goToPreset(value);
	}
}

void handleProgramChange(MidiInterfaceType interface, uint8_t channel, uint8_t number)
{

}

void handleSysEx(MidiInterfaceType interface, uint8_t* array, unsigned size)
{

}