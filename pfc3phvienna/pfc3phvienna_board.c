/* ==============================================================================
System Name:

File Name:

Target:			TIDM_HV_1PH_DCAC R3, F2837xD

Author:			Manish Bhardwaj

Description:	This file consists of board related initialization, this file is used to make the
				main file more readable

Copyright (C) {2015} Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED*
=================================================================================  */
#include "pfc3phvienna_board.h"

//*********************** USER Variables *************************************//
// Global variables used that are applicable to this board
//****************************************************************************//

// Variables used to indirectly address the peripheral
volatile struct EPWM_REGS *ePWM[] = {
		&EPwm1Regs,			//intentional: (ePWM[0] not used)
		&EPwm1Regs, &EPwm2Regs, &EPwm3Regs, &EPwm4Regs, &EPwm5Regs, &EPwm6Regs,
		&EPwm7Regs, &EPwm8Regs, &EPwm9Regs, &EPwm10Regs, &EPwm11Regs, &EPwm12Regs};

volatile struct SDFM_REGS *SDFM[] =
{	0, &Sdfm1Regs, &Sdfm2Regs};

// Used to indirectly access eQEP module
volatile struct EQEP_REGS *eQEP[] =
 				  { &EQep1Regs,
 				  	&EQep1Regs,
					&EQep2Regs,
				  };

// Used to indirectly access ADC module
volatile struct ADC_REGS *ADC[] =
 				  { &AdcaRegs,
 				  	&AdcaRegs,
					&AdcbRegs,
					&AdccRegs,
					&AdcdRegs
				  };

// Used to indirectly access CMPSS module
volatile struct CMPSS_REGS *CMPSS[] =
 				  { &Cmpss1Regs,
 				  	&Cmpss1Regs,
					&Cmpss2Regs,
					&Cmpss3Regs,
					&Cmpss4Regs,
					&Cmpss5Regs,
					&Cmpss6Regs
				  };


/*  This routine sets up the basic device ocnfiguration such as initializing PLL
    copying code from FLASH to RAM */
//TODO device_setup()
void setupDevice(void)
{
#ifdef FLASH
// Copy time critical code and Flash setup code to RAM
// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
// symbols are created by the linker. Refer to the linker files.
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);
#endif

	//DeviceInit();	// Device Life support & GPIO
	// Step 1. Initialize System Control:
	// PLL, WatchDog, enable Peripheral Clocks
	// This example function is found in the F28M3Xx_SysCtrl.c file.
	InitSysCtrl();

#ifdef FLASH
// Call Flash Initialization to setup flash waitstates
// This function must reside in RAM
	InitFlash();	// Call the flash wrapper init function
#endif //(FLASH)

	// Step 2. Initialize GPIO:
	// This example function is found in the F28M3Xx_Gpio.c file and
	// illustrates how to set the GPIO to it's default state.
	//InitGpio(); // Skipped for this example
	// Step 3. Clear all interrupts and initialize PIE vector table:
	// Disable CPU interrupts
	DINT;

	// Initialize the PIE control registers to their default state.
	// The default state is all PIE interrupts disabled and flags
	// are cleared.
	// This function is found in the F28M3Xx_PieCtrl.c file.
	InitPieCtrl();

	// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR).
	// This will populate the entire table, even if the interrupt
	// is not used in this example.  This is useful for debug purposes.
	// The shell ISR routines are found in F28M3Xx_DefaultIsr.c.
	// This function is found in F28M3Xx_PieVect.c.
	InitPieVectTable();

	// initialize CPU timers
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer0, 200, 10000);
	ConfigCpuTimer(&CpuTimer1, 200, 20000);
	CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
	CpuTimer1Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0

}

