#include "spin.h"

#include "Arduino.h"
#include "device_api.h"


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
	delay(200);
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

	
	

	midi_Init();
	//wifi_Connect("uLoop", "uLoopAP", "password");	
	
}

void loop()
{
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


