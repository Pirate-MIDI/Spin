#include "continuous_pot.h"
#include "stdint.h"
#include <stdlib.h>

#define FILTER_HYSTERISIS  2

#define DIR_CW     0
#define DIR_CCW    1

void contPot_init(CONTPOT *pot)
{
    pot->pos, pot->dir = 0;
    pot->lastReadingA, pot->lastReadingB = 0;
    pot->readIndex = 0;
    for(uint8_t i=0; i<RV142FPF_NUM_READINGS; i++)
    {
        pot->readingsA[i] = 0;
        pot->readingsB[i] = 0;
    }
}

void contPot_update(CONTPOT *pot, uint16_t readingA, uint16_t readingB)
{
    // Update readings and filter raw values
    if(abs(readingA-pot->lastReadingA) < FILTER_HYSTERISIS || abs(readingB-pot->lastReadingB) < FILTER_HYSTERISIS)
    {
        return;
    }
    pot->readingsA[pot->readIndex] = readingA;
    pot->readingsB[pot->readIndex] = readingB;
    pot->readIndex++;
    if(pot->readIndex >= RV142FPF_NUM_READINGS)
    {
        pot->readIndex = 0;
    }
     
    uint16_t avgA = 0;
    uint16_t avgB = 0;
    for(uint8_t i=0; i<RV142FPF_NUM_READINGS; i++)
    {
        avgA += pot->readingsA[i];
        avgB += pot->readingsB[i];
    }
    avgA /= RV142FPF_NUM_READINGS;
    avgB /= RV142FPF_NUM_READINGS;
    
    // Determine position and direction
    // 0-180 degrees
    if(avgB  >= 512)
    {
        pot->pos = (1024 - avgA)/2;
        if(avgA < pot->lastReadingA)
        {
            pot->dir = DIR_CW;
        }
        else if(avgA > pot->lastReadingA)
        {
            pot->dir = DIR_CCW;
        }
    }
    // 180-360 degrees
    else
    {
        pot->pos = avgA/2 + 512;
        if(avgA > pot->lastReadingA)
        {
            pot->dir = DIR_CW;
        }
        else if(avgA < pot->lastReadingA)
        {
            pot->dir = DIR_CCW;
        }
    }
    
    pot->lastReadingA = avgA;
    pot->lastReadingB = avgB;
}