//TODO setupADC()
void setupADC(void)
{
	//Write ADC configurations and power up the ADC for both ADC A
	Uint16 i;

	EALLOW;
	//write configurations for ADC-A
	// External REFERENCE must be provided
	AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdcaRegs.ADCCTL2.bit.RESOLUTION = RESOLUTION_12BIT;
	AdcaRegs.ADCCTL2.bit.SIGNALMODE = SIGNAL_SINGLE;

	//Set pulse positions to late
	AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

	//power up the ADC
	AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

	//write configurations for ADC-B
	// External REFERENCE must be provided
	AdcbRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdcbRegs.ADCCTL2.bit.RESOLUTION = RESOLUTION_12BIT;
	AdcbRegs.ADCCTL2.bit.SIGNALMODE = SIGNAL_SINGLE;

	//Set pulse positions to late
	AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;

	//power up the ADC
	AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;

	//	//write configurations for ADC-C
	// External REFERENCE must be provided
	AdccRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdccRegs.ADCCTL2.bit.RESOLUTION = RESOLUTION_12BIT;
	AdccRegs.ADCCTL2.bit.SIGNALMODE = SIGNAL_SINGLE;

	//Set pulse positions to late
	AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;

	//power up the ADC
	AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	//
	//	//write configurations for ADC-D
	// External REFERENCE must be provided
	AdcdRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdcdRegs.ADCCTL2.bit.RESOLUTION = RESOLUTION_12BIT;
	AdcdRegs.ADCCTL2.bit.SIGNALMODE = SIGNAL_SINGLE;

	//Set pulse positions to late
	AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;

	//power up the ADC
	AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;

	//delay for > 1ms to allow ADC time to power up
	for(i = 0; i < 1000; i++){
	asm("   RPT#255 || NOP");
	}

	EDIS;
}



//TODO setupSDFM()
void setupSDFM(uint16_t PWM_ticks, uint16_t PWM_ticks_in_sdfm_osr,  uint16_t SD_clk_ecap_sys_ticks, uint16_t sdfmosr)
{

	// setup CAP to generate CLK for SDFM
	configureECAPforSDFMClk(SD_clk_ecap_sys_ticks);

	// setup PWM 11 for syncing up the SD filter windows with motor control PWMs
	configurePWMsyncforSDFM(PWM_ticks,PWM_ticks_in_sdfm_osr);

	// Setup GPIO for SD
	//SD Filter 1 , iL1
	GPIO_SetupPinOptions(48, GPIO_INPUT, GPIO_SYNC);
	GPIO_SetupPinMux(48,0,7);

	GPIO_SetupPinOptions(49, GPIO_INPUT, GPIO_SYNC);
	GPIO_SetupPinMux(49,0,7);

	//SD Filter 2 , iL2
	GPIO_SetupPinOptions(50, GPIO_INPUT, GPIO_SYNC);
	GPIO_SetupPinMux(50,0,7);

	GPIO_SetupPinOptions(51, GPIO_INPUT, GPIO_SYNC);
	GPIO_SetupPinMux(51,0,7);

	//SD Filter 3 , iL3
	GPIO_SetupPinOptions(52, GPIO_INPUT, GPIO_SYNC);
	GPIO_SetupPinMux(52,0,7);

	GPIO_SetupPinOptions(53, GPIO_INPUT, GPIO_SYNC);
	GPIO_SetupPinMux(53,0,7);

	/*******************************************************/
	/* Input Control Module */
	/*******************************************************/
	//Configure Input Control Mode: Modulator Clock rate = Modulator data rate
	Sdfm_configureInputCtrl(1,FILTER1,MODE_0);
	Sdfm_configureInputCtrl(1,FILTER2,MODE_0);
	Sdfm_configureInputCtrl(1,FILTER3,MODE_0);

	/*******************************************************/
	/* Comparator Module */
	/*******************************************************/
	// comparator module is setup in the board protection function

	/*******************************************************/
	/* Sinc filter Module */
	/*******************************************************/
	//Configure Data filter modules filter type, OSR value and enable / disable data filter
	// 16 bit data representation is chosen for OSR 128 using Sinc3, from the table in the TRM
	// the max value represented for OSR 128 using sinc 3 is +/-2097152 i.e. 2^21
	// the max value represented for OSR 64 using sinc 3 is +/-262144 i.e. 2^18
	// to represent this in 16 bit format where the first bit is sign shift by 6 bits
	Sdfm_configureData_filter(1, FILTER1, FILTER_ENABLE, SINC3, sdfmosr-1, DATA_32_BIT, SHIFT_0_BITS );
	Sdfm_configureData_filter(1, FILTER2, FILTER_ENABLE, SINC3, sdfmosr-1, DATA_32_BIT,  SHIFT_0_BITS );
	Sdfm_configureData_filter(1, FILTER3, FILTER_ENABLE, SINC3, sdfmosr-1, DATA_32_BIT,  SHIFT_0_BITS );

	//PWM11.CMPC, PWM11.CMPD, PWM12.CMPC and PWM12.CMPD signals cannot synchronize the filters. This option is not being used in this example.
	Sdfm_configureExternalreset(1,FILTER_1_EXT_RESET_ENABLE, FILTER_2_EXT_RESET_ENABLE, FILTER_3_EXT_RESET_ENABLE, FILTER_4_EXT_RESET_ENABLE);

	/*******************************************************/
	/* Enable interrupts */
	/*******************************************************/
	//Following SDFM interrupts can be enabled / disabled using this function.
	//	Enable / disable comparator high threshold
	//  Enable / disable comparator low threshold
	//  Enable / disable modulator clock failure
	//  Enable / disable filter acknowledge
	Sdfm_configureInterrupt(1, FILTER1, IEH_ENABLE, IEL_ENABLE, MFIE_ENABLE, AE_ENABLE);
	Sdfm_configureInterrupt(1, FILTER2, IEH_ENABLE, IEL_ENABLE, MFIE_ENABLE, AE_ENABLE);
	Sdfm_configureInterrupt(1, FILTER3, IEH_ENABLE, IEL_ENABLE, MFIE_ENABLE, AE_ENABLE);

	// Enable master filter bit of the SDFM module 1
	Sdfm_enableMFE(1);
	Sdfm_enableMIE(1);
}


