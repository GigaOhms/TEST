/* ==============================================================================
System Name:	Power Factor Correction Three Phase Vienna Rectifier

File Name:	  	pfc3phvienna.c

Target:			F2837xD

Author:			Manish Bhardwaj, 21/11/2016

Description:	Project implements power factor correction control algorithm for a three phase vienna rectifier

 	 	 	 	Current and voltages on the design are sensed using the SDFM
 	 	 	 	or ADC selectable through the powerSUITE page.

				The control ISR rate is 50Khz and is triggered by EPWM1

				following is the description of other files that are used by this system
				<solution name>.c -> This file, which has the main.c of the project
				<solution name>.h -> This is the main header file for the project
				<solution name>_settings.h -> powerSUITE generated settings
				<boad name>_board.c -> This is the file that is used to keep common functions
									   related to the board hardware like setting up the PWM, ADC, SDFM,
									   reading data and common variables liked current, voltage etc.

Copyright (C) {2016} Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED*
=================================================================================  */

#include "pfc3phvienna.h"

//*********************** USER Variables *************************************//
// Global variables used in this system specific to this control method
// common variables are kept in <system_name>_board.c
//****************************************************************************//

//TODO  Control Variables

// Measurement Variables
// Inductor Current Measurement
volatile float iL1MeasADC,iL2MeasADC,iL3MeasADC;
volatile float iL1MeasSD,iL2MeasSD,iL3MeasSD;
volatile float iL1Meas,iL2Meas,iL3Meas;
// Inductor Current Measurement Offset
volatile float iL1MeasOffset,iL2MeasOffset,iL3MeasOffset;

// Output Voltage Bus measurement
volatile float vBusMNMeas,vBusPMMeas, vBusMeas, vBusHalfMeas;
volatile float vBusMNMeasAvg,vBusPMMeasAvg, vBusMeasAvg;

// variables used for calibration of output voltage measurements
volatile float m_VBusMNMeas, b_VBusMNMeas;
volatile float m_VBusPMMeas, b_VBusPMMeas;

// Input Grid Voltage Measurement
volatile float v1Meas,v2Meas,v3Meas;
volatile float v1MeasOffset,v2MeasOffset,v3MeasOffset;

// Sine analyzer block for RMS Volt, Curr and Power measurements
sineAnalyzerWithPowerMeas sine_mains1, sine_mains2, sine_mains3;

volatile float vRmsMeasAvg;

// Variables used to calibrate measurement offsets
//Offset filter coefficient K1: 0.05/(T+0.05);
float k1 = 0.998;
//Offset filter coefficient K2: T/(T+0.05)
float k2 = 0.001999;
int16_t offsetCalCounter;
float offset165;

// Display Values
volatile float  guiVbusMN,guiVbusPM, guiVbus,
				guiV1, guiV2, guiV3,
				guiIL1, guiIL2, guiIL3,
				guiIL1sd, guiIL2sd, guiIL3sd;

volatile float guiACFreq;
volatile float guiPrms1,guiPrms2, guiPrms3, guiPrmsTotal;
volatile float guiIrms1,guiIrms2, guiIrms3;
volatile float guiVrms1, guiVrms2, guiVrms3;
volatile float guiPF1, guiPF2, guiPF3;
volatile float guiVA1, guiVA2, guiVA3;

float guiVbusTripLimit=720.0;

uint16_t guiPowerStageStart;
uint16_t guiPowerStageStop;

// PFC Filtered DC bus measurement
volatile float vBusAvg;

volatile float iL1_CalibrationGain=1.0; //0.95999979;
volatile float iL2_CalibrationGain=1.0;
volatile float iL3_CalibrationGain=1.0; // 0.9850000;



//SD Trip Level
// 32 OSR and SINC3 is used for the comparator trip
// Max value is 32768
// As the SD is used to sense positive and negative values offset is 16384
Uint16  HLT = 0x7FFF;
Uint16  LLT = 0x0000;

// Control Variables

// CNTL 2p2z for DC Bus controller
#pragma DATA_SECTION(Gv_vars,"cntl_var_RAM")
#pragma DATA_SECTION(Gv_coeff,"cntl_coeff_RAM")
CNTL_2P2Z_F_VARS Gv_vars;
CNTL_2P2Z_F_COEFFS Gv_coeff;

CNTL_PI_F Gv;

// Reference variables
// Peak value of the Ii, Io current set point
volatile float iLRef, iL1Ref, iL2Ref, iL3Ref;
// DC Bus set point
volatile float vBusRef=0.0;
volatile float vBusRefSlewed=0.0;


// variables for third harmmonic injection
volatile float vMin, vMax;
volatile float thirdHarmonicInjection;

// individual duty cycles for each phase
volatile float duty1PU, duty2PU, duty3PU;
volatile float dutyPU_DC;

// Flags for clearing trips and closing the loops
int16_t closeGiLoop,closeGvLoop, closeGsLoop, clearTrip, firstTimeGvLoop;

// datalogger
DLOG_4CH_F dLog1;
float dBuff1[100], dBuff2[100], dBuff3[100], dBuff4[100];
float dVal1, dVal2, dVal3, dVal4;

volatile int updateCoeff=0;

volatile float Gi1_Out,Gi2_Out,Gi3_Out, Gs_Out;
volatile float Gs_GainKp=1.0;
volatile float Gi_GainKp=GI_GAINKP;

volatile float vBusDiff=0;
volatile float vBusZero=0;

volatile int16_t thirdHarmonicInjectionEnable=1;

volatile float iL1Ref_prev, iL2Ref_prev, iL3Ref_prev;
volatile float inductor_voltage_drop_feedforward1, inductor_voltage_drop_feedforward2, inductor_voltage_drop_feedforward3;


int16_t slewState;

volatile uint16_t BusVoltageSlew=0;

// Stand Alone Flash Image Instrumentation

