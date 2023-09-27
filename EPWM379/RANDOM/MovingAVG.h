#ifndef MOVINGAVG_H
#define MOVINGAVG_H

#include <stdint.h>

#define MOVING_AVG_MAX_BUF 1000

typedef struct {
	uint16_t L;		// Windows length
	float invL;		// 1 divided by window length
	uint16_t count;		// circular buffer
	float sum_in;
	float out;		// RMS estimate Squared
} MovingAVG;

void MovingAVG_Init(MovingAVG *mavg, uint16_t L);
float MovingAVG_Update(MovingAVG *mavg, float in);

#endif