//TODO configureECAPforSDFMClk()
void configureECAPforSDFMClk(uint16_t SD_clk_ecap_sys_ticks)
{
	//Use CAP as source for the SD Clock
	// PWM3A -> OUTPUTXBAR3 -> GPIO4
	// PWM4A -> OUTPUTXBAR4 -> GPIO6

	EALLOW;

	OutputXbarRegs.OUTPUT1MUX0TO15CFG.bit.MUX0 = 3;	// Select ECAP1.OUT on Mux0
	OutputXbarRegs.OUTPUT1MUXENABLE.bit.MUX0 = 1;	// Enable MUX0 for ECAP1.OUT

	GPIO_SetupPinOptions(24, GPIO_OUTPUT, GPIO_SYNC);
	GPIO_SetupPinMux(24,0,1); // set to OUTPUTXBAR1



	// Setup APWM mode on CAP1, set period and compare registers
	ECap1Regs.ECCTL2.bit.CAP_APWM = 1;	// Enable APWM mode
	ECap1Regs.CAP1 = SD_clk_ecap_sys_ticks-1;			// Set Period value
	ECap1Regs.CAP2 = SD_clk_ecap_sys_ticks>>1;			// Set Compare value
	// set next duty cycle to 50%
	ECap1Regs.CAP3 = SD_clk_ecap_sys_ticks-1;
	ECap1Regs.CAP4 = SD_clk_ecap_sys_ticks>>1;
	ECap1Regs.ECCLR.all = 0x0FF;			// Clear pending __interrupts
	// Start counters
	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;

	EDIS;
}

//TODO configurePWMsyncforSDFM()
void configurePWMsyncforSDFM(uint16_t PWM_ticks, uint16_t PWM_ticks_in_sdfm_osr)
{
	configurePWM1chUpCnt(11,PWM_ticks);

	EPwm3Regs.TBCTL.bit.SYNCOSEL=TB_SYNC_IN;
	EPwm3Regs.TBCTL.bit.PHSEN=TB_ENABLE;
	EPwm3Regs.TBPHS.bit.TBPHS=2;
	EPwm3Regs.TBCTL.bit.PHSDIR=TB_UP;

	SyncSocRegs.SYNCSELECT.bit.EPWM4SYNCIN=0; 	 //EPwm1SyncOut

	EPwm4Regs.TBCTL.bit.SYNCOSEL=TB_SYNC_IN;
	EPwm5Regs.TBCTL.bit.SYNCOSEL=TB_SYNC_IN;

	SyncSocRegs.SYNCSELECT.bit.EPWM10SYNCIN=0;  //EPwm1Sync Out
	EPwm10Regs.TBCTL.bit.SYNCOSEL=TB_SYNC_IN;

	EPwm11Regs.TBCTL.bit.PHSEN=TB_ENABLE;
	EPwm11Regs.TBPHS.bit.TBPHS=2;
	EPwm11Regs.TBCTL.bit.PHSDIR=TB_UP;


	//EPwm11Regs.CMPC=(EPwm11Regs.TBPRD>>1-PWM_ticks_in_sdfm_osr-PWM_ticks_in_sdfm_osr>>1);
	//EPwm11Regs.CMPD=(EPwm11Regs.TBPRD>>1-PWM_ticks_in_sdfm_osr-PWM_ticks_in_sdfm_osr>>1);


//	EPwm11Regs.CMPC=(EPwm11Regs.TBPRD>>1-PWM_ticks_in_sdfm_osr-PWM_ticks_in_sdfm_osr>>1);
//	EPwm11Regs.CMPD=(EPwm11Regs.TBPRD>>1-PWM_ticks_in_sdfm_osr-PWM_ticks_in_sdfm_osr>>1);

//	EPwm11Regs.CMPA.bit.CMPA= EPwm11Regs.TBPRD>>1 + PWM_ticks_in_sdfm_osr+ (PWM_ticks_in_sdfm_osr>>1) - 10; // 40 cycles advance so the ISR can begin executing as soon as the data for the current conversion is ready


	EPwm11Regs.CMPC = 1750;
	EPwm11Regs.CMPD = 1750;
	EPwm11Regs.CMPA.bit.CMPA= 1250;
}