int16_t i;
int16_t timer1;

//--------------------------------- SFRA Related Variables ----------------------------
float plantMagVect[SFRA_FREQ_LENGTH];
float plantPhaseVect[SFRA_FREQ_LENGTH];
float olMagVect[SFRA_FREQ_LENGTH];
float olPhaseVect[SFRA_FREQ_LENGTH];
float freqVect[SFRA_FREQ_LENGTH];

volatile SFRA_F sfra1;

//extern to access tables in ROM
extern long FPUsinTable[];
//--------------------------------- SFRA GUI Related Variables ----------------------
volatile int16_t SerialCommsTimer;
volatile int16_t CommsOKflg;
//Flag for reinitializing SFRA variables
volatile int16_t initializationFlag;

//GUI support variables
// sets a limit on the amount of external GUI controls - increase as necessary
volatile int16_t *varSetTxtList[16]; //16 textbox controlled variables
volatile int16_t *varSetBtnList[16]; //16 button controlled variables
volatile int16_t *varSetSldrList[16]; //16 slider controlled variables
volatile int16_t *varGetList[16]; //16 variables sendable to GUI
volatile int32_t *arrayGetList[16]; //16 arrays sendable to GUI
volatile uint32_t *dataSetList[16];	//16 32-bit textbox or label controlled variables

//--------------------------- State Machine Related --------------------------------

int16_t	vTimer0[4];			// Virtual Timers slaved off CPU Timer 0 (A events)
int16_t	vTimer1[4]; 		// Virtual Timers slaved off CPU Timer 1 (B events)

// Variable declarations for state machine
void (*Alpha_State_Ptr)(void);	// Base States pointer
void (*A_Task_Ptr)(void);		// State pointer A branch
void (*B_Task_Ptr)(void);		// State pointer B branch

//------------------------------------------------------------------------------------
// Enum for build level of software
enum enum_BuildLevel BuildInfo = BuildLevel1_OpenLoop ;

enum enum_boardState boardState = PowerStageOFF;

enum enum_boardStatus boardStatus = boardStatus_Idle;

