//#include "spin.h"

#include "Arduino.h"
#include "device_api.h"
//#include "spin.h"
#include "ota_pull.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#include <AppleMIDI.h>

#include "usb_comms.h"

const char* host = "Spin";
const char* apName = "SpinAP";

char deviceApiBuffer[8192];

const char url[] = "https://raw.githubusercontent.com/Pirate-MIDI/Spin/refs/heads/main/Firmware/ota_configuration.json";

void setup()
{
	/*
	initPins();
	
	// LED init
	
	FastLED.addLeds<SK6812, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
	FastLED.setBrightness(255);

	FastLED.show();
	initLedBars();
	// Read stored global config settings
	
	assignMidiCallbacks();
	byte* testBuffer = (byte*)ps_malloc(500000);
	
	// Serial config
	Serial.begin(9600);
	
	//delay(1000);
	Serial.print("Booting - ");
	//bootCheck();
	
	// Print system diagnostic data
	
	Serial.print("Total heap: %d");
	Serial.println(ESP.getHeapSize());
	Serial.print("Free heap: %d");
	Serial.println(ESP.getFreeHeap());
	Serial.print("Total PSRAM: %d");
	Serial.println(ESP.getPsramSize());
	Serial.print("Free PSRAM: %d");
	Serial.println(ESP.getFreePsram());
	

	wifiEnabled = 1;
	midi_Init();
	//turnOnBLE();
	wifi_Connect(WIFI_HOSTNAME, WIFI_AP_SSID, NULL);
	turnOnWifiRtp();
	//wifi_CheckConnectionPing();



	const char url[] = "https://raw.githubusercontent.com/Pirate-MIDI/Spin/refs/heads/main/Firmware/ota_configuration.json";
	//Serial.println(ota_GetLatestVersion(url));
	*/
	

	// init USB
    Serial0.println("Init USB");
    init_usb_comms();
}

void loop()
{
	
	mainProcess();
	if(wifi_ConnectionStatus())
	{
		//ota_Loop();
	}
	
	readPots();
	
	for(uint8_t i=0; i<NUM_POTS; i++)
	{
		ledBar_Update(&ledBars[i]);
	}
	
	FastLED.show();
	
	
	//delay(2);
	if(Serial.available())
	{
		deviceApi_Handler(deviceApiBuffer, 0);
	}
	
	// Listen to incoming MIDI from all sources
	//testMidi();
	midi_ReadAll();
	
}