//TODO configurePWM1chUpCnt()
void configurePWM1chUpCnt(int16 n, Uint16 period)
{
	EALLOW;
	// Time Base SubModule Registers
	(*ePWM[n]).TBCTL.bit.PRDLD = TB_IMMEDIATE; // set Immediate load
	(*ePWM[n]).TBPRD = period-1; // PWM frequency = 1 / period
	(*ePWM[n]).TBPHS.bit.TBPHS = 0;
	(*ePWM[n]).TBCTR = 0;
	(*ePWM[n]).TBCTL.bit.CTRMODE = TB_COUNT_UP;
	(*ePWM[n]).TBCTL.bit.HSPCLKDIV = TB_DIV1;
	(*ePWM[n]).TBCTL.bit.CLKDIV = TB_DIV1;

	(*ePWM[n]).TBCTL.bit.PHSEN = TB_DISABLE;
	(*ePWM[n]).TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // sync "down-stream"

	// Counter Compare Submodule Registers
	(*ePWM[n]).CMPA.bit.CMPA = 0; // set duty 0% initially
	(*ePWM[n]).CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	(*ePWM[n]).CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;

	// Action Qualifier SubModule Registers
	(*ePWM[n]).AQCTLA.bit.CAU = AQ_CLEAR;
	(*ePWM[n]).AQCTLA.bit.ZRO = AQ_SET;
	EDIS;
}

void setupComparatorSubsystem(uint16_t cmpss_no, float current_limit, float current_max_sense)
{
	EALLOW;
	// LEM Current goes ADC A2/ CMPSS 1
	//Enable CMPSS1
	CMPSS[cmpss_no]->COMPCTL.bit.COMPDACE=1;

	//NEG signal comes from DAC for the low comparator
	CMPSS[cmpss_no]->COMPCTL.bit.COMPLSOURCE=NEGIN_DAC;

	//NEG signal comes from DAC for the high comparator
	CMPSS[cmpss_no]->COMPCTL.bit.COMPHSOURCE=NEGIN_DAC;

	//Use VDDA as the reference for comparator DACs
	CMPSS[cmpss_no]->COMPDACCTL.bit.SELREF= REFERENCE_VDDA;

	//Set DAC to H~75% and L ~25% values
	CMPSS[cmpss_no]->DACHVALS.bit.DACVAL=2048+(int16_t)((float)current_limit*(float)2048.0/(float)current_max_sense);
	CMPSS[cmpss_no]->DACLVALS.bit.DACVAL=2048-(int16_t)((float)current_limit*(float)2048.0/(float)current_max_sense);

	// comparator oputput is "not" inverted for high compare event
	CMPSS[cmpss_no]->COMPCTL.bit.COMPHINV=0;

	// Comparator output is inverted for for low compare event
	CMPSS[cmpss_no]->COMPCTL.bit.COMPLINV=1;

	// Configure Digital Filter
	//Maximum CLKPRESCALE value provides the most time between samples
	CMPSS[cmpss_no]->CTRIPHFILCLKCTL.bit.CLKPRESCALE = 0x10;

	//Maximum SAMPWIN value provides largest number of samples
	CMPSS[cmpss_no]->CTRIPHFILCTL.bit.SAMPWIN        = 0x1F;

	//Maximum THRESH value requires static value for entire window
	//  THRESH should be GREATER than half of SAMPWIN
	CMPSS[cmpss_no]->CTRIPHFILCTL.bit.THRESH         = 0x0F;

	//Reset filter logic & start filtering
	CMPSS[cmpss_no]->CTRIPHFILCTL.bit.FILINIT        = 1;

	// Configure CTRIPOUT path
	//Digital filter output feeds CTRIPH and CTRIPOUTH
	CMPSS[cmpss_no]->COMPCTL.bit.CTRIPHSEL           = CTRIP_FILTER;
	CMPSS[cmpss_no]->COMPCTL.bit.CTRIPOUTHSEL        = CTRIP_FILTER;
	CMPSS[cmpss_no]->COMPCTL.bit.CTRIPLSEL           = CTRIP_FILTER;
	CMPSS[cmpss_no]->COMPCTL.bit.CTRIPOUTLSEL        = CTRIP_FILTER;

	// Make sure the asynchronous path compare high and low event
	// does not go to the OR gate with latched digital filter output
	CMPSS[cmpss_no]->COMPCTL.bit.ASYNCHEN=0;
	CMPSS[cmpss_no]->COMPCTL.bit.ASYNCLEN=0;

	//Comparator hysteresis control , set to 2x typical value
	CMPSS[cmpss_no]->COMPHYSCTL.bit.COMPHYS=2;
	// Dac value is updated on sysclock
	CMPSS[cmpss_no]->COMPDACCTL.bit.SWLOADSEL=0;
	// ramp is bypassed
	CMPSS[cmpss_no]->COMPDACCTL.bit.DACSOURCE=0;

	// Clear the latched comparator events
	CMPSS[cmpss_no]->COMPSTSCLR.bit.HLATCHCLR=1;
	CMPSS[cmpss_no]->COMPSTSCLR.bit.LLATCHCLR=1;

	EDIS;
}

