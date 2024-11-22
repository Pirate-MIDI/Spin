#ifndef HARDWARE_DEF_H_
#define HARDWARE_DEF_H_

#include "SPI.h"

//---------------------- Pins ----------------------//
#define LED_DATA_PIN					15

#define MIDI_OUT_PIN        		43
#define MIDI_IN_PIN             	44

#define NUM_LEDS              	168
#define NUM_LEDS_PER_RING			21
#define NUM_POTS              	8

// USB Host 
#define USBH_INT_PIN			1
#define USBH_GPX_PIN			2
#define USBH_CS_PIN			36
#define USBH_RES_PIN			38

#define USBH_SPI_SCK_PIN	37
#define USBH_SPI_MOSI_PIN	45
#define USBH_SPI_MISO_PIN	35

#define USBH_SPI				VSPI

#define SPI_SCK_PIN			8
#define SPI_MOSI_PIN			10
#define SPI_MISO_PIN			46

#define ADC_CS1_PIN           	0
#define ADC_CS2_PIN           	3

#define POT1A_CHANNEL   2
#define POT1B_CHANNEL   3
#define POT2A_CHANNEL   4
#define POT2B_CHANNEL   5
#define POT3A_CHANNEL   2
#define POT3B_CHANNEL   3
#define POT4A_CHANNEL   4
#define POT4B_CHANNEL   5
#define POT5A_CHANNEL   0
#define POT5B_CHANNEL   1
#define POT6A_CHANNEL   6
#define POT6B_CHANNEL   7
#define POT7A_CHANNEL   0
#define POT7B_CHANNEL   1
#define POT8A_CHANNEL   6
#define POT8B_CHANNEL   7



#endif // HARDWARE_DEF_H_