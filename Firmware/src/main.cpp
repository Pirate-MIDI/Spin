#include "spin.h"

#include "Arduino.h"
#include "device_api.h"

const char* host = "Spin";
const char* apName = "SpinAP";

char deviceApiBuffer[8192];




void setup()
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

	initPins();
	
	// LED init
	
	FastLED.addLeds<SK6812, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
	FastLED.setBrightness(255);

	FastLED.show();
	// Read stored global config settings

	
	// MIDI init
	midi_Init();
	assignMidiCallbacks();
	byte* testBuffer = (byte*)ps_malloc(500000);
	
	// Serial config
	Serial.begin(9600);
	delay(200);
	Serial.print("Booting - ");
	bootCheck();

	pinMode(ADC_CS1_PIN, OUTPUT);
	pinMode(ADC_CS2_PIN, OUTPUT);
	hspi = new SPIClass(HSPI);
	hspi->begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN);


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
	midi_ReadAll();
	readPots();
	for(uint8_t i=0; i<NUM_POTS; i++)
		//ledBar_Update(&ledBars[i]);

	//astLED.show();
	if(Serial.available())
	{
		deviceApi_Handler(deviceApiBuffer, 0);
	}

}