void setupPWMTrip(uint16_t pwm_no )
{
	// Now enable TRIP 4 to trip the PWM of the vienna rectifier
	EALLOW;

	// TRIP4 is the source for DCAEVT1
	// TRIP5 is the source for DCAEVT2

	(*ePWM[pwm_no]).DCTRIPSEL.bit.DCAHCOMPSEL=3; //Trip 4 is the input to the DCAHCOMPSEL
	(*ePWM[pwm_no]).TZDCSEL.bit.DCAEVT1=TZ_DCAH_HI;
	(*ePWM[pwm_no]).DCACTL.bit.EVT1SRCSEL= 0;
	(*ePWM[pwm_no]).DCACTL.bit.EVT1FRCSYNCSEL=DC_EVT_SYNC  ; // this forces scnchronization before accepting the trip
//	(*ePWM[pwm_no]).TZSEL.bit.DCAEVT1=1;
	(*ePWM[pwm_no]).TZCTL.bit.DCAEVT1 = TZ_NO_CHANGE;


	(*ePWM[pwm_no]).DCTRIPSEL.bit.DCBHCOMPSEL=4; //Trip 5 is the input to the DCBHCOMPSEL
	(*ePWM[pwm_no]).TZDCSEL.bit.DCBEVT1=TZ_DCBH_HI;
	(*ePWM[pwm_no]).DCBCTL.bit.EVT1SRCSEL= 0;
	(*ePWM[pwm_no]).DCBCTL.bit.EVT1FRCSYNCSEL=DC_EVT_SYNC  ; //this forces synchronization before accepting the trip
	(*ePWM[pwm_no]).TZSEL.bit.DCBEVT1=1;
//	(*ePWM[pwm_no]).TZCTL.bit.DCBEVT1 = TZ_NO_CHANGE; // EPWMxB will go low

	(*ePWM[pwm_no]).TZSEL.bit.CBC6=0x1; // Emulator Stop

	// What do we want the OST/CBC events to do?
	// TZA events can force EPWMxA
	// TZB events can force EPWMxB

	(*ePWM[pwm_no]).TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
	(*ePWM[pwm_no]).TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low

	// Clear any spurious trip
	(*ePWM[pwm_no]).TZCLR.bit.DCBEVT1=1;
	(*ePWM[pwm_no]).TZCLR.bit.DCAEVT1=1;
	(*ePWM[pwm_no]).TZCLR.bit.OST = 1;


	(*ePWM[pwm_no]).TZEINT.bit.DCAEVT1=1;
	//(*ePWM[pwm_no]).TZEINT.bit.DCBEVT1=1;
	// force a PWM Trip
	(*ePWM[pwm_no]).TZFRC.bit.OST=1;

	EDIS;
}

