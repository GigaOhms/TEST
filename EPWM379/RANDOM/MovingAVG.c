#include "MovingAVG.h"

void MovingAVG_Init(MovingAVG *mavg, uint16_t L){
	mavg->L = L;
	mavg->invL = 1.0f / ((float)L);
	mavg->count = 0;
	// Clear buffer
	mavg->sum_in = 0.0f;
	// // Clear output
	mavg->out = 0.0f;
}

float MovingAVG_Update(MovingAVG *mavg, float in){
	mavg->sum_in += in;
	if (mavg->count >= (mavg->L - 1)){
		mavg->count = 0;
		mavg->out = mavg->invL * mavg->sum_in;
		mavg->sum_in = 0.0f;
	}
	else mavg->count++;
		
	return mavg->out;
}
