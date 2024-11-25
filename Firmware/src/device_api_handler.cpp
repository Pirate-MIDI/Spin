#include "device_api_handler.h"
#include "device_api_utility.h"
#include "device_api.h"
#include "ArduinoJson.h"
#include "math.h"
#include "esp32-hal-tinyusb.h" // required for entering download mode
#include "midi_handling.h"
#include "wifi_management.h"
#include "ota_pull.h"
#include "ota_updating.h"

// Transmit functions
void sendCheckResponse(uint8_t transport)
{
	// Allocate the JSON document
	JsonDocument doc;

	doc["deviceModel"] = "Spin";
	doc["firmwareVersion"] = FW_VERSION;
	doc["hardwareVersion"] = HW_VERSION;
	doc["uId"] = ((ESP.getEfuseMac() << 40) >> 40);
	doc["deviceName"] = "Spin";
	doc["profileId"] = 0;

	if(transport == USB_CDC_TRANSPORT)
	{
		serializeJson(doc, Serial);
		sendPacketTermination(USB_CDC_TRANSPORT);
	}
	else if(transport == MIDI_TRANSPORT)
	{
		CustomWriter writer;
		writer.transport = MIDI_TRANSPORT;
		serializeJson(doc, writer);
		writer.flush();
		sendPacketTermination(MIDI_TRANSPORT);
	}
	return;
}

void sendGlobalSettings(uint8_t transport)
{
	
}

void sendBankSettings(int bankNum, uint8_t transport)
{
	
}

void sendBankId(int bankNum, uint8_t transport)
{
	
}

void sendCurrentBank(uint8_t transport)
{
	
}


// Parsing functions
void parseGlobalSettings(char* appData, uint8_t transport)
{
	
}

void parseBankSettings(char* appData, uint16_t bankNum, uint8_t transport)
{
	
}

void ctrlCommandHandler(char* appData, uint8_t transport)
{
	// Allocate the JSON document
	JsonDocument doc;

	// Deserialize the JSON document
	DeserializationError error = deserializeJson(doc, appData);
	// Test if parsing succeeds
	if (error)
	{
		Serial.write("deserializeJson() failed: ", strlen("deserializeJson() failed: "));
		Serial.write(error.c_str(), strlen(error.c_str()));
		Serial.write("\n", strlen("\n"));
		return;
	}

	if(doc[USB_COMMAND_STRING])
	{
		// If there is an array of commands
		uint8_t numCommands = doc.size();
		for(uint16_t i=0; i<numCommands; i++)
		{
			// Strings
			if(doc[USB_COMMAND_STRING][i].is<const char*>())
			{
				const char* command = doc[USB_COMMAND_STRING][i];
				// Prioritise the restart command
				if(strcmp(command, USB_RESTART_STRING) == 0)
				{
					ESP.restart();
				}
				else if(strcmp(command, USB_ENTER_BOOTLOADER_STRING) == 0)
				{
					usb_persist_restart(RESTART_BOOTLOADER);
				}
				else if(strcmp(command, USB_BANK_UP_STRING) == 0)
				{
					//todo
				}
				else if(strcmp(command, USB_BANK_DOWN_STRING) == 0)
				{
					// todo
				}
				else if(strcmp(command, "turnOffBLE") == 0)
				{
					//turnOffBLE();
				}
				else if(strcmp(command, "turnOnWifi") == 0)
				{
					wifi_Connect("SpinAP", "SpinAP", NULL);
				}
				else if(strcmp(command, "checkFirmwareUpdate") == 0)
				{
					ESP32OTAPull ota;
					const char url[] = "https://raw.githubusercontent.com/Pirate-MIDI/Spin/refs/heads/main/Firmware/ota_configuration.json";
					ota.CheckForOTAUpdate(url, "0.1.0", ota.UPDATE_AND_BOOT);
				}
				else if(strcmp(command, "checkLatestFirmwareVersion") == 0)
				{
					Serial.println(ota_GetLatestVersion("https://raw.githubusercontent.com/Pirate-MIDI/Spin/refs/heads/main/Firmware/ota_configuration.json"));
				}
			}
			else
			{
				if(!doc[USB_COMMAND_STRING][i][USB_GO_TO_BANK_STRING].isNull())
				{
					uint16_t bankIndex = doc[USB_COMMAND_STRING][i][USB_GO_TO_BANK_STRING];
					if(bankIndex < 128)
					{
						// todo
					}
				}
			}
		}
	}
}