//TODO setupBoardProtection()
void setupBoardProtection(uint16_t current_sense, float current_limit, float current_max_sense)
{
	EALLOW;
	// configure TRIP4 for SDFM

	// Reset the MUX CFG
	EPwmXbarRegs.TRIP4MUX0TO15CFG.all=0x0000;
	EPwmXbarRegs.TRIP4MUX16TO31CFG.all=0x0000;
	// Disable all the muxes first
	EPwmXbarRegs.TRIP4MUXENABLE.all=0x0000;
	EALLOW;

	Sdfm_clearFlagRegister(1,0x8000FFFF);

	Sdfm_configureComparator ( 1, FILTER1, SINC3, OSR_32,
			((32768/2)+(int16_t)((float)current_limit*(float)16384/(float)I_MAX_SENSE_SD)),
			((32768/2)-(int16_t)((float)current_limit*(float)16384/(float)I_MAX_SENSE_SD)));

	Sdfm_configureComparator(1, FILTER2, SINC3, OSR_32,
			((32768/2)+(int16_t)((float)current_limit*(float)16384/(float)I_MAX_SENSE_SD)),
			((32768/2)-(int16_t)((float)current_limit*(float)16384/(float)I_MAX_SENSE_SD)));

	Sdfm_configureComparator(1, FILTER3, SINC3, OSR_32,
			((32768/2)+(int16_t)((float)current_limit*(float)16384/(float)I_MAX_SENSE_SD)),
			((32768/2)-(int16_t)((float)current_limit*(float)16384/(float)I_MAX_SENSE_SD)));

	Sdfm_clearFlagRegister(1,0x8000FFFF);

	EALLOW;
	EPwmXbarRegs.TRIP4MUX16TO31CFG.bit.MUX16=1; // select the 2nd option of MUx 16
	EPwmXbarRegs.TRIP4MUX16TO31CFG.bit.MUX18=1; // select the 2nd option of MUx 18
	EPwmXbarRegs.TRIP4MUX16TO31CFG.bit.MUX20=1; // select the 2nd option of MUx 20

	// Enable Mux 16,18 and 20, option 2 SD1FLT1/2/3.COMPH_OR_COMPL to generate TRIP4
	EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX16=1;
	EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX18=1;
	EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX20=1;
	EDIS;



	// configure TRIP5 for ADC comparator based trip

	// Reset the MUX CFG
	EPwmXbarRegs.TRIP5MUX0TO15CFG.all=0x0000;
	EPwmXbarRegs.TRIP5MUX16TO31CFG.all=0x0000;
	// Disable all the muxes first
	EPwmXbarRegs.TRIP5MUXENABLE.all=0x0000;
	EDIS;

	setupComparatorSubsystem(1, (current_limit), current_max_sense);
	setupComparatorSubsystem(2, (current_limit), current_max_sense);
	setupComparatorSubsystem(4, (current_limit), current_max_sense);

	EALLOW;

	// see TRM ePWM X-Bar MUX Configuration Table
	// Enable Mux 0,option 2 CMPSS1.CTRIPH_OR_CTRIPL to generate TRIP5
	EPwmXbarRegs.TRIP5MUX0TO15CFG.bit.MUX0=1;
	EPwmXbarRegs.TRIP5MUXENABLE.bit.MUX0=1;

	// Enable Mux 2,option 2 CMPSS2.CTRIPH_OR_CTRIPL to generate TRIP5
	EPwmXbarRegs.TRIP5MUX0TO15CFG.bit.MUX2=1;
	EPwmXbarRegs.TRIP5MUXENABLE.bit.MUX2=1;

	// Enable Mux 6,option 2 CMPSS4.CTRIPH_OR_CTRIPL to generate TRIP5
	EPwmXbarRegs.TRIP5MUX0TO15CFG.bit.MUX6=1;
	EPwmXbarRegs.TRIP5MUXENABLE.bit.MUX6=1;

	EDIS;



	setupPWMTrip(1);
	setupPWMTrip(2);
	setupPWMTrip(3);


//************************** End of Prot. Conf. ***************************//
}




