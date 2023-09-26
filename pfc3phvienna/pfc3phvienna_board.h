/* ==============================================================================
System Name:

File Name:	  	<BoardName>_board.h

Target:

Author:

Description:	This file consits of common variables for a particular hardware board
				Like variables and functions to read current and voltage signals on the board
				functions to setup the basic peripherals of the board
				These common set is independent of the control method used and hence helps
				in eliminating duplication of this code when implementing different
				control schemes on the same design
				This file must be settings independent, an settings dependent code should reside
				in the parent solution project.

Copyright (C) {2015} Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED*
=================================================================================  */
#ifndef BOARD_H
#define BOARD_H

#ifdef __cplusplus

extern "C" {
#endif


//*****************************************************************************
// the includes
//*****************************************************************************

// Include the IQmath Library First, define Global Q
#define MATH_TYPE 1
#include "IQmathLib.h"

// Include files for device support, F2806x in this case
#include "F2837xD_Device.h"        // F2837xD Headerfile Include File
#include "F2837xD_Examples.h"    // F2837xD Examples Include File
#include "F2837xD_sdfm_drivers.h"

#include "pfc3phvienna_settings.h"

//*****************************************************************************
//defines
//*****************************************************************************
// Timer definitions based on System Clock
// 150 MHz devices
#define		mSec0_5		0.5*SYSTEM_FREQUENCY*1000		// 0.5 mS
#define		mSec1		1*SYSTEM_FREQUENCY*1000		// 1.0 mS
#define		mSec2		2.0*SYSTEM_FREQUENCY*1000		// 2.0 mS
#define		mSec5		5*SYSTEM_FREQUENCY*1000		// 5.0 mS
#define		mSec7_5		7.5*SYSTEM_FREQUENCY*1000		// 7.5 mS
#define		mSec10		10*SYSTEM_FREQUENCY*1000		// 10 mS
#define		mSec20		20*SYSTEM_FREQUENCY*1000		// 20 mS
#define		mSec50		50*SYSTEM_FREQUENCY*1000		// 50 mS
#define		mSec100		100*SYSTEM_FREQUENCY*1000		// 100 mS
#define		mSec500		500*SYSTEM_FREQUENCY*1000	// 500 mS
#define		mSec1000	1000*SYSTEM_FREQUENCY*1000	// 1000 mS

#ifndef TRUE
#define FALSE 0
#define TRUE  1
#endif

#define HistorySize 8
//#define DLOG_SIZE   200

#define ADC_PU_SCALE_FACTOR  (float32)(0.000244140625)
#define HI_RES_SHIFT (float32)(65536.0)
#define ADC_PU_PPB_SCALE_FACTOR 0.000488281250 //1/2^11
#define SD_PU_SCALE_FACTOR  0.000030517578125 // 1/2^15


//definitions for selecting DACH reference
#define REFERENCE_VDDA     0

//definitions for COMPH input selection
#define NEGIN_DAC          0
#define NEGIN_PIN          1

//definitions for CTRIPH/CTRIPOUTH output selection
#define CTRIP_ASYNCH       0
#define CTRIP_SYNCH        1
#define CTRIP_FILTER       2
#define CTRIP_LATCH        3

// ADC Configuration
#define REFERENCE_INTERNAL 0 //internal reference (12-bit only)
#define REFERENCE_EXTERNAL 1 //external reference
//definitions for selecting ADC resolution
#define RESOLUTION_12BIT 0 //12-bit resolution
#define RESOLUTION_16BIT 1 //16-bit resolution (not supported for all variants)
//definitions for selecting ADC signal mode
#define SIGNAL_SINGLE 0 //single-ended channel conversions (12-bit mode only)
#define SIGNAL_DIFFERENTIAL 1 //differential pair channel conversions

#define ACQPS_SYS_CLKS 	   16
#define REFERENCE_VDAC     0
#define REFERENCE_VREF     1

#define HWREG32(x) (*((volatile Uint32 *)(x)))
#define HWREG16(x) (*((volatile Uint16 *)(x)))


//*****************************************************************************
//globals
//*****************************************************************************

extern volatile struct EPWM_REGS *ePWM[];
extern volatile struct SDFM_REGS *SDFM[];
extern volatile struct ADC_REGS *ADC[];

//*****************************************************************************
// the function prototypes
//*****************************************************************************

void setupDevice(void);
void configure3phViennaPWM(uint16_t inv_pwm_no, uint16_t pwm_period_ticks);
void configurePWMUpDwnCnt(uint16_t inv_pwm_no, uint16_t pwm_period_ticks);
void setupADC(void);
void setupPWMTrip(uint16_t pwm_no );
void setupSDFM(uint16_t PWM_ticks, uint16_t PWM_ticks_in_sdfm_osr, uint16_t SD_clk_ecap_sys_ticks, uint16_t sdfmosr);
void configureECAPforSDFMClk(uint16_t SD_clk_ecap_sys_ticks);
void configurePWMsyncforSDFM(uint16_t PWM_ticks, uint16_t PWM_ticks_in_sdfm_osr);
void configurePWM1chUpDwnCnt(int16_t n, int16_t period, int16_t mode, int16_t phase, int16_t db_red, int16_t db_fed,int16_t phase_dir);
void configurePWM1chUpCnt(int16_t n, uint16_t period);
void setupBoardProtection(uint16_t current_sense, float current_limit, float current_max_sense);
void initGlobalVariables(void);
void calibrateOffset(volatile float *iL1MeasOffset, volatile float *iL2MeasOffset, volatile float *iL3MeasOffset ,
		volatile float *v1MeasOffset, volatile float *v2MeasOffset, volatile float *v3MeasOffset ,
		float k1, float k2);
void setupProfilingGPIO(void);
void setupRelayGPIO(void);
void toggleLED(void);
void setupLEDGPIO(void);
void disablePWMCLKCounting(void);
void enablePWMCLKCounting(void);
void setupSCIconnectionForSFRA(void);
void updateBoardStatus(void);
inline void closeRelay(void);
inline void openRelay(void);
inline void setProfilingGPIO(void);
inline void resetProfilingGPIO(void);
inline void readCurrVolADCSignals(void);
inline void readCurrVolSDFMSignals(float sdfm_scale_factor);

void SCIA_Init(long SCI_VBUS_CLOCKRATE, long SCI_BAUDRATE);
void DeviceInit(void);
void MemCopy(void);
void InitFlash(void);
void SerialHostComms(void);
interrupt void controlISR(void);
interrupt void tenkHzISR(void);

// Inline functions

//TODO updateInverterPWM()
inline void updatePFCviennaPWM(float duty1PU, float duty2PU, float duty3PU, uint16_t inv_pwm_no)
{
	float pwm_period;
	uint16_t duty1, duty2, duty3;

	pwm_period= (*ePWM[inv_pwm_no]).TBPRD;

	duty1=  (uint16_t) ( (float) pwm_period * (float) fabs(duty1PU) );
	duty2=  (uint16_t) ( (float) pwm_period * (float) fabs(duty2PU) );
	duty3=  (uint16_t) ( (float) pwm_period * (float) fabs(duty3PU) );

	(*ePWM[inv_pwm_no]).CMPA.bit.CMPA = duty1;

	(*ePWM[inv_pwm_no+1]).CMPA.bit.CMPA = duty2;

	(*ePWM[inv_pwm_no+2]).CMPA.bit.CMPA = duty3;

}

//TODO closeRelay
inline void closeRelay(void)
{
	GpioDataRegs.GPASET.bit.GPIO5=1;
}

//TODO openRelay
inline void openRelay(void)
{
	GpioDataRegs.GPACLEAR.bit.GPIO5=1;
//	invVoRef=0;
//	invIiRef=0;
}

//TODO setProfilingGPIO
inline void setProfilingGPIO(void)
{
	GpioDataRegs.GPASET.bit.GPIO12 = 1;
}

//TODO resetProfilingGPIO
inline void resetProfilingGPIO(void)
{
	GpioDataRegs.GPACLEAR.bit.GPIO12=1;
}

//TODO setProfilingGPIO
inline void setProfilingGPIO2(void)
{
	GpioDataRegs.GPASET.bit.GPIO13 = 1;
}

//TODO resetProfilingGPIO
inline void resetProfilingGPIO2(void)
{
	GpioDataRegs.GPACLEAR.bit.GPIO13=1;
}

#define SDFM_READ_FILTER1_DATA_32BIT	*(int32_t *)0x5E16
#define SDFM_READ_FILTER2_DATA_32BIT	*(int32_t *)0x5E26
#define SDFM_READ_FILTER3_DATA_32BIT	*(int32_t *)0x5E36
#define SDFM_READ_FILTER4_DATA_32BIT	*(int32_t *)0x5E46





//TODO setupADCconversion()
inline void setupADCconversion(uint16_t adc_module_no, uint16_t adc_soc_offset, uint16_t adc_channel, uint16_t trig_sel, uint16_t acqps)
{
	EALLOW;
	*( ( (uint32_t *) ADC[adc_module_no] ) + (uint32_t)(adc_soc_offset>>1)) = ((uint32_t)trig_sel <<20) + ((uint32_t)adc_channel <<15) + acqps ;
	EDIS;
}


//TODO clearPWMTripFlags()
inline void clearPWMTripFlags(uint16_t pwm_no)
{
		// clear all the configured trip sources for the PWM module
		EALLOW;
		(*ePWM[pwm_no]).TZCLR.bit.OST=0x1;
		(*ePWM[pwm_no]).TZCLR.bit.CBC=0x1;
		(*ePWM[pwm_no]).TZCLR.bit.DCAEVT1=0x1;
		(*ePWM[pwm_no]).TZCLR.bit.DCBEVT1=0x1;
		(*ePWM[pwm_no]).TZCLR.bit.INT=0x1;
		EDIS;
}

//TODO clearInterrupt
inline void clearInterrupt(void)
{
#if SENSING_OPTION ==SDFM_BASED_SENSING
    EPwm11Regs.ETCLR.bit.INT=1;
#elif SENSING_OPTION ==ADC_BASED_SENSING
    EPwm1Regs.ETCLR.bit.INT=1;
    EPwm11Regs.ETCLR.bit.INT=1;
#endif
    PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}

inline void setupInterrupt(void)
{
#if SENSING_OPTION ==SDFM_BASED_SENSING
	EALLOW;
	//PWM11 INT is used to trigger the ISR
	PieVectTable.EPWM11_INT = &inverterISR;
	EPwm11Regs.ETSEL.bit.INTSEL = ET_CTRU_CMPA;  	// INT on PRD event
	EPwm11Regs.ETSEL.bit.INTEN = 1;              // Enable INT
	EPwm11Regs.ETPS.bit.INTPRD = CNTRL_ISR_FREQ_RATIO;
	clearInterrupt();

	PieCtrlRegs.PIEIER3.bit.INTx11 = 1; // Enable PWM11INT in PIE group 3
	EPwm11Regs.ETCLR.bit.INT=1;

	IER |= M_INT3; //Enable group 3 interrupts
#else
	/*EALLOW;
	//PWM1 INT is used to trigger the ISR
	PieVectTable.EPWM1_INT = &inverterISR;
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;  	// INT on PRD event
	EPwm1Regs.ETSEL.bit.INTEN = 1;              // Enable INT
	EPwm1Regs.ETPS.bit.INTPRD = CNTRL_ISR_FREQ_RATIO;         // Generate INT on every event
	clearInterrupt();

	PieCtrlRegs.PIEIER3.bit.INTx1 = 1; // Enable PWM11INT in PIE group 3
	EPwm1Regs.ETCLR.bit.INT=1;

	IER |= M_INT3; //Enable group 3 interrupts
	*/

	EALLOW;
	//PWM11 INT is used to trigger the ISR
	PieVectTable.EPWM11_INT = &controlISR;
	EPwm11Regs.ETSEL.bit.INTSEL = ET_CTRU_CMPA;  	// INT on PRD event
	EPwm11Regs.ETSEL.bit.INTEN = 1;              // Enable INT
	EPwm11Regs.ETPS.bit.INTPRD = CNTRL_ISR_FREQ_RATIO;

	PieVectTable.TIMER2_INT = &tenkHzISR;		// 1kHz interrupt from CPU timer 2

	ConfigCpuTimer(&CpuTimer2, 200, 100);
	CpuTimer2Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
	clearInterrupt();
	PieCtrlRegs.PIEIER3.bit.INTx11 = 1; // Enable PWM11INT in PIE group 3
	EPwm11Regs.ETCLR.bit.INT=1;

	IER |= M_INT3; //Enable group 3 interrupts
	IER |= M_INT14;					// CPU timer 2 is connected to CPU INT 14

#endif

	EINT;  // Enable Global interrupt INTM
	ERTM;  // Enable Global realtime interrupt DBGM
	EDIS;
}
#ifdef __cplusplus
}
#endif                                  /* extern "C" */


#endif
