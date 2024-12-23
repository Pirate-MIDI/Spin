//#include "spin.h"

#include "Arduino.h"
#include "device_api.h"
#include "Adafruit_TinyUSB.h"
#include "SPI.h"
#include "ota_pull.h"
#include "spin.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#include <AppleMIDI.h>
#include "hardware_def.h"
#include "esp_log.h"

#include "usb_helpers.h"
#include "tonexOne.h"
#include "usb_cdc_handling.h"

//idVendor = 0x1963
//idProduct = 0x00D1
#define TONEX_ONE_PID	0x00D1
#define TONEX_ONE_VID	0x1963

static const char *TAG = "app_usb";

const char* host = "Spin";
const char* apName = "SpinAP";

char deviceApiBuffer[8192];


const char url[] = "https://raw.githubusercontent.com/Pirate-MIDI/Spin/refs/heads/main/Firmware/ota_configuration.json";
#define USBH_STACK_SZ 2048
#define USE_FREERTOS

//Adafruit_USBH_Host USBHost(&SPI, USBH_SPI_SCK_PIN, USBH_SPI_MOSI_PIN, USBH_SPI_MISO_PIN, USBH_CS_PIN, USBH_INT_PIN);
//extern Adafruit_USBH_CDC SerialHost;







void setup()
{
	
	initPins();
	/*
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
	//Serial0.println("Init USB");
	//init_usb_comms();
	Serial.begin(115200);
	Serial0.begin(115200);
	delay(300);

	// init host stack on controller (rhport) 1
	usbh_Init();
	//cdc_Init();
	SerialHost.begin(115200);

  // Initialize SerialHost
  

  // Create a task to run USBHost.task() in background
  //xTaskCreate(usbhost_rtos_task, "usbh", USBH_STACK_SZ, NULL, 3, NULL);

  //while ( !Serial ) delay(10);   // wait for native usb
}

void loop()
{
	//forward_serial();
	//tuh_task();
	usbh_Process();
	cdc_Process();
	//();
	//Serial0.flush();
	/*
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
	*/
}

/*
extern "C" {

// Invoked when a device with CDC interface is mounted
// idx is index of cdc interface in the internal pool.
void tuh_cdc_mount_cb(uint8_t idx) {
  // bind SerialHost object to this interface index
  SerialHost.mount(idx);
  Serial.println("SerialHost is connected to a new CDC device");
}

// Invoked when a device with CDC interface is unmounted
void tuh_cdc_umount_cb(uint8_t idx) {
  SerialHost.umount(idx);
  Serial.println("SerialHost is disconnected");
}

}
*/