//TODO calibrateOffset()
void calibrateOffset(volatile float *iL1MeasOffset, volatile float *iL2MeasOffset, volatile float *iL3MeasOffset ,
		volatile float *v1MeasOffset, volatile float *v2MeasOffset, volatile float *v3MeasOffset ,
		float k1, float k2)
{
	int16_t offsetCalCounter=0;

	EALLOW;

	EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTRU_CMPA; // select CMPC when counting up to trigger the ADC
	EPwm1Regs.ETSEL.bit.SOCASELCMP = 1; // enable the CMPC and CMPD Pulses for the ADC trigger
	EPwm1Regs.CMPC= EPwm1Regs.TBPRD - (ACQPS_SYS_CLKS * 6);
	EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST; /* Generate pulse on 1st even*/
	EPwm1Regs.ETSEL.bit.SOCAEN = 1; /* Enable SOC on A group*/

	//PWM1 INT is used to trigger the ISR
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTRD_CMPB;  	// INT on Zero event
	EPwm1Regs.CMPB.bit.CMPB= EPwm1Regs.TBPRD - (ACQPS_SYS_CLKS <<1);
	EPwm1Regs.ETSEL.bit.INTEN = 1;              // Enable INT
	EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;         // Generate INT on every event

	EPwm1Regs.ETCLR.bit.INT=1;

	EDIS;

	offsetCalCounter=0;
	*iL1MeasOffset=0;
	*iL2MeasOffset=0;
	*iL3MeasOffset=0;

	*v1MeasOffset=0;
	*v2MeasOffset=0;
	*v3MeasOffset=0;

	while(offsetCalCounter<25000)
	{
		if(EPwm1Regs.ETFLG.bit.INT==1)
		{
			if(offsetCalCounter>1000)
			{
				// offset of the inductor current sense
				*iL1MeasOffset = k1*(*iL1MeasOffset) + k2*(IL1_ADC_READ1+IL1_ADC_READ2+IL1_ADC_READ3+IL1_ADC_READ4)*0.25*ADC_PU_SCALE_FACTOR;
				*iL2MeasOffset = k1*(*iL2MeasOffset) + k2*(IL2_ADC_READ1+IL2_ADC_READ2+IL2_ADC_READ3+IL2_ADC_READ4)*0.25*ADC_PU_SCALE_FACTOR;
				*iL3MeasOffset = k1*(*iL3MeasOffset) + k2*(IL3_ADC_READ1+IL3_ADC_READ2+IL3_ADC_READ3+IL3_ADC_READ4)*0.25*ADC_PU_SCALE_FACTOR;

				// offset of the inductor current sense
				*v1MeasOffset = k1*(*v1MeasOffset) + k2*(V1_ADC_READ1+V1_ADC_READ2+V1_ADC_READ3+V1_ADC_READ4)*0.25*ADC_PU_SCALE_FACTOR;
				*v2MeasOffset = k1*(*v2MeasOffset) + k2*(V2_ADC_READ1+V2_ADC_READ2+V2_ADC_READ3+V2_ADC_READ4)*0.25*ADC_PU_SCALE_FACTOR;
				*v3MeasOffset = k1*(*v3MeasOffset) + k2*(V3_ADC_READ1+V3_ADC_READ2+V3_ADC_READ3+V3_ADC_READ4)*0.25*ADC_PU_SCALE_FACTOR;
			}
			EPwm1Regs.ETCLR.bit.INT=1;
			offsetCalCounter++;
		}
	}


// NO use using the PPB because of the over sampling

}

//TODO disablePWMCLKCounting
void disablePWMCLKCounting(void)
{
	EALLOW;
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	EDIS;
}

//TODO enablePWMCLKCounting
void enablePWMCLKCounting(void)
{
	EALLOW;
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	EDIS;
}

//TODO setupLEDGPIO()
void setupLEDGPIO(void)
{
	GPIO_SetupPinOptions(31, GPIO_OUTPUT, GPIO_ASYNC);
	GPIO_SetupPinMux(31,0,0);

	GPIO_SetupPinOptions(34, GPIO_OUTPUT, GPIO_ASYNC);
	GPIO_SetupPinMux(34,0,0);
}

//TODO toggleLED()
void toggleLED(void)
{
	static uint16_t ledCnt1=0;

	if(ledCnt1==0)
		{
		GpioDataRegs.GPBTOGGLE.bit.GPIO34=1;
		ledCnt1=10;
		}
	else
		ledCnt1--;
}

//TODO setupProfilingGPIO()
void setupProfilingGPIO(void)
{	//To profile use GPIO 40 and GPIO41
	GPIO_SetupPinOptions(12, GPIO_OUTPUT, GPIO_ASYNC);
	GPIO_SetupPinMux(12,0,0);

	GPIO_SetupPinOptions(13, GPIO_OUTPUT, GPIO_ASYNC);
	GPIO_SetupPinMux(13,0,0);
}

//TODO setupRelayGPIO();
void setupRelayGPIO(void)
{
	// Configure GPIO 5 as Output, this goes to the Relay
	GPIO_SetupPinOptions(5,GPIO_OUTPUT, GPIO_ASYNC);
	GPIO_SetupPinMux(5,0,0);;
}

