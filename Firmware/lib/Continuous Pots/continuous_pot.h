#ifndef CONTINUOUS_POT_H
#define CONTINUOUS_POT_H

#include <stdint.h>

#define RV142FPF_NUM_READINGS    5
#define POT_WRAP_THRESHOLD  		50

typedef struct 
{
    uint16_t pos;       // Relative position
    uint8_t dir;        // Last rotated direction
    uint16_t lastReadingA;       
    uint16_t lastReadingB;
    uint16_t readingsA[RV142FPF_NUM_READINGS];  
    uint16_t readingsB[RV142FPF_NUM_READINGS];
    uint8_t readIndex;
} CONTPOT;

void contPot_init(CONTPOT *pot);

void contPot_update(CONTPOT *pot, uint16_t readingA, uint16_t readingB);

#endif /* CONTINUOUS_POT_H */