//TODO Main
void main(void)
{
	// This routine sets up the basic device configuration such as initializing PLL
	// copying code from FLASH to RAM,
	setupDevice();

	// Tasks State-machine init
	Alpha_State_Ptr = &A0;
	A_Task_Ptr = &A1;
	B_Task_Ptr = &B1;

	// Stop all PWM mode clock
	disablePWMCLKCounting();

	configure3phViennaPWM(PWM_NO,PFC3PH_PWM_PERIOD);

	// power up ADC on the device
	setupADC();
	// setup conversion on ADCA for inverter current and voltages
	// iL1Meas
	setupADCconversion(IL1_ADC_MODULE, IL1_ADC_SOC1, IL1_ADC_PIN, IL1_ADC_TRIG_SOURCE, IL1_ACQPS_SYS_CLKS);
	// iL2Meas
	setupADCconversion(IL2_ADC_MODULE, IL2_ADC_SOC1, IL2_ADC_PIN, IL2_ADC_TRIG_SOURCE, IL2_ACQPS_SYS_CLKS);
	// iL3Meas
	setupADCconversion(IL3_ADC_MODULE, IL3_ADC_SOC1, IL3_ADC_PIN, IL3_ADC_TRIG_SOURCE, IL3_ACQPS_SYS_CLKS);

	// iL1Meas2
	setupADCconversion(IL1_ADC_MODULE, IL1_ADC_SOC2, IL1_ADC_PIN, IL1_ADC_TRIG_SOURCE, IL1_ACQPS_SYS_CLKS);
	// iL2Meas2
	setupADCconversion(IL2_ADC_MODULE, IL2_ADC_SOC2, IL2_ADC_PIN, IL2_ADC_TRIG_SOURCE, IL2_ACQPS_SYS_CLKS);
	// iL3Meas2
	setupADCconversion(IL3_ADC_MODULE, IL3_ADC_SOC2, IL3_ADC_PIN, IL3_ADC_TRIG_SOURCE, IL3_ACQPS_SYS_CLKS);

	// iL1Meas3
	setupADCconversion(IL1_ADC_MODULE, IL1_ADC_SOC3, IL1_ADC_PIN, IL1_ADC_TRIG_SOURCE, IL1_ACQPS_SYS_CLKS);
	// iL2Meas3
	setupADCconversion(IL2_ADC_MODULE, IL2_ADC_SOC3, IL2_ADC_PIN, IL2_ADC_TRIG_SOURCE, IL2_ACQPS_SYS_CLKS);
	// iL3Meas3
	setupADCconversion(IL3_ADC_MODULE, IL3_ADC_SOC3, IL3_ADC_PIN, IL3_ADC_TRIG_SOURCE, IL3_ACQPS_SYS_CLKS);

	// iL1Meas4
	setupADCconversion(IL1_ADC_MODULE, IL1_ADC_SOC4, IL1_ADC_PIN, IL1_ADC_TRIG_SOURCE, IL1_ACQPS_SYS_CLKS);
	// iL2Meas4
	setupADCconversion(IL2_ADC_MODULE, IL2_ADC_SOC4, IL2_ADC_PIN, IL2_ADC_TRIG_SOURCE, IL2_ACQPS_SYS_CLKS);
	// iL3Meas4
	setupADCconversion(IL3_ADC_MODULE, IL3_ADC_SOC4, IL3_ADC_PIN, IL3_ADC_TRIG_SOURCE, IL3_ACQPS_SYS_CLKS);

	// v1Meas
	setupADCconversion(V1_ADC_MODULE, V1_ADC_SOC1, V1_ADC_PIN, V1_ADC_TRIG_SOURCE, V1_ACQPS_SYS_CLKS);
	// v2Meas
	setupADCconversion(V2_ADC_MODULE, V2_ADC_SOC1, V2_ADC_PIN, V2_ADC_TRIG_SOURCE, V2_ACQPS_SYS_CLKS);
	// v3Meas
	setupADCconversion(V3_ADC_MODULE, V3_ADC_SOC1, V3_ADC_PIN, V3_ADC_TRIG_SOURCE, V3_ACQPS_SYS_CLKS);

	// v1Meas2
	setupADCconversion(V1_ADC_MODULE, V1_ADC_SOC2, V1_ADC_PIN, V1_ADC_TRIG_SOURCE, V1_ACQPS_SYS_CLKS);
	// v2Meas2
	setupADCconversion(V2_ADC_MODULE, V2_ADC_SOC2, V2_ADC_PIN, V2_ADC_TRIG_SOURCE, V2_ACQPS_SYS_CLKS);
	// v3Meas2
	setupADCconversion(V3_ADC_MODULE, V3_ADC_SOC2, V3_ADC_PIN, V3_ADC_TRIG_SOURCE, V3_ACQPS_SYS_CLKS);

	// v1Meas3
	setupADCconversion(V1_ADC_MODULE, V1_ADC_SOC3, V1_ADC_PIN, V1_ADC_TRIG_SOURCE, V1_ACQPS_SYS_CLKS);
	// v2Meas3
	setupADCconversion(V2_ADC_MODULE, V2_ADC_SOC3, V2_ADC_PIN, V2_ADC_TRIG_SOURCE, V2_ACQPS_SYS_CLKS);
	// v3Meas3
	setupADCconversion(V3_ADC_MODULE, V3_ADC_SOC3, V3_ADC_PIN, V3_ADC_TRIG_SOURCE, V3_ACQPS_SYS_CLKS);

	// v1Meas4
	setupADCconversion(V1_ADC_MODULE, V1_ADC_SOC4, V1_ADC_PIN, V1_ADC_TRIG_SOURCE, V1_ACQPS_SYS_CLKS);
	// v2Meas4
	setupADCconversion(V2_ADC_MODULE, V2_ADC_SOC4, V2_ADC_PIN, V2_ADC_TRIG_SOURCE, V2_ACQPS_SYS_CLKS);
	// v3Meas4
	setupADCconversion(V3_ADC_MODULE, V3_ADC_SOC4, V3_ADC_PIN, V3_ADC_TRIG_SOURCE, V3_ACQPS_SYS_CLKS);

	// vBusMeas
	setupADCconversion(VBUSPM_ADC_MODULE, VBUSPM_ADC_SOC1, VBUSPM_ADC_PIN, VBUSPM_ADC_TRIG_SOURCE, VBUSPM_ACQPS_SYS_CLKS);
	setupADCconversion(VBUSPM_ADC_MODULE, VBUSPM_ADC_SOC2, VBUSPM_ADC_PIN, VBUSPM_ADC_TRIG_SOURCE, VBUSPM_ACQPS_SYS_CLKS);
	setupADCconversion(VBUSPM_ADC_MODULE, VBUSPM_ADC_SOC3, VBUSPM_ADC_PIN, VBUSPM_ADC_TRIG_SOURCE, VBUSPM_ACQPS_SYS_CLKS);
	setupADCconversion(VBUSPM_ADC_MODULE, VBUSPM_ADC_SOC4, VBUSPM_ADC_PIN, VBUSPM_ADC_TRIG_SOURCE, VBUSPM_ACQPS_SYS_CLKS);

	// vBusMidMeas
	setupADCconversion(VBUSMN_ADC_MODULE, VBUSMN_ADC_SOC1, VBUSMN_ADC_PIN, VBUSMN_ADC_TRIG_SOURCE, VBUSMN_ACQPS_SYS_CLKS);
	setupADCconversion(VBUSMN_ADC_MODULE, VBUSMN_ADC_SOC2, VBUSMN_ADC_PIN, VBUSMN_ADC_TRIG_SOURCE, VBUSMN_ACQPS_SYS_CLKS);
	setupADCconversion(VBUSMN_ADC_MODULE, VBUSMN_ADC_SOC3, VBUSMN_ADC_PIN, VBUSMN_ADC_TRIG_SOURCE, VBUSMN_ACQPS_SYS_CLKS);
	setupADCconversion(VBUSMN_ADC_MODULE, VBUSMN_ADC_SOC4, VBUSMN_ADC_PIN, VBUSMN_ADC_TRIG_SOURCE, VBUSMN_ACQPS_SYS_CLKS);

	setupSDFM(PFC3PH_PWM_PERIOD,PWM_CLK_IN_SDFM_OSR, SD_CLK_COUNT,SDFM_OSR);

	//Profiling GPIO and led GPIOs
	setupProfilingGPIO();

	//configure lED GPIO
	setupLEDGPIO();

	//Control Variable specific to the voltage source inverter Initialization

	CNTL_2P2Z_F_VARS_init(&Gv_vars);
	CNTL_2P2Z_F_COEFFS_init(&Gv_coeff);

	Gv_coeff.Coeff_A1 = (float)(CNTL_GV_A1);
	Gv_coeff.Coeff_A2 = (float)(CNTL_GV_A2);
	Gv_coeff.Coeff_B0 = (float)(CNTL_GV_B0);
	Gv_coeff.Coeff_B1 = (float)(CNTL_GV_B1);
	Gv_coeff.Coeff_B2 = (float)(CNTL_GV_B2);
	Gv_coeff.IMin	 = (float)(CNTL_GV_IMIN);
	Gv_coeff.Min	= (float)(CNTL_GV_MIN);
	Gv_coeff.Max	= (float)(CNTL_GV_MAX);

	CNTL_PI_F_init(&Gv);
	Gv.Ki=0.04;
	Gv.Kp=1.0;
	Gv.Umax=CNTL_GV_MAX;
	Gv.Umin=CNTL_GV_MIN;

	setupSFRA();

	//sine analyzer initialization
	sineAnalyzerWithPowerMeas_init(&sine_mains1);
	sine_mains1.sampleFreq = (float32)(ISR_10KHZ_FREQUENCY);
	sine_mains1.threshold = (float32)(0.08);
	sine_mains1.nSamplesMax=ISR_10KHZ_FREQUENCY/UNIVERSAL_GRID_MIN_FREQ;
	sine_mains1.nSamplesMin=ISR_10KHZ_FREQUENCY/UNIVERSAL_GRID_MAX_FREQ;
	sine_mains1.emaFilterMultiplier=2.0/ISR_10KHZ_FREQUENCY;

	sineAnalyzerWithPowerMeas_init(&sine_mains2);
	sine_mains2.sampleFreq = (float32)(ISR_10KHZ_FREQUENCY);
	sine_mains2.threshold = (float32)(0.08);
	sine_mains2.nSamplesMax=ISR_10KHZ_FREQUENCY/UNIVERSAL_GRID_MIN_FREQ;
	sine_mains2.nSamplesMin=ISR_10KHZ_FREQUENCY/UNIVERSAL_GRID_MAX_FREQ;
	sine_mains2.emaFilterMultiplier=2.0/ISR_10KHZ_FREQUENCY;

	sineAnalyzerWithPowerMeas_init(&sine_mains3);
	sine_mains3.sampleFreq = (float32)(ISR_10KHZ_FREQUENCY);
	sine_mains3.threshold = (float32)(0.08);
	sine_mains3.nSamplesMax=ISR_10KHZ_FREQUENCY/UNIVERSAL_GRID_MIN_FREQ;
	sine_mains3.nSamplesMin=ISR_10KHZ_FREQUENCY/UNIVERSAL_GRID_MAX_FREQ;
	sine_mains3.emaFilterMultiplier=2.0/ISR_10KHZ_FREQUENCY;

	// Initialize global variables generic to the board like ones used to read current values etc
	initGlobalVariables();

	// Enable PWM Clocks
	enablePWMCLKCounting();

	// Offset Calibration Routine
#if SENSING_OPTION ==ADC_BASED_SENSING
	calibrateOffset(&iL1MeasOffset, &iL2MeasOffset, &iL3MeasOffset,
			&v1MeasOffset, &v2MeasOffset, &v3MeasOffset, k1 , k2);
#endif

	// clear any spurious flags in the SDFM module
	// setup protection and trips for the board
	setupBoardProtection(SENSING_OPTION, I_TRIP_LIMIT,I_MAX_SENSE);

	// PWM were tripped low in the previoud routine

	// now it is safe to enable them as PWM, untill now they will be GPIO input
	// and because of the res pull down all will be drivern low
	InitEPwm1Gpio();
	InitEPwm2Gpio();
	InitEPwm3Gpio();

	// ISR Mapping
	setupInterrupt();

// IDLE loop. Just sit and loop forever, periodically will branch into A0-A3, B0-B3, C0-C3 tasks
// Frequency of this brnaching is set in setupDevice routine:
	for(;;)  //infinite loop
	{
		// State machine entry & exit point
		//===========================================================
		(*Alpha_State_Ptr)();	// jump to an Alpha state (A0,B0,...)
		//===========================================================
		if(initializationFlag == 1)
		{
		//	SFRA_F_INIT(&SFRA1);
			initializationFlag = 0;
			sfra1.start = 1;
		}
	}
} //END MAIN CODE



