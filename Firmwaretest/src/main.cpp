//#include "spin.h"

#include "Arduino.h"


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
	
	
}


