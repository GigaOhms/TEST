/*
 * epwm.h
 *
 *  Created on: 2019. 10. 28.
 *      Author: LTH
 */

#ifndef SETUP_EPWM_H_
#define SETUP_EPWM_H_
#ifdef __cplusplus
extern "C" {
#endif

//
// Defines
//
//#define EPWM1_TIMER_TBPRD  10000U  //Period register : 10kHz triangle
//#define EPWM1_TIMER_TBPRD  5000U //Period register : 20kHz triangle
//#define EPWM1_TIMER_TBPRD  2500U //Period register : 40kHz triangle
#define EPWM1_TIMER_TBPRD  2000U //Period register : 50kHz triangle
//#define EPWM1_TIMER_TBPRD  1000U //Period register : 100kHz triangle
//#define EPWM1_TIMER_TBPRD  500U  //Period register : 200kHz triangle




void setup_epwm(void);
void gpio_select(void);

void setup_epwm1(void);
void setup_epwm2(void);
void setup_epwm3(void);
void setup_epwm4(void);
void setup_epwm5(void);
void setup_epwm6(void);
void setup_epwm7(void);
void setup_epwm8(void);
void setup_epwm9(void);
void setup_epwm10(void);
void setup_epwm11(void);
void setup_epwm12(void);
void setup_ADC(void);
void setup_DAC(void);




















#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SETUP_EPWM_H_ */