//=================================================================================
//	STATE-MACHINE SEQUENCING AND SYNCRONIZATION FOR SLOW BACKGROUND TASKS
//=================================================================================

//--------------------------------- SFRAMEWORK -------------------------------------
void A0(void)
{
	// loop rate synchronizer for A-tasks
	if(CpuTimer0Regs.TCR.bit.TIF == 1)
	{
		CpuTimer0Regs.TCR.bit.TIF = 1;	// clear flag

		//-----------------------------------------------------------
		(*A_Task_Ptr)();		// jump to an A Task (A1,A2,A3,...)
		//-----------------------------------------------------------

		vTimer0[0]++;			// virtual timer 0, instance 0 (spare)
		SerialCommsTimer++;
	}



	Alpha_State_Ptr = &B0;		// Comment out to allow only A tasks
}

void B0(void)
{
	// loop rate synchronizer for B-tasks
	if(CpuTimer1Regs.TCR.bit.TIF == 1)
	{
		CpuTimer1Regs.TCR.bit.TIF = 1;				// clear flag

		//-----------------------------------------------------------
		(*B_Task_Ptr)();		// jump to a B Task (B1,B2,B3,...)
		//-----------------------------------------------------------
		vTimer1[0]++;			// virtual timer 1, instance 0 (spare)
	}

	Alpha_State_Ptr = &A0;		// Allow C state tasks
}


//=================================================================================
//	A - TASKS (executed in every 1 msec)
//=================================================================================
//--------------------------------------------------------
void A1(void) // SPARE (not used)
//--------------------------------------------------------
{

	SerialHostComms();
	SFRA_F_BACKGROUND(&sfra1);


	//-------------------
	//the next time CpuTimer0 'counter' reaches Period value go to A2
	A_Task_Ptr = &A1;
	//-------------------
}

//=================================================================================
//	B - TASKS (executed in every 5 msec)
//=================================================================================

//----------------------------------- USER ----------------------------------------

//----------------------------------------
void B1(void)
//----------------------------------------
{
	//updateBoardStatus();
#if INCR_BUILD==1
	BuildInfo=BuildLevel1_OpenLoop;
#elif INCR_BUILD==2
	BuildInfo=BuildLevel2_CurrentLoop;
#elif INCR_BUILD==3
	BuildInfo=BuildLevel3_VoltageAndCurrentLoop;
#elif INCR_BUILD==4
	BuildInfo=BuildLevel4_BalanceVoltageAndCurrentLoop;
#endif
	//-----------------
	//the next time CpuTimer1 'counter' reaches Period value go to B2
	B_Task_Ptr = &B2;	
	//-----------------
}

