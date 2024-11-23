#ifndef HARDWARE_DEF_H_
#define HARDWARE_DEF_H_

#include "SPI.h"

//---------------------- Pins ----------------------//
#define LED_DATA_PIN				15

#define MIDI_OUT_PIN        	43
#define MIDI_IN_PIN             44

// USB Host 
#define USBH_INT_PIN				1
#define USBH_GPX_PIN				2
#define USBH_CS_PIN				36
#define USBH_RST_PIN				38
#define USBH_SPI_SCK_PIN		37
#define USBH_SPI_MOSI_PIN		45
#define USBH_SPI_MISO_PIN		35

// ADC
#define ADC_CS1_PIN          0
#define ADC_CS2_PIN          3

#define SPI_SCK_PIN				8
#define SPI_MOSI_PIN				10
#define SPI_MISO_PIN				46

// IO expander and display
#define I2C0_SDA_PIN				17
#define I2C0_SCL_PIN				16

#define I2C1_SDA_PIN				21
#define I2C1_SCL_PIN				14

#define NUM_LEDS              	168
#define NUM_LEDS_PER_RING			21
#define NUM_POTS              	8


#define PCA9555_ADDRESS			0b0100000
#define PCA9555_INT_PIN			48

// PCA9555 channels
#define ENC1_SW_CHANNEL				12
#define ENC2_SW_CHANNEL				5

#define ENC1_RED_CHANNEL			9
#define ENC1_GREEN_CHANNEL			10
#define ENC1_BLUE_CHANNEL			11
#define ENC2_RED_CHANNEL			4
#define ENC2_GREEN_CHANNEL			3
#define ENC2_BLUE_CHANNEL			6

#define USBH_ENABLE_CHANNEL		13
#define USBH_CHARGE_CHANNEL		14

#define USBH_VBUS_SENSE_CHANNEL	15

//#define USBH_SPI				FSPI


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