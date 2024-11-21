#include "spin.h"

#include "Arduino.h"

#include "midi_handling.h"
#include "device_api.h"
#include "wifi_management.h"

const char* host = "Spin";
const char* apName = "SpinAP";

char deviceApiBuffer[8192];

LEDBar ledBars[NUM_POTS];
SPIClass * hspi = NULL;

CONTPOT pots[NUM_POTS];

void readPots();

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

	//wifi_Connect("uLoop", "uLoopAP", "password");	
}

void loop()
{
	readPots();
	for(uint8_t i=0; i<NUM_POTS; i++)
		ledBar_Update(&ledBars[i]);

	FastLED.show();
	delay(1);

}


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