//----------------------------------------
void B2(void) //  SPARE
//----------------------------------------
{
	toggleLED();

	if(EPwm1Regs.TZFLG.bit.DCBEVT1==1)
		{
			if(boardStatus==boardStatus_NoFault)
				boardStatus = 	boardStatus_OverCurrentTrip;
		}

	//-----------------
	//the next time CpuTimer1 'counter' reaches Period value go to B3
	B_Task_Ptr = &B3;
	//-----------------
}

//----------------------------------------
void B3(void) //  SPARE
//----------------------------------------
{

	if(updateCoeff==1)
	{
		DINT;

		EINT;
		updateCoeff=0;
	}

	//-----------------
	//the next time CpuTimer1 'counter' reaches Period value go to B1
	B_Task_Ptr = &B1;	
	//-----------------
}


//TODO control ISR Code
interrupt void controlISR(void)
{

	setProfilingGPIO();

	readCurrVolADCSignals();

	//readCurrVolSDFMSignals(SD32_PU_SCALE_FACTOR);

#if(ALL_PHASE_ENABLED==1)
	clearPWMTrip();
#else
	if (clearTrip==1 )
		{
		EALLOW;
#if CHECK_PHASE == 1
			// clear all the configured trip sources for the PWM module 1
			EPwm1Regs.TZCLR.bit.OST=0x1;
			EPwm1Regs.TZCLR.bit.CBC=0x1;
			EPwm1Regs.TZCLR.bit.DCAEVT1=0x1;
#elif CHECK_PHASE == 2
			// clear all the configured trip sources for the PWM module 2
			EPwm2Regs.TZCLR.bit.OST=0x1;
			EPwm2Regs.TZCLR.bit.CBC=0x1;
			EPwm2Regs.TZCLR.bit.DCAEVT1=0x1;
			clearTrip=0;
#elif CHECK_PHASE == 3
			// clear all the configured trip sources for the PWM module 2
			EPwm3Regs.TZCLR.bit.OST=0x1;
			EPwm3Regs.TZCLR.bit.CBC=0x1;
			EPwm3Regs.TZCLR.bit.DCAEVT1=0x1;
#else
#warning define CHECK_PHASE in this build
#endif
		EDIS;
		}
#endif

	//TODO BUILD 1
	//-----------------------------------------------------------------------------------------
	#if (INCR_BUILD == 1 ) // Open Loop Check
	//-----------------------------------------------------------------------------------------
		duty1PU = SFRA_F_INJECT(dutyPU_DC);
		duty2PU = dutyPU_DC;
		duty3PU = dutyPU_DC;

	#endif // (INCR_BUILD == 1)


	//TODO BUILD 2
	//-----------------------------------------------------------------------------------------
	#if (INCR_BUILD == 2 || INCR_BUILD==3 || INCR_BUILD==4) 	//Closed Current Loop
	//-----------------------------------------------------------------------------------------

	iL1Ref= iLRef * (v1Meas/vRmsMeasAvg);
	iL2Ref= iLRef* (v2Meas/vRmsMeasAvg);
	iL3Ref= iLRef* (v3Meas/vRmsMeasAvg);



#if INDUCTOR_VOLTAGE_DROP_FEEDFORWARD ==1
	// inductor voltage drop feed forward (L*di/dt)
	inductor_voltage_drop_feedforward1 = (iL1Ref - iL1Ref_prev)* ( PFC_INDUCTOR_VALUE *ISR_CONTROL_FREQUENCY * I_MAX_SENSE / V_MAX_SENSE);
	inductor_voltage_drop_feedforward2 = (iL2Ref - iL2Ref_prev)* ( PFC_INDUCTOR_VALUE *ISR_CONTROL_FREQUENCY * I_MAX_SENSE / V_MAX_SENSE);
	inductor_voltage_drop_feedforward3 = (iL3Ref - iL3Ref_prev)* ( PFC_INDUCTOR_VALUE *ISR_CONTROL_FREQUENCY * I_MAX_SENSE / V_MAX_SENSE);
#else
	inductor_voltage_drop_feedforward1 = 0;
	inductor_voltage_drop_feedforward2 = 0;
	inductor_voltage_drop_feedforward3 = 0;
#endif

		if(closeGiLoop==1)
		{

#if SFRA_TYPE==SFRA_CURRENT
			Gi1_Out= (iL1Meas - SFRA_F_INJECT(iL1Ref))* Gi_GainKp;
#else
			Gi1_Out= (iL1Meas - iL1Ref)* Gi_GainKp;
#endif
			Gi2_Out= (iL2Meas - iL2Ref)* Gi_GainKp;

			Gi3_Out= (iL3Meas - iL3Ref)* Gi_GainKp;


#if THIRD_HARMONIC_INJECTION == 0
	thirdHarmonicInjection=0;
#else
	vMin= (v1Meas<v2Meas)?v1Meas:v2Meas;
	vMin= (vMin<v3Meas)?vMin:v3Meas;
	vMax= (v1Meas>v2Meas)?v1Meas:v2Meas;
	vMax= (vMax>v3Meas)?vMax:v3Meas;

	thirdHarmonicInjection = (vMin+vMax)*0.5;
#endif


			duty1PU = ( (Gi1_Out
							+ inductor_voltage_drop_feedforward1
							+ v1Meas
							- thirdHarmonicInjection
							) / vBusHalfMeas ) - Gs_Out;

			duty2PU = ( (Gi2_Out
							+ inductor_voltage_drop_feedforward2
							+ v2Meas
							- thirdHarmonicInjection
							) / vBusHalfMeas ) - Gs_Out;


			duty3PU = ( (Gi3_Out
							+ inductor_voltage_drop_feedforward3
							+ v3Meas
							- thirdHarmonicInjection
							 ) / vBusHalfMeas ) - Gs_Out;
		}
		else
		{
			duty1PU=1.0;
			duty2PU=1.0;
			duty3PU=1.0;
		}

	#endif

	//--------------------------------------------------------------------------------
	// PWM Driver for three phase Vienna Rectifier PFC
	//--------------------------------------------------------------------------------
	duty1PU = (duty1PU>1.0)?1.0:duty1PU;
	duty1PU = (duty1PU<-1.0)?-1.0:duty1PU;
	duty2PU = (duty2PU>1.0)?1.0:duty2PU;
	duty2PU = (duty2PU<-1.0)?-1.0:duty2PU;
	duty3PU = (duty3PU>1.0)?1.0:duty3PU;
	duty3PU = (duty3PU<-1.0)?-1.0:duty3PU;

	updatePFCviennaPWM(duty1PU, duty2PU, duty3PU, PWM_NO);

	iL1Ref_prev=iL1Ref;
	iL2Ref_prev=iL2Ref;
	iL3Ref_prev=iL3Ref;
//TODO BUILD 3
//-----------------------------------------------------------------------------------------
#if(INCR_BUILD ==3 || INCR_BUILD ==4)
//-----------------------------------------------------------------------------------------
	if(closeGvLoop==1)
	{
		if(firstTimeGvLoop==1)
		{
			vBusRefSlewed=vBusMeas;
			firstTimeGvLoop=0;
		}

#if SFRA_TYPE==SFRA_VOLTAGE
		Gv_vars.Ref = SFRA_F_INJECT(vBusRefSlewed);
#else
		Gv_vars.Ref = vBusRefSlewed;
#endif
		Gv_vars.Fdbk = vBusMeas;


		Gv_vars.Errn = Gv_vars.Ref - Gv_vars.Fdbk;

#if NON_LINEAR_VOLTAGE_LOOP
		if(fabs(Gv_vars.Errn) > 0.01)
		{
			Gv_vars.Out = Gv_vars.Out1 + Gv_coeff.Coeff_B0*3*(Gv_vars.Errn) + Gv_coeff.Coeff_B1*3*(Gv_vars.Errn1);
		}
		else
			Gv_vars.Out = Gv_vars.Out1 + Gv_coeff.Coeff_B0*(Gv_vars.Errn) + Gv_coeff.Coeff_B1*(Gv_vars.Errn1);

#else
		Gv_vars.Out = Gv_vars.Out1 + Gv_coeff.Coeff_B0*(Gv_vars.Errn) + Gv_coeff.Coeff_B1*(Gv_vars.Errn1);
#endif

		Gv_vars.Out = ( Gv_vars.Out>Gv_coeff.Max )? Gv_coeff.Max : Gv_vars.Out;
		Gv_vars.Out = ( Gv_vars.Out<Gv_coeff.Min )? Gv_coeff.Min : Gv_vars.Out;

		Gv_vars.Out1 = Gv_vars.Out;
		Gv_vars.Errn1 = Gv_vars.Errn;

		// Output of the voltage loop is thought to be the Power
		iLRef=Gv_vars.Out * (vBusMeasAvg/(3*vRmsMeasAvg));
		//iLRef=Gv_vars.Out ;

	}
#endif



#if(INCR_BUILD ==4)
	if(closeGsLoop==1)
	{
		vBusDiff= vBusPMMeas-vBusMNMeas;

#if SFRA_TYPE==SFRA_BALANCECNTL
		Gs_Out = ( vBusDiff -SFRA_F_INJECT(vBusZero))*Gs_GainKp;
#else
		Gs_Out = (vBusDiff)*Gs_GainKp;
#endif
	}
	else
	{
		Gs_Out=0;
	}
#else
	Gs_Out=0;
#endif


	#if (SFRA_TYPE == SFRA_VOLTAGE ) 	//Running FRA on Voltage
		SFRA_F_COLLECT(&Gv_vars.Out,&vBusMeas);
	#elif(SFRA_TYPE ==SFRA_CURRENT) // running FRA on Current
		SFRA_F_COLLECT(&Gi1_Out,&iL1Meas);
	#elif(SFRA_TYPE == SFRA_BALANCECNTL)
		SFRA_F_COLLECT(&Gs_Out,&vBusDiff);
	#endif

	clearInterrupt();

    resetProfilingGPIO();

}// MainISR Ends Here


