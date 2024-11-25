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
#include "pca9555.h"
#include "Wire.h"

CRGB leds[NUM_LEDS];

Preset presets[NUM_PRESETS];
GlobalSettings globalSettings;
Preset* currentBankPtr = &presets[globalSettings.currentBank];
Preferences storage;

LEDBar ledBars[NUM_POTS];
SPIClass* hspi = NULL;

CONTPOT pots[NUM_POTS];

PCA9555 pca9555(PCA9555_ADDRESS, PCA9555_INT_PIN);


void softwareReset();

void setChannel(uint8_t channel, uint8_t channelState);

void handleControlChange(MidiInterfaceType interface, uint8_t channel, uint8_t number, uint8_t value);
void handleProgramChange(MidiInterfaceType interface, uint8_t channel, uint8_t number);
void handleGeneralSysEx(MidiInterfaceType interface, uint8_t* array, unsigned size);

//--------------------- SYSTEM & EVENT HANDLERS ---------------------//
void bootCheck()
{
	// Check if the device has been configured
	Serial.println("Checking boot state...");
	storage.begin("global", false);
	size_t len = storage.getBytes("config", &globalSettings, sizeof(GlobalSettings));
	storage.end();
	// Uncomment to force a new device configuration
	globalSettings.bootState = 0;
	if(globalSettings.bootState != DEVICE_CONFIGURED_VALUE || len != sizeof(GlobalSettings))
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
	globalSettings.bootState = DEVICE_CONFIGURED_VALUE;
	globalSettings.currentBank = 0;
	globalSettings.midiInChannel = MIDI_CHANNEL_OMNI;
	globalSettings.profileId = 0;
	globalSettings.ledBrightness = DEFAULT_LED_BRIGHTNESS;
	FastLED.setBrightness(DEFAULT_LED_BRIGHTNESS);
	
	
	// Erase the NVS partition and initialize
	nvs_flash_erase(); 
	nvs_flash_init();

	// Save the default config to eeprom
	storage.begin("global", false);
	size_t len = storage.putBytes("config", &globalSettings, sizeof(GlobalSettings));
	Serial.printf("Wrote global config to storage with size %d.\n", len);

	// Save the default presets to eeprom
	storage.begin("presets", false);
	len = 0;
	
	storage.end();
	Serial.printf("Wrote presets (128*%d) to storage with size %d.\n", sizeof(Preset), len);
}

void standardBoot()
{
	FastLED.setBrightness(globalSettings.ledBrightness);
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
	// ADC CS pins and SPI peripheral
	
	pinMode(ADC_CS1_PIN, OUTPUT);
	pinMode(ADC_CS2_PIN, OUTPUT);
	hspi = new SPIClass(HSPI);
	hspi->begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN);
	
	// PCA9555 IO expander
	Wire.setPins(I2C1_SDA_PIN, I2C1_SCL_PIN);
	Wire.begin();
	pca9555.setClock(400000);
	Serial.println(pca9555.begin());
	pca9555.pinMode(ENC1_SW_CHANNEL, INPUT_PULLDOWN);
	pca9555.pinMode(ENC2_SW_CHANNEL, INPUT_PULLDOWN);

	pca9555.pinMode(ENC1_RED_CHANNEL, OUTPUT);
	pca9555.pinMode(ENC1_GREEN_CHANNEL, OUTPUT);
	pca9555.pinMode(ENC1_BLUE_CHANNEL, OUTPUT);
	pca9555.pinMode(ENC2_RED_CHANNEL, OUTPUT);
	pca9555.pinMode(ENC2_GREEN_CHANNEL, OUTPUT);
	pca9555.pinMode(ENC2_BLUE_CHANNEL, OUTPUT);

	pca9555.pinMode(USBH_ENABLE_CHANNEL, OUTPUT);
	pca9555.pinMode(USBH_CHARGE_CHANNEL, OUTPUT);
	pca9555.pinMode(USBH_VBUS_SENSE_CHANNEL, INPUT);

	pca9555.digitalWrite(ENC1_RED_CHANNEL, HIGH);
	pca9555.digitalWrite(ENC2_RED_CHANNEL, HIGH);

	pca9555.digitalWrite(ENC1_GREEN_CHANNEL, HIGH);
	pca9555.digitalWrite(ENC1_BLUE_CHANNEL, HIGH);
	pca9555.digitalWrite(ENC2_GREEN_CHANNEL, HIGH);
	pca9555.digitalWrite(ENC2_BLUE_CHANNEL, HIGH);

	pca9555.digitalWrite(USBH_ENABLE_CHANNEL, HIGH);
	pca9555.digitalWrite(USBH_CHARGE_CHANNEL, HIGH);

	// Reset the USB host controller
	pinMode(USBH_RST_PIN, OUTPUT);
	digitalWrite(USBH_RST_PIN, LOW);
	digitalWrite(USBH_RST_PIN, HIGH);
	
}

