#ifndef ARDURCT_REMOTE_H
#define ARDURCT_REMOTE_H

#include <WProgram.h>
#include <inttypes.h>

#include "ArduRCT_Remote_Config.h"

uint8_t servoPosition[NB_CHANNELS];

void radioSetup();
void radioProcessReceive();

#endif