//TODO control ISR Code
interrupt void tenkHzISR(void)
{
	// Let the controlISR interrupt this routine
	EINT;
	setProfilingGPIO2();

	if(closeGvLoop==1)
	{
		if ( fabs(vBusRef - vBusRefSlewed) > 0.1)
		{
			if(vBusRef>vBusRefSlewed)
				vBusRefSlewed = vBusRefSlewed + 0.0001;
			else
				vBusRefSlewed = vBusRefSlewed - 0.0001;
		}
		else if (fabs(vBusRef - vBusRefSlewed) > 0.01)
		{
			if(vBusRef>vBusRefSlewed)
				vBusRefSlewed = vBusRefSlewed + 0.00005;
			else
				vBusRefSlewed = vBusRefSlewed - 0.00005;
		}
		else if (fabs(vBusRef - vBusRefSlewed) > 0.005)
		{
			if(vBusRef>vBusRefSlewed)
				vBusRefSlewed = vBusRefSlewed + 0.00001;
			else
				vBusRefSlewed = vBusRefSlewed - 0.00001;
		}
		else
		{
			vBusRefSlewed = vBusRef;
		}
	}


	if(guiVbus > guiVbusTripLimit || guiVbusPM > 415.0  || guiVbusMN > 415.0)
	{
		EALLOW;
		EPwm1Regs.TZFRC.bit.OST = 1;
		EPwm2Regs.TZFRC.bit.OST = 1;
		EPwm3Regs.TZFRC.bit.OST = 1;
		EDIS;

		if(boardStatus==boardStatus_NoFault)
			boardStatus = 	boardStatus_OverVoltageTrip;
	}

	EMAVG_MACRO(vBusPMMeas, vBusPMMeasAvg);
	EMAVG_MACRO(vBusMNMeas, vBusMNMeasAvg);

	vBusMeasAvg= vBusPMMeasAvg+ vBusMNMeasAvg;


	guiVbusPM= vBusPMMeasAvg * V_MAX_SENSE;
	guiVbusMN= vBusMNMeasAvg * V_MAX_SENSE;
	guiVbus = guiVbusPM + guiVbusMN;

	guiV1 = v1Meas * V_MAX_SENSE;
	guiV2 = v2Meas * V_MAX_SENSE;
	guiV3 = v3Meas * V_MAX_SENSE;

	guiIL1 = iL1Meas * I_MAX_SENSE;
	guiIL2 = iL2Meas * I_MAX_SENSE;
	guiIL3 = iL3Meas * I_MAX_SENSE;

	/*guiIL1sd = iL1MeasSD * I_MAX_SENSE_SD;
	guiIL2sd = iL2MeasSD * I_MAX_SENSE_SD;
	guiIL3sd = iL3MeasSD * I_MAX_SENSE_SD;
*/
	// Sine Analyzer
	//Calculate RMS input voltage and input frequency
	sine_mains1.i = iL1Meas;
	sine_mains1.v = v1Meas;
	sineAnalyzerWithPowerMeas_calc(&sine_mains1);

	sine_mains2.i = iL2Meas;
	sine_mains2.v = v2Meas;
	sineAnalyzerWithPowerMeas_calc(&sine_mains2);

	sine_mains3.i = iL3Meas;
	sine_mains3.v = v3Meas;
	sineAnalyzerWithPowerMeas_calc(&sine_mains3);

	EMAVG_MACRO(sine_mains1.vRms, vRmsMeasAvg);

	guiIrms1=sine_mains1.iRms*I_MAX_SENSE;
	guiVrms1=sine_mains1.vRms*V_MAX_SENSE;
	guiPrms1=sine_mains1.pRms*V_MAX_SENSE*I_MAX_SENSE;
	guiPF1=sine_mains1.powerFactor;
	guiVA1=sine_mains1.vaRms*V_MAX_SENSE*I_MAX_SENSE;

	guiIrms2=sine_mains2.iRms*I_MAX_SENSE;
	guiVrms2=sine_mains2.vRms*V_MAX_SENSE;
	guiPrms2=sine_mains2.pRms*V_MAX_SENSE*I_MAX_SENSE;
	guiPF2=sine_mains2.powerFactor;
	guiVA2=sine_mains2.vaRms*V_MAX_SENSE*I_MAX_SENSE;

	guiIrms3=sine_mains3.iRms*I_MAX_SENSE;
	guiVrms3=sine_mains3.vRms*V_MAX_SENSE;
	guiPrms3=sine_mains3.pRms*V_MAX_SENSE*I_MAX_SENSE;
	guiPF3=sine_mains3.powerFactor;
	guiVA3=sine_mains3.vaRms*V_MAX_SENSE*I_MAX_SENSE;

	guiACFreq = sine_mains1.acFreqAvg;

	//TODO DLOG
	// ------------------------------------------------------------------------------
	//    Connect inputs of the Datalogger module
	// ------------------------------------------------------------------------------

	// check voltage and inductor current meas.
	dVal1 = guiV1;
	dVal2 = iL1Meas;
	dVal3 = iL2Meas;
	dVal4 = iL3Meas;

	DLOG_4CH_F_MACRO(dLog1);

	resetProfilingGPIO2();
}