void mainProcess()
{
	static int flash = 0;
	if(millis() % 500 == 0)
	{
		pca9555.digitalWrite(ENC2_BLUE_CHANNEL, flash);
		flash = !flash;
	}
}

//--------------------- PRESET MANAGEMENT ---------------------//
void readcurrentBank()
{
  EEPROM.get(sizeof(GlobalSettings) + sizeof(Preset) * globalSettings.currentBank,
                *currentBankPtr);
}

void savecurrentBank()
{
  EEPROM.put(sizeof(GlobalSettings) + sizeof(Preset) * globalSettings.currentBank,
                *currentBankPtr);
  EEPROM.commit();
  delay(1);
}

void readglobalSettings()
{
  EEPROM.get(sizeof(GlobalSettings) + sizeof(Preset) * globalSettings.currentBank,
                *currentBankPtr);
}

void saveglobalSettings()
{
  EEPROM.put(0, globalSettings);
  EEPROM.commit();
}

void presetUp()
{
  // Increment presets
  if(globalSettings.currentBank >= NUM_PRESETS)
  {
    globalSettings.currentBank = 0;
  }
  else
  {
    globalSettings.currentBank++;
  }
  readcurrentBank();
  saveglobalSettings();
}

void presetDown()
{
  // Increment presets
  if(globalSettings.currentBank == 0)
  {
    globalSettings.currentBank = NUM_PRESETS;
  }
  else
  {
    globalSettings.currentBank--;
  }
  readcurrentBank();
  saveglobalSettings();
}

void goToPreset(uint8_t newPreset)
{
  if(newPreset > NUM_PRESETS)
  {
    return;
  }
  globalSettings.currentBank = newPreset;
  readcurrentBank();
  saveglobalSettings();
}


//--------------------- CONTROL INTERFACE ---------------------//
void readPots()
{
    static uint16_t lastPotValues[NUM_POTS];
    static uint8_t lastPotDirections[NUM_POTS];
    int results[NUM_POTS*2];
    // Read raw ADC values
    results[0] = mcp3008_readADC(hspi, ADC_CS1_PIN, POT1A_CHANNEL, 0);
    results[1] = mcp3008_readADC(hspi, ADC_CS1_PIN, POT1B_CHANNEL, 0);
    results[2] = mcp3008_readADC(hspi, ADC_CS1_PIN, POT2A_CHANNEL, 0);
    results[3] = mcp3008_readADC(hspi, ADC_CS1_PIN, POT2B_CHANNEL, 0);
    results[4] = mcp3008_readADC(hspi, ADC_CS2_PIN, POT3A_CHANNEL, 0);
    results[5] = mcp3008_readADC(hspi, ADC_CS2_PIN, POT3B_CHANNEL, 0);
    results[6] = mcp3008_readADC(hspi, ADC_CS2_PIN, POT4A_CHANNEL, 0);
    results[7] = mcp3008_readADC(hspi, ADC_CS2_PIN, POT4B_CHANNEL, 0);
    results[8] = mcp3008_readADC(hspi, ADC_CS1_PIN, POT5A_CHANNEL, 0);
    results[9] = mcp3008_readADC(hspi, ADC_CS1_PIN, POT5B_CHANNEL, 0);
    results[10] = mcp3008_readADC(hspi, ADC_CS1_PIN, POT6A_CHANNEL, 0);
    results[11] = mcp3008_readADC(hspi, ADC_CS1_PIN, POT6B_CHANNEL, 0);
	 results[12] = mcp3008_readADC(hspi, ADC_CS2_PIN, POT7A_CHANNEL, 0);
    results[13] = mcp3008_readADC(hspi, ADC_CS2_PIN, POT7B_CHANNEL, 0);
    results[14] = mcp3008_readADC(hspi, ADC_CS2_PIN, POT8A_CHANNEL, 0);
    results[15] = mcp3008_readADC(hspi, ADC_CS2_PIN, POT8B_CHANNEL, 0);

    // Filter and process results
    for(uint8_t i = 0; i < NUM_POTS; i++)
    {
        // Process absolute values
        contPot_update(&pots[i], results[i*2], results[i*2+1]);
        // Map filtered values to interface structure

        int valueDelta = pots[i].pos - lastPotValues[i];  // Amount of change in the pot's absolute position
        // Apply min/max boundaries
        int tempPotValue = ledBars[i].value + valueDelta;
        if(abs(valueDelta) < POT_WRAP_THRESHOLD)
        {
          if(tempPotValue < 0)
          {
            ledBars[i].value = 0;
          }
          else if(tempPotValue > 1023)
          {
            ledBars[i].value = 1023;
          }
          else
          {
            ledBars[i].value += valueDelta;
          }
        }

        lastPotValues[i] = pots[i].pos;
        lastPotDirections[i] = pots[i].dir;
        char str[20];
#ifdef SERIAL_PRINT_POTS
        sprintf(str, "%d Pos: %4d %d  ", i+1, ledBars[i].value, pots[i].dir);
        Serial.print(str);
#endif
    } 
#ifdef SERIAL_PRINT_POTS
    Serial.println();
#endif
}