//TODO setupSCIconnectionForSFRA()
void setupSCIconnectionForSFRA()
{

	// Use Gpio 28 and 29 for the SCI comms through JTAG
	EALLOW;
	GPIO_SetupPinMux(28, GPIO_MUX_CPU1, 1);
	GPIO_SetupPinOptions(28, GPIO_INPUT, GPIO_SYNC);
	GPIO_SetupPinMux(29, GPIO_MUX_CPU1, 1);
	GPIO_SetupPinOptions(29, GPIO_OUTPUT, GPIO_SYNC);
	EDIS;

	// 50000000 is the LSPCLK or the Clock used for the SCI Module
	// 57600 is the Baudrate desired of the SCI module
	SCIA_Init(50000000, 57600);
}

void configure3phViennaPWM(uint16_t inv_pwm_no, uint16_t pwm_period_ticks)
{
	EALLOW;
	// PWM clock on F2837x is divided by 2
	// ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV=1
	configurePWMUpDwnCnt(inv_pwm_no,pwm_period_ticks);
	configurePWMUpDwnCnt(inv_pwm_no+1,pwm_period_ticks);
	configurePWMUpDwnCnt(inv_pwm_no+2,pwm_period_ticks);

	(*ePWM[inv_pwm_no]).TBCTL.bit.PHSEN = TB_DISABLE;
	(*ePWM[inv_pwm_no]).TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // sync "down-stream"

	// configure next PWM  as slaves and let it pass the sync in pulse from PWM1
	ePWM[inv_pwm_no+1]->TBCTL.bit.SYNCOSEL=TB_SYNC_IN;
	ePWM[inv_pwm_no+1]->TBCTL.bit.PHSEN=TB_ENABLE;
	ePWM[inv_pwm_no+1]->TBPHS.bit.TBPHS=2;
	ePWM[inv_pwm_no+1]->TBCTL.bit.PHSDIR=TB_UP;

	ePWM[inv_pwm_no+2]->TBCTL.bit.SYNCOSEL=TB_SYNC_IN;
	ePWM[inv_pwm_no+2]->TBCTL.bit.PHSEN=TB_ENABLE;
	ePWM[inv_pwm_no+2]->TBPHS.bit.TBPHS=2;
	ePWM[inv_pwm_no+2]->TBCTL.bit.PHSDIR=TB_UP;

	EDIS;
}


//TODO configurePWM1chUpDwnCnt()
void configurePWMUpDwnCnt(uint16_t inv_pwm_no, uint16_t pwm_period_ticks)
{
	EALLOW;

	// Time Base SubModule Registers
	(*ePWM[inv_pwm_no]).TBCTL.bit.PRDLD = TB_SHADOW;
	(*ePWM[inv_pwm_no]).TBPRD = pwm_period_ticks >>1 ; // PWM frequency = 1 / period
	(*ePWM[inv_pwm_no]).TBPHS.bit.TBPHS = 0;
	(*ePWM[inv_pwm_no]).TBCTR = 0;
	(*ePWM[inv_pwm_no]).TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	(*ePWM[inv_pwm_no]).TBCTL.bit.HSPCLKDIV = TB_DIV1;
	(*ePWM[inv_pwm_no]).TBCTL.bit.CLKDIV = TB_DIV1;

	// Counter Compare Submodule Registers
	(*ePWM[inv_pwm_no]).CMPA.bit.CMPA = 	(*ePWM[inv_pwm_no]).TBPRD ; // set duty 0% initially
	(*ePWM[inv_pwm_no]).CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	(*ePWM[inv_pwm_no]).CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;

	// Action Qualifier SubModule Registers
	// assuming positive half cycle comes first i.e. dhl>0
	(*ePWM[inv_pwm_no]).AQCTLA.bit.CAU = AQ_SET; // CTR = CMPA@UP , set to 1
	(*ePWM[inv_pwm_no]).AQCTLA.bit.CAD = AQ_CLEAR; // CTR = CMPA@Down , toggle
	(*ePWM[inv_pwm_no]).AQCTLA.bit.ZRO = AQ_CLEAR; // CTR=0, clear to 0

	(*ePWM[inv_pwm_no]).AQCTLB.bit.CAU = AQ_SET; // CTR = CMPA@UP , set to 1
	(*ePWM[inv_pwm_no]).AQCTLB.bit.CAD = AQ_CLEAR; // CTR = CMPA@Down , toggle
	(*ePWM[inv_pwm_no]).AQCTLB.bit.ZRO = AQ_CLEAR; // CTR=0, clear to 0

	EDIS;
}

