#include "spin.h"

#include "Arduino.h"
#include "device_api.h"
#include "ota_pull.h"

const char* host = "Spin";
const char* apName = "SpinAP";

char deviceApiBuffer[8192];




void setup()
{
	
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
	delay(2000);
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

	
	
	delay(1000);
	midi_Init();
	//wifi_Connect("uLoop", "uLoopAP", "password");	
	wifi_Connect("Spin", "SpinAP", NULL);
	ESP32OTAPull ota;
	int ret = ota.CheckForOTAUpdate("http://example.com/myimages/example.json", "0.1.0");
	//ota_Begin();
	
}

void loop()
{
	if(wifi_ConnectionStatus())
	{
		ota_Loop();
	}
	readPots();
	for(uint8_t i=0; i<NUM_POTS; i++)
	{
		ledBar_Update(&ledBars[i]);
	}
	FastLED.show();
	
	midi_ReadAll();

	//delay(2);
	if(Serial.available())
	{
		deviceApi_Handler(deviceApiBuffer, 0);
	}
	
}