void initLedBars()
{
	ledBars[0].firstLedIndex = 0;
	ledBars[1].firstLedIndex = NUM_LEDS_PER_RING;
	ledBars[2].firstLedIndex = NUM_LEDS_PER_RING*2;
	ledBars[3].firstLedIndex = NUM_LEDS_PER_RING*3;
	ledBars[4].firstLedIndex = NUM_LEDS_PER_RING*7;
	ledBars[5].firstLedIndex = NUM_LEDS_PER_RING*6;
	ledBars[6].firstLedIndex = NUM_LEDS_PER_RING*5;
	ledBars[7].firstLedIndex = NUM_LEDS_PER_RING*4;

	ledBars[0].ledMode = PotLedFillGradient;
	ledBars[1].ledMode = PotLedDot;
	ledBars[2].ledMode = PotLedDotGradient;
	ledBars[3].ledMode = PotLedFillGradient;

	ledBars[4].ledMode = PotLedFillGradient;
	ledBars[5].ledMode = PotLedDot;
	ledBars[6].ledMode = PotLedDotGradient;
	ledBars[7].ledMode = PotLedFillGradient;

	for(uint8_t i=0; i<NUM_POTS; i++)
	{
		ledBars[i].numLeds = 21;
		
		ledBars[i].dataDirection = 0;
		if(i < 4)
			ledBars[i].mode = PotCentred;

		else
			ledBars[i].mode = PotNormal;

		ledBars[i].wrap = PotWrap;
		ledBars[i].value = 0;
		ledBar_Update(&ledBars[i]);
	}
	ledBars[0].colours[0] = csvNeonPink;
	ledBars[0].colours[1] = csvNeonLightBlue;

	ledBars[1].colours[0] = csvNeonPink;
	ledBars[1].colours[1] = csvNeonLightBlue;

	ledBars[2].colours[0] = csvNeonLime;
	ledBars[2].colours[1] = csvNeonRed;

	ledBars[3].colours[0] = csvNeonLime;
	ledBars[3].colours[1] = csvNeonRed;

	ledBars[4].colours[0] = csvNeonPink;
	ledBars[4].colours[1] = csvNeonLightBlue;

	ledBars[5].colours[0] = csvNeonPink;
	ledBars[5].colours[1] = csvNeonLightBlue;

	ledBars[6].colours[0] = csvNeonLime;
	ledBars[6].colours[1] = csvNeonRed;

	ledBars[7].colours[0] = csvNeonLime;
	ledBars[7].colours[1] = csvNeonRed;
}

//--------------------- MIDI CALLBACK HANDLERS ---------------------//
void assignMidiCallbacks()
{
	midi_AssignControlChangeCallback(handleControlChange);
	midi_AssignProgramChangeCallback(handleProgramChange);
	midi_AssignSysemExclusiveCallback(handleGeneralSysEx);
}

// Spin application specific MIDI callbacks
void handleControlChange(MidiInterfaceType interface, uint8_t channel, uint8_t number, uint8_t value)
{
  	
	// Save current preset
	if (number == SAVE_CURRENT_PRESET_CC)
	{
		savecurrentBank();
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

void handleGeneralSysEx(MidiInterfaceType interface, uint8_t* array, unsigned size)
{

}