//TODO setupSFRA
void setupSFRA(void)
{
	setupSCIconnectionForSFRA();

	//Specify the injection amplitude
	sfra1.amplitude=SFRA_AMPLITUDE;

	SFRA_F_INIT(&sfra1);

	//SFRA Related
	//SFRA Object Initialization

	//Specify the length of SFRA
	sfra1.Vec_Length=SFRA_FREQ_LENGTH;
	//Specify the SFRA ISR Frequency
	sfra1.ISR_Freq=SFRA_ISR_FREQ;
	//Specify the Start Frequency of the SFRA analysis
	sfra1.Freq_Start=SFRA_FREQ_START;
	//Specify the Frequency Step
	sfra1.Freq_Step=FREQ_STEP_MULTIPLY;
	//Assign array location to Pointers in the SFRA object
	sfra1.FreqVect=freqVect;
	sfra1.GH_MagVect=olMagVect;
	sfra1.GH_PhaseVect=olPhaseVect;
	sfra1.H_MagVect=plantMagVect;
	sfra1.H_PhaseVect=plantPhaseVect;

	// Re-initialize the frequency array to make SFRA sweep go fast
	i=0;

	#if SFRA_TYPE==SFRA_CURRENT// current loop
	sfra1.FreqVect[i++]=SFRA_FREQ_START;
	for(;i<sfra1.Vec_Length;i++)
	{
		sfra1.FreqVect[i]=sfra1.FreqVect[i-1]*sfra1.Freq_Step;
	}
	#else
	sfra1.FreqVect[0]=2;
	sfra1.FreqVect[1]=4;
	sfra1.FreqVect[2]=6;
	sfra1.FreqVect[3]=8;
	sfra1.FreqVect[4]=10;
	sfra1.FreqVect[5]=12;
	sfra1.FreqVect[6]=14;
	sfra1.FreqVect[7]=16;
	sfra1.FreqVect[8]=18;
	sfra1.FreqVect[9]=20;
	sfra1.FreqVect[10]=22;
	sfra1.FreqVect[11]=24;
	sfra1.FreqVect[12]=26;
	sfra1.FreqVect[13]=28;
	sfra1.FreqVect[14]=30;
	sfra1.FreqVect[15]=35;
	sfra1.FreqVect[16]=40;
	sfra1.FreqVect[17]=45;
	sfra1.FreqVect[18]=55;
	sfra1.FreqVect[19]=65;
	sfra1.FreqVect[20]=70;
	sfra1.FreqVect[21]=80;
	sfra1.FreqVect[22]=90;
	sfra1.FreqVect[23]=100;
	sfra1.FreqVect[24]=120;
	sfra1.FreqVect[25]=140;
	sfra1.FreqVect[26]=160;
	sfra1.FreqVect[27]=170;
	sfra1.FreqVect[28]=210;
	sfra1.FreqVect[29]=250;
	#endif



	//"Set" variables
	// assign GUI Buttons to desired flag addresses
	varSetBtnList[0] = (int16*)&initializationFlag;

	//"Get" variables
	//---------------------------------------
	// assign a GUI "getable" parameter address
	varGetList[0] = (int16*)&(sfra1.Vec_Length);
	varGetList[1] = (int16*)&(sfra1.status);
	varGetList[2] = (int16*)&(sfra1.FreqIndex);

	//"Setable" variables
	//----------------------------------------
	// assign GUI "setable" by Text parameter address
	dataSetList[0] = (Uint32*)&(sfra1.Freq_Start);
	dataSetList[1] = (Uint32*)&(sfra1.amplitude);
	dataSetList[2] = (Uint32*)&(sfra1.Freq_Step);

	// assign a GUI "getable" parameter array address
	arrayGetList[0] = (int32*)freqVect;
	arrayGetList[1] = (int32*)olMagVect;
	arrayGetList[2] = (int32*)olPhaseVect;
	arrayGetList[3] = (int32*)plantMagVect;
	arrayGetList[4] = (int32*)plantPhaseVect;
	arrayGetList[5] = (int32*)&(sfra1.Freq_Start);
	arrayGetList[6] = (int32*)&(sfra1.amplitude);
	arrayGetList[7] = (int32*)&(sfra1.Freq_Step);

	CommsOKflg = 0;
	SerialCommsTimer = 0;
}

