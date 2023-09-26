/* ==============================================================================
System Name:

File Name:	  	{ProjectName}-Settings.h

Target:

Author: Manish Bhardwaj, C2000 Systems Solutions Group

Description:

Copyright (C) {2015} Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED*
=================================================================================  */
#ifndef _PROJSETTINGS_H
#define _PROJSETTINGS_H

#ifdef __cplusplus

extern "C" {
#endif

//*****************************************************************************
//defines
//*****************************************************************************

/* Device Related Defines */
#define CPU_SYS_CLOCK (200*1000000)
#define PWMSYSCLOCK_FREQ (100*1000000)
#define ECAPSYSCLOCK_FREQ	(200*1000000)

/* Project Options*/
//==================================================================================
// Incremental Build options for System check-out
//==================================================================================
// BUILD 1 	 Open Loop Check
// BUILD 2   Closed Current Loop Check
// BUILD 3   Voltage loop with inner current loop check
// BUILD 4   Balance loop with voltage and current loop closed
#define INCR_BUILD 2

// Check system under DC condition (cleanest SFRA curves) 0 is FALSE, 1 is TRUE
#define DC_CHECK 0 
// Select sensing option, 1 for ADC, 2 for SD
#define ADC_BASED_SENSING 1
#define SDFM_BASED_SENSING 2
#define SENSING_OPTION		1 
#define AC_FREQ        		60

#if INCR_BUILD==4
#define INDUCTOR_VOLTAGE_DROP_FEEDFORWARD 1
#define THIRD_HARMONIC_INJECTION 1
#else
#define INDUCTOR_VOLTAGE_DROP_FEEDFORWARD 0
#define THIRD_HARMONIC_INJECTION 0
#endif

#define NON_LINEAR_VOLTAGE_LOOP 0

/* Power Stage Related Values*/
#define PFC3PH_PWM_SWITCHING_FREQUENCY ((float)50*1000)
#define PFC3PH_PWM_PERIOD (PWMSYSCLOCK_FREQ)/(PFC3PH_PWM_SWITCHING_FREQUENCY)

#define VAC_MAX_SENSE 454
#define VDCBUS_MAX_SENSE 717.79
#define V_MAX_SENSE (454)

#define I_MAX_SENSE 12
#define I_TRIP_LIMIT 11
#define I_MAX_SENSE_SD 15.625
#define VAC_TYPICAL 208

/* Control Loop Design */
#define	CNTRL_ISR_FREQ_RATIO	1
#define VOLTAGE_LOOP_RUN_RATIO	1

#define ISR_CONTROL_FREQUENCY (PFC3PH_PWM_SWITCHING_FREQUENCY)/(CNTRL_ISR_FREQ_RATIO)
#define ISR_10KHZ_FREQUENCY (10000)/(CNTRL_ISR_FREQ_RATIO)

#define PFC_INDUCTOR_VALUE 0.003 // 3mH

//SFRA Options 	
//1 FRA for the Current Loop
//2 FRA for the Voltage Loop
//3 FRA for the Voltage Balance Loop
#define SFRA_CURRENT 1
#define SFRA_VOLTAGE 2
#define SFRA_BALANCECNTL 3
#if INCR_BUILD ==4
#define SFRA_TYPE			SFRA_BALANCECNTL
#elif INCR_BUILD==3
#define SFRA_TYPE			SFRA_VOLTAGE
#else
#define SFRA_TYPE			SFRA_CURRENT
#endif
#define SFRA_ISR_FREQ 		ISR_CONTROL_FREQUENCY

#define PI 3.141592653589

#define GI_GAINKP 2.0000000000

#define CNTL_GV_A1 1.0000000000
#define CNTL_GV_A2 0.0000000000
#define CNTL_GV_B0 0.4000400000
#define CNTL_GV_B1 -0.3999600000
#define CNTL_GV_B2 0.0000000000
#define CNTL_GV_IMIN -0.25
#define CNTL_GV_MAX 0.9
#define CNTL_GV_MIN -0.25

#define GS_GAINKP 1.0000000000

/* SDFM Sensing Parameters*/

#define SDCLK_FREQ		(float)(20*1000000)
#define SD_CLK_COUNT ECAPSYSCLOCK_FREQ/SDCLK_FREQ
#define PWM_CLK_IN_SDFM_OSR (int)(((float)PWMSYSCLOCK_FREQ/(float)SDCLK_FREQ)*(float)SDFM_OSR)
#define SDFM_FILTER_TYPE	3
#define SDFM_OSR			100


/* PWM pin, ADC, SDFM, Relay Selection related variables */
#define PWM_NO       		1
#define ADC_TRIG_SOURCE   	5

#define ADC_TRIG_SOURCE   	5

#define IL1_ADC_MODULE	1
#define IL1_ADC_PIN	    2
#define IL1_CMPSS_NO	1
#define IL1_ADC_TRIG_SOURCE ADC_TRIG_SOURCE
#define IL1_ACQPS_SYS_CLKS 50
#define IL1_ADC_SOC1 0x10
#define IL1_ADC_READ1 AdcaResultRegs.ADCRESULT0
#define IL1_ADC_SOC2 0x16
#define IL1_ADC_READ2 AdcaResultRegs.ADCRESULT3
#define IL1_ADC_SOC3 0x1C
#define IL1_ADC_READ3 AdcaResultRegs.ADCRESULT6
#define IL1_ADC_SOC4 0x22
#define IL1_ADC_READ4 AdcaResultRegs.ADCRESULT9

#define IL2_ADC_MODULE	1
#define IL2_ADC_PIN	    4
#define IL2_CMPSS_NO	2
#define IL2_ADC_TRIG_SOURCE ADC_TRIG_SOURCE
#define IL2_ACQPS_SYS_CLKS 50
#define IL2_ADC_SOC1 0x12
#define IL2_ADC_READ1 AdcaResultRegs.ADCRESULT1
#define IL2_ADC_SOC2 0x18
#define IL2_ADC_READ2 AdcaResultRegs.ADCRESULT4
#define IL2_ADC_SOC3 0x1E
#define IL2_ADC_READ3 AdcaResultRegs.ADCRESULT7
#define IL2_ADC_SOC4 0x24
#define IL2_ADC_READ4 AdcaResultRegs.ADCRESULT10

#define IL3_ADC_MODULE	1
#define IL3_ADC_PIN	    14
#define IL3_CMPSS_NO	4
#define IL3_ADC_TRIG_SOURCE ADC_TRIG_SOURCE
#define IL3_ACQPS_SYS_CLKS 50
#define IL3_ADC_SOC1 0x14
#define IL3_ADC_READ1 AdcaResultRegs.ADCRESULT2
#define IL3_ADC_SOC2 0x1A
#define IL3_ADC_READ2 AdcaResultRegs.ADCRESULT5
#define IL3_ADC_SOC3 0x20
#define IL3_ADC_READ3 AdcaResultRegs.ADCRESULT8
#define IL3_ADC_SOC4 0x26
#define IL3_ADC_READ4 AdcaResultRegs.ADCRESULT11


#define V1_ADC_MODULE	2
#define V1_ADC_PIN	    2
#define V1_ADC_TRIG_SOURCE ADC_TRIG_SOURCE
#define V1_ACQPS_SYS_CLKS 50
#define V1_ADC_SOC1 0x10
#define V1_ADC_READ1 AdcbResultRegs.ADCRESULT0
#define V1_ADC_SOC2 0x12
#define V1_ADC_READ2 AdcbResultRegs.ADCRESULT1
#define V1_ADC_SOC3 0x14
#define V1_ADC_READ3 AdcbResultRegs.ADCRESULT2
#define V1_ADC_SOC4 0x16
#define V1_ADC_READ4 AdcbResultRegs.ADCRESULT3

#define V2_ADC_MODULE	4
#define V2_ADC_PIN	    0
#define V2_ADC_TRIG_SOURCE ADC_TRIG_SOURCE
#define V2_ACQPS_SYS_CLKS 50
#define V2_ADC_SOC1 0x10
#define V2_ADC_READ1 AdcdResultRegs.ADCRESULT0
#define V2_ADC_SOC2 0x14
#define V2_ADC_READ2 AdcdResultRegs.ADCRESULT2
#define V2_ADC_SOC3 0x18
#define V2_ADC_READ3 AdcdResultRegs.ADCRESULT4
#define V2_ADC_SOC4 0x1C
#define V2_ADC_READ4 AdcdResultRegs.ADCRESULT6


#define V3_ADC_MODULE	4
#define V3_ADC_PIN	    2
#define V3_ADC_TRIG_SOURCE ADC_TRIG_SOURCE
#define V3_ACQPS_SYS_CLKS 50
#define V3_ADC_SOC1 0x12
#define V3_ADC_READ1 AdcdResultRegs.ADCRESULT1
#define V3_ADC_SOC2 0x16
#define V3_ADC_READ2 AdcdResultRegs.ADCRESULT3
#define V3_ADC_SOC3 0x1A
#define V3_ADC_READ3 AdcdResultRegs.ADCRESULT5
#define V3_ADC_SOC4 0x1E
#define V3_ADC_READ4 AdcdResultRegs.ADCRESULT7

#define VBUSPM_ADC_MODULE	3
#define VBUSPM_ADC_PIN	    2
#define VBUSPM_ADC_TRIG_SOURCE ADC_TRIG_SOURCE
#define VBUSPM_ACQPS_SYS_CLKS 50
#define VBUSPM_ADC_SOC1 0x10
#define VBUSPM_ADC_READ1 AdccResultRegs.ADCRESULT0
#define VBUSPM_ADC_SOC2 0x14
#define VBUSPM_ADC_READ2 AdccResultRegs.ADCRESULT2
#define VBUSPM_ADC_SOC3 0x18
#define VBUSPM_ADC_READ3 AdccResultRegs.ADCRESULT4
#define VBUSPM_ADC_SOC4 0x1C
#define VBUSPM_ADC_READ4 AdccResultRegs.ADCRESULT6

#define VBUSMN_ADC_MODULE	3
#define VBUSMN_ADC_PIN	    4
#define VBUSMN_ADC_TRIG_SOURCE ADC_TRIG_SOURCE
#define VBUSMN_ACQPS_SYS_CLKS 50
#define VBUSMN_ADC_SOC1 0x12
#define VBUSMN_ADC_READ1 AdccResultRegs.ADCRESULT1
#define VBUSMN_ADC_SOC2 0x16
#define VBUSMN_ADC_READ2 AdccResultRegs.ADCRESULT3
#define VBUSMN_ADC_SOC3 0x1A
#define VBUSMN_ADC_READ3 AdccResultRegs.ADCRESULT5
#define VBUSMN_ADC_SOC4 0x1E
#define VBUSMN_ADC_READ4 AdccResultRegs.ADCRESULT7

#define VBUS_REF_SET 620

#ifdef __cplusplus
}
#endif                                  /* extern "C" */

#endif //_PROJSETTINGS_H