//===========================================================================
// No more.
//===========================================================================



//TODO initGlobalVariables()
void initGlobalVariables(void)
{
	// Gui Variables
	guiVbus=0;
	guiACFreq=0;
	guiPrms1=0;
	guiIrms1=0;
	guiVrms1=0;

	guiPrms2=0;
	guiIrms2=0;
	guiVrms2=0;

	guiPrms3=0;
	guiIrms3=0;
	guiVrms3=0;


	vBusPMMeasAvg=0;
	vBusMNMeasAvg=0;

	dLog1.input_ptr1 = &dVal1;
	dLog1.input_ptr2 = &dVal2;
	dLog1.input_ptr3 = &dVal3;  // spll.Out logged in buff3
	dLog1.input_ptr4 = &dVal4;  // ramp_sin logged in buff4
	dLog1.output_ptr1 = dBuff1;
	dLog1.output_ptr2 = dBuff2;
	dLog1.output_ptr3 = dBuff3;
	dLog1.output_ptr4 = dBuff4;
	dLog1.prev_value = (float32)(0);
	dLog1.trig_value = (float32)(0.05);
	dLog1.status = 1;
	dLog1.pre_scalar = 5;
	dLog1.skip_count = 0;
	dLog1.size = 100;
	dLog1.count = 0;

	//Variable initialization

	offsetCalCounter=0;

	closeGiLoop=0;
	closeGsLoop=0;
	closeGvLoop=0;
	clearTrip=0;

	duty1PU=0.0;
	duty2PU=0.0;
	duty3PU=0.0;

	iL1MeasOffset=0.0;
	iL2MeasOffset=0.0;
	iL3MeasOffset=0.0;

	m_VBusPMMeas=0.985769085;
	b_VBusPMMeas=0.003082908;

	m_VBusMNMeas=0.991549;
	b_VBusMNMeas=-0.00324;

	vRmsMeasAvg=0.0;

	guiPowerStageStart=0;
	guiPowerStageStop=0;

	dutyPU_DC=0.5;

	firstTimeGvLoop=1;
	closeGvLoop=0;
	vBusRef=((float)VBUS_REF_SET / (float)V_MAX_SENSE );
	iLRef=0.05;


}


void SPLL_LPF_Coeff_Update(SPLL_1ph_SOGI_F *spll, float Kp, float Ki, float Ts)
{
	spll->lpf_coeff.B0_lf= ((2.0)*Kp+(float)Ki*(float)Ts)*0.5;
	spll->lpf_coeff.B1_lf= -((2.0)*Kp-(float)Ki*(float)Ts)*0.5;
	spll->lpf_coeff.A1_lf = 1;
}


//TODO updateBoardStatus()
void updateBoardStatus(void)
{
	if(EPwm1Regs.TZFLG.all==0 && EPwm2Regs.TZFLG.all==0)
		boardStatus=boardStatus_NoFault;
	else
	{
		if(EPwm1Regs.TZFLG.bit.DCAEVT1==1 || EPwm1Regs.TZFLG.bit.DCAEVT1==1)
		{
			boardStatus=boardStatus_OverCurrentTrip;

		}
		else if(EPwm1Regs.TZFLG.bit.CBC==1 || EPwm2Regs.TZFLG.bit.CBC==1)
		{
			boardStatus=boardStatus_EmulatorStopTrip;

			openRelay();
		}
		else if (EPwm1Regs.TZFLG.bit.OST==1 && EPwm2Regs.TZFLG.bit.OST==1 )
			boardStatus=boardStatus_Idle;
	}
}
