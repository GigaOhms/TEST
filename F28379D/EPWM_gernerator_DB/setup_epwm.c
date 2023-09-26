/*
 * epwm.c
 *
 *  Created on: 2019. 10. 28.
 *      Author: LTH
 */

#include "F28x_Project.h"
#include "setup_epwm.h"


//*************************************8
void setup_epwm(void)
{
    gpio_select();
    setup_epwm1();
    setup_epwm2();
//    setup_epwm3();
   // setup_epwm4();
   // setup_epwm5();
   // setup_epwm6();
   // setup_epwm7();
   // setup_epwm8();
   // setup_epwm9();
   // setup_epwm10();
   // setup_epwm11();
  //  setup_epwm12();
    setup_ADC();
//    setup_DAC();
}
//*********************C*****************
//*********************C*****************

void gpio_select(void)
{
    EALLOW;
    ///////////////*********************************/////////////////////////
    ///////////////** For configuration for EPWM ***////////////////////////

    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // ePWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // ePWM1B
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up on GPIO1 (EPWM1B)

    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // ePWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // ePWM2B
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO3 (EPWM2B)

    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // ePWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // ePWM3B
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO5 (EPWM3B)

    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // ePWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // ePWM4B
    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Disable pull-up on GPIO7 (EPWM4B)

    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // ePWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // ePWM5B
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;    // Disable pull-up on GPIO8 (EPWM5A)
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;    // Disable pull-up on GPIO9 (EPWM5B)

    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;  // ePWM6A
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // ePWM6B
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;    // Disable pull-up on GPIO10 (EPWM6A)
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;    // Disable pull-up on GPIO11 (EPWM6B)

    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1; // ePWM7A
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1; // ePWM7B
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;    // Disable pull-up on GPIO12 (EPWM7A)
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 1;    // Disable pull-up on GPIO13 (EPWM7B)

    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1; // ePWM8A
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1; // ePWM8B
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1;    // Disable pull-up on GPIO14 (EPWM8A)
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;    // Disable pull-up on GPIO15 (EPWM8B)

    GpioCtrlRegs.GPFMUX1.bit.GPIO161 = 1; // ePWM9A
    GpioCtrlRegs.GPFMUX1.bit.GPIO162 = 1; // ePWM9B
    GpioCtrlRegs.GPFPUD.bit.GPIO161 = 1;    // Disable pull-up on GPIO161 (EPWM9A)
    GpioCtrlRegs.GPFPUD.bit.GPIO162 = 1;    // Disable pull-up on GPIO162 (EPWM9B)

    GpioCtrlRegs.GPFMUX1.bit.GPIO163 = 1; // ePWM10A
    GpioCtrlRegs.GPFMUX1.bit.GPIO164 = 1; // ePWM10B
    GpioCtrlRegs.GPFPUD.bit.GPIO163 = 1;    // Disable pull-up on GPIO163 (EPWM10A)
    GpioCtrlRegs.GPFPUD.bit.GPIO164 = 1;    // Disable pull-up on GPIO164 (EPWM10B)

    GpioCtrlRegs.GPFMUX1.bit.GPIO165 = 1; // ePWM11A
    GpioCtrlRegs.GPFMUX1.bit.GPIO166 = 1; // ePWM11B
    GpioCtrlRegs.GPFPUD.bit.GPIO165 = 1;    // Disable pull-up on GPIO165 (EPWM11A)
    GpioCtrlRegs.GPFPUD.bit.GPIO166 = 1;    // Disable pull-up on GPIO166 (EPWM11B)

    GpioCtrlRegs.GPFMUX1.bit.GPIO167 = 1; // ePWM12A
    GpioCtrlRegs.GPFMUX1.bit.GPIO168 = 1; // ePWM12B
    GpioCtrlRegs.GPFPUD.bit.GPIO167 = 1;    // Disable pull-up on GPIO167 (EPWM12A)
    GpioCtrlRegs.GPFPUD.bit.GPIO168 = 1;    // Disable pull-up on GPIO168 (EPWM12B)


    EDIS;
    ///////////////*********************************/////////////////////////

}
void setup_DAC(void)
{
//#define REFERENCE_VDAC      0
//#define REFERENCE_VREF      1
    /////////setup channel A
    EALLOW;
    DacaRegs.DACCTL.bit.DACREFSEL = 1;//REFERENCE_VREF
    DacaRegs.DACOUTEN.bit.DACOUTEN = 1;
    DacaRegs.DACVALS.all = 0;
    /////////setup channel B
    DacbRegs.DACCTL.bit.DACREFSEL = 1;//REFERENCE
    DacbRegs.DACOUTEN.bit.DACOUTEN = 1;
    DacbRegs.DACVALS.all = 0;
    /////////setup channel C
    DaccRegs.DACCTL.bit.DACREFSEL = 1;//REFERENCE
    DaccRegs.DACOUTEN.bit.DACOUTEN = 1;
    DaccRegs.DACVALS.all = 0;
    DELAY_US(10); // Delay for buffered DAC to power up
    EDIS;

////////////////////////// example output DAC////
    DacaRegs.DACVALS.all=4095*3.3/3.3;
    DacbRegs.DACVALS.all=4095;
   // DacbRegs.DACVALS.all = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER1);

}
void setup_ADC(void)
{
    EALLOW;
       //write configurations
       AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /1
       AdcbRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /1
       AdccRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /1
       AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
       AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
       AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
       AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;//power up A the ADC
       AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;//power up B the ADC
       AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;//power up C the ADC

       //************** ADCA_BASE ************************************************

       AdcaRegs.ADCSOC0CTL.bit.CHSEL = 14;  //SOC0 will convert pin A0
       AdcaRegs.ADCSOC0CTL.bit.ACQPS = 14; //sample window is 100 SYSCLK cycles
       AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C

       AdcaRegs.ADCSOC1CTL.bit.CHSEL = 3;  //SOC0 will convert pin A0
       AdcaRegs.ADCSOC1CTL.bit.ACQPS = 14; //sample window is 100 SYSCLK cycles
       AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C


       //************** ADCB_BASE ************************************************

          AdcbRegs.ADCSOC0CTL.bit.CHSEL = 3;  //SOC0 will convert pin A0
          AdcbRegs.ADCSOC0CTL.bit.ACQPS = 14; //sample window is 100 SYSCLK cycles
          AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C


       //************** ADCC_BASE ************************************************
          AdccRegs.ADCSOC0CTL.bit.CHSEL = 2;  //SOC0 will convert pin A0
          AdccRegs.ADCSOC0CTL.bit.ACQPS = 14; //sample window is 100 SYSCLK cycles
          AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C

          AdccRegs.ADCSOC1CTL.bit.CHSEL = 3;  //SOC0 will convert pin A0
          AdccRegs.ADCSOC1CTL.bit.ACQPS = 14; //sample window is 100 SYSCLK cycles
          AdccRegs.ADCSOC1CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C


       //************** END ************************************************
          EDIS;
}

//*********************C*****************
void setup_epwm1(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM1=1;
    // Setup TBCLK
    EPwm1Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm1Regs.TBCTR = 0x0000;                  // Clear counter
    // Set Compare values
    EPwm1Regs.CMPA.bit.CMPA = 0;    // Set compare A value
    EPwm1Regs.CMPB.bit.CMPB = 0;    // Set Compare B value
    // Setup counter mode
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;   // Sync down-stream module
    EPwm1Regs.TBCTL.bit.PHSDIR=TB_UP;           // MODE up AFTER SYNC

    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    // Setup shadowing
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // Set actions
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on event A, up count
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, down count
    EPwm1Regs.AQCTLB.bit.CBU = AQ_SET;            // Set PWM1B on event B, up count
    EPwm1Regs.AQCTLB.bit.CBD = AQ_CLEAR;          // Clear PWM1B on event B,down count

    // Active Low PWMs - Setup Deadband
    // note: when using Deadband module, you can not set actions for EPwmxB.
    //at time, actions for EPwmxB will be setup inversion of EPwmxA as default
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary --> deadtime
    EPwm1Regs.DBFED.all = 50; // FED = 50 TBCLKs
    EPwm1Regs.DBRED.all = 50; // RED = 50 TBCLKs




    // Trigger ADC SOC
     EPwm1Regs.ETSEL.bit.SOCAEN=1;
     EPwm1Regs.ETSEL.bit.SOCASEL=3;
     EPwm1Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event



   // Interrupt where we will change the Compare Values
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}

void setup_epwm2(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM2=1;
    // Setup TBCLK
    EPwm2Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm2Regs.TBCTR = 0x0000;                  // Clear counter
    // Set Compare values
    EPwm2Regs.CMPA.bit.CMPA = 0;    // Set compare A value
    EPwm2Regs.CMPB.bit.CMPB = 0;    // Set Compare B value
    // Setup counter mode
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;   // Sync down-stream module
    EPwm2Regs.TBCTL.bit.PHSDIR=TB_UP;           // MODE up AFTER SYNC
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    // Setup shadowing
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    // Set actions
    EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on event A, up count
    EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, down count
    EPwm2Regs.AQCTLB.bit.CBU = AQ_SET;            // Set PWM1B on event B, up count
    EPwm2Regs.AQCTLB.bit.CBD = AQ_CLEAR;          // Clear PWM1B on event B,down count

    // Active Low PWMs - Setup Deadband
    // note: when using Deadband module, you can not set actions for EPwmxB.
    //at time, actions for EPwmxB will be setup inversion of EPwmxA as default
    EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary --> deadtime
    EPwm2Regs.DBFED.all = 50; // FED = 50 TBCLKs
    EPwm2Regs.DBRED.all = 50; // RED = 50 TBCLKs


    // Trigger ADC SOC
     EPwm2Regs.ETSEL.bit.SOCAEN=1;
     EPwm2Regs.ETSEL.bit.SOCASEL=3;
     EPwm2Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event



   // Interrupt where we will change the Compare Values
    EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm2Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm2Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}
void setup_epwm3(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM3=1;
    // Setup TBCLK
    EPwm3Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm3Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm3Regs.TBCTR = 0x0000;                  // Clear counter
    // Set Compare values
    EPwm3Regs.CMPA.bit.CMPA = 10;    // Set compare A value
    EPwm3Regs.CMPB.bit.CMPB = 10;    // Set Compare B value
    // Setup counter mode
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;   // Sync down-stream module
    EPwm3Regs.TBCTL.bit.PHSDIR=TB_UP;           // MODE up AFTER SYNC
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    // Setup shadowing
    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    // Set actions
    EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on event A, up count
    EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, down count
    EPwm3Regs.AQCTLB.bit.CBU = AQ_SET;            // Set PWM1B on event B, up count
    EPwm3Regs.AQCTLB.bit.CBD = AQ_CLEAR;          // Clear PWM1B on event B,down count

    // Active Low PWMs - Setup Deadband
    // note: when using Deadband module, you can not set actions for EPwmxB.
    //at time, actions for EPwmxB will be setup inversion of EPwmxA as default
    EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary --> deadtime
    EPwm3Regs.DBFED.all = 50; // FED = 50 TBCLKs
    EPwm3Regs.DBRED.all = 50; // RED = 50 TBCLKs

    // Trigger ADC SOC
     EPwm3Regs.ETSEL.bit.SOCAEN=1;
     EPwm3Regs.ETSEL.bit.SOCASEL=3;
     EPwm3Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event

   // Interrupt where we will change the Compare Values
    EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm3Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm3Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}
void setup_epwm4(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM4=1;
    // Setup TBCLK
    EPwm4Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm4Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm4Regs.TBCTR = 0x0000;                  // Clear counter
    // Set Compare values
    EPwm4Regs.CMPA.bit.CMPA = 10;    // Set compare A value
    EPwm4Regs.CMPB.bit.CMPB = 10;    // Set Compare B value
    // Setup counter mode
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;   // Sync down-stream module
    EPwm4Regs.TBCTL.bit.PHSDIR=TB_UP;           // MODE up AFTER SYNC
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    // Setup shadowing
    EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    // Set actions
    EPwm4Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on event A, up count
    EPwm4Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, down count
    EPwm4Regs.AQCTLB.bit.CBU = AQ_SET;            // Set PWM1B on event B, up count
    EPwm4Regs.AQCTLB.bit.CBD = AQ_CLEAR;          // Clear PWM1B on event B,down count

    // Active Low PWMs - Setup Deadband
    // note: when using Deadband module, you can not set actions for EPwmxB.
    //at time, actions for EPwmxB will be setup inversion of EPwmxA as default
    EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary --> deadtime
    EPwm4Regs.DBFED.all = 50; // FED = 50 TBCLKs
    EPwm4Regs.DBRED.all = 50; // RED = 50 TBCLKs


    // Trigger ADC SOC
     EPwm4Regs.ETSEL.bit.SOCAEN=1;
     EPwm4Regs.ETSEL.bit.SOCASEL=3;
     EPwm4Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event

   // Interrupt where we will change the Compare Values
    EPwm4Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm4Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm4Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}
void setup_epwm5(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM5=1;
    // Setup TBCLK
    EPwm5Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm5Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm5Regs.TBCTR = 0x0000;                  // Clear counter
    // Set Compare values
    EPwm5Regs.CMPA.bit.CMPA = 10;    // Set compare A value
    EPwm5Regs.CMPB.bit.CMPB = 10;    // Set Compare B value
    // Setup counter mode
    EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm5Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;   // Sync down-stream module
    EPwm5Regs.TBCTL.bit.PHSDIR=TB_UP;           // MODE up AFTER SYNC
    EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    // Setup shadowing
    EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    // Set actions
    EPwm5Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on event A, up count
    EPwm5Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, down count
    EPwm5Regs.AQCTLB.bit.CBU = AQ_SET;            // Set PWM1B on event B, up count
    EPwm5Regs.AQCTLB.bit.CBD = AQ_CLEAR;          // Clear PWM1B on event B,down count

    // Active Low PWMs - Setup Deadband
    // note: when using Deadband module, you can not set actions for EPwmxB.
    //at time, actions for EPwmxB will be setup inversion of EPwmxA as default
    EPwm5Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary --> deadtime
    EPwm5Regs.DBFED.all = 50; // FED = 50 TBCLKs
    EPwm5Regs.DBRED.all = 50; // RED = 50 TBCLKs

    // Trigger ADC SOC
     EPwm5Regs.ETSEL.bit.SOCAEN=1;
     EPwm5Regs.ETSEL.bit.SOCASEL=3;
     EPwm5Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event


   // Interrupt where we will change the Compare Values
    EPwm5Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm5Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm5Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}
void setup_epwm6(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM6=1;
    // Setup TBCLK
    EPwm6Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm6Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm6Regs.TBCTR = 0x0000;                  // Clear counter
    // Set Compare values
    EPwm6Regs.CMPA.bit.CMPA = 10;    // Set compare A value
    EPwm6Regs.CMPB.bit.CMPB = 10;    // Set Compare B value
    // Setup counter mode
    EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm6Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;   // Sync down-stream module
    EPwm6Regs.TBCTL.bit.PHSDIR=TB_UP;           // MODE up AFTER SYNC
    EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    // Setup shadowing
    EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    // Set actions
    EPwm6Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on event A, up count
    EPwm6Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, down count
    EPwm6Regs.AQCTLB.bit.CBU = AQ_SET;            // Set PWM1B on event B, up count
    EPwm6Regs.AQCTLB.bit.CBD = AQ_CLEAR;          // Clear PWM1B on event B,down count

    // Active Low PWMs - Setup Deadband
    // note: when using Deadband module, you can not set actions for EPwmxB.
    //at time, actions for EPwmxB will be setup inversion of EPwmxA as default
    EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary --> deadtime
    EPwm6Regs.DBFED.all = 50; // FED = 50 TBCLKs
    EPwm6Regs.DBRED.all = 50; // RED = 50 TBCLKs


    // Trigger ADC SOC
     EPwm6Regs.ETSEL.bit.SOCAEN=1;
     EPwm6Regs.ETSEL.bit.SOCASEL=3;
     EPwm6Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event

   // Interrupt where we will change the Compare Values
    EPwm6Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm6Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm6Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}

//*********************C*****************
void setup_epwm7(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM7=1;
    // Setup TBCLK
    EPwm7Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm7Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm7Regs.TBCTR = 0x0000;                  // Clear counter
    // Set Compare values
    EPwm7Regs.CMPA.bit.CMPA = 10;    // Set compare A value
    EPwm7Regs.CMPB.bit.CMPB = 10;    // Set Compare B value
    // Setup counter mode
    EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm7Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm7Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;   // Sync down-stream module
    EPwm7Regs.TBCTL.bit.PHSDIR=TB_UP;           // MODE up AFTER SYNC
    EPwm7Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm7Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    // Setup shadowing
    EPwm7Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm7Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm7Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    // Set actions
    EPwm7Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on event A, up count
    EPwm7Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, down count
    EPwm7Regs.AQCTLB.bit.CBU = AQ_SET;            // Set PWM1B on event B, up count
    EPwm7Regs.AQCTLB.bit.CBD = AQ_CLEAR;          // Clear PWM1B on event B,down count

    // Active Low PWMs - Setup Deadband
    // note: when using Deadband module, you can not set actions for EPwmxB.
    //at time, actions for EPwmxB will be setup inversion of EPwmxA as default
    EPwm7Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    EPwm7Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary --> deadtime
    EPwm7Regs.DBFED.all = 50; // FED = 50 TBCLKs
    EPwm7Regs.DBRED.all = 50; // RED = 50 TBCLKs


    // Trigger ADC SOC
     EPwm7Regs.ETSEL.bit.SOCAEN=1;
     EPwm7Regs.ETSEL.bit.SOCASEL=3;
     EPwm7Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event


   // Interrupt where we will change the Compare Values
    EPwm7Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm7Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm7Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}

void setup_epwm8(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM8=1;
    // Setup TBCLK
    EPwm8Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm8Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm8Regs.TBCTR = 0x0000;                  // Clear counter
    // Set Compare values
    EPwm8Regs.CMPA.bit.CMPA = 10;    // Set compare A value
    EPwm8Regs.CMPB.bit.CMPB = 10;    // Set Compare B value
    // Setup counter mode
    EPwm8Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm8Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm8Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;   // Sync down-stream module
    EPwm8Regs.TBCTL.bit.PHSDIR=TB_UP;           // MODE up AFTER SYNC
    EPwm8Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm8Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    // Setup shadowing
    EPwm8Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm8Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm8Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm8Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    // Set actions
    EPwm8Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on event A, up count
    EPwm8Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, down count
    EPwm8Regs.AQCTLB.bit.CBU = AQ_SET;            // Set PWM1B on event B, up count
    EPwm8Regs.AQCTLB.bit.CBD = AQ_CLEAR;          // Clear PWM1B on event B,down count

    // Active Low PWMs - Setup Deadband
    // note: when using Deadband module, you can not set actions for EPwmxB.
    //at time, actions for EPwmxB will be setup inversion of EPwmxA as default
    EPwm8Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    EPwm8Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary --> deadtime
    EPwm8Regs.DBFED.all = 50; // FED = 50 TBCLKs
    EPwm8Regs.DBRED.all = 50; // RED = 50 TBCLKs

    // Trigger ADC SOC
     EPwm8Regs.ETSEL.bit.SOCAEN=1;
     EPwm8Regs.ETSEL.bit.SOCASEL=3;
     EPwm8Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event

   // Interrupt where we will change the Compare Values
    EPwm8Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm8Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm8Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}
void setup_epwm9(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM9=1;
    // Setup TBCLK
    EPwm9Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm9Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm9Regs.TBCTR = 0x0000;                  // Clear counter
    // Set Compare values
    EPwm9Regs.CMPA.bit.CMPA = 10;    // Set compare A value
    EPwm9Regs.CMPB.bit.CMPB = 10;    // Set Compare B value
    // Setup counter mode
    EPwm9Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm9Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm9Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;   // Sync down-stream module
    EPwm9Regs.TBCTL.bit.PHSDIR=TB_UP;           // MODE up AFTER SYNC
    EPwm9Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm9Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    // Setup shadowing
    EPwm9Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm9Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm9Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm9Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    // Set actions
    EPwm9Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on event A, up count
    EPwm9Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, down count
    EPwm9Regs.AQCTLB.bit.CBU = AQ_SET;            // Set PWM1B on event B, up count
    EPwm9Regs.AQCTLB.bit.CBD = AQ_CLEAR;          // Clear PWM1B on event B,down count

    // Active Low PWMs - Setup Deadband
    // note: when using Deadband module, you can not set actions for EPwmxB.
    //at time, actions for EPwmxB will be setup inversion of EPwmxA as default
    EPwm9Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    EPwm9Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary --> deadtime
    EPwm9Regs.DBFED.all = 50; // FED = 50 TBCLKs
    EPwm9Regs.DBRED.all = 50; // RED = 50 TBCLKs


    // Trigger ADC SOC
     EPwm9Regs.ETSEL.bit.SOCAEN=1;
     EPwm9Regs.ETSEL.bit.SOCASEL=3;
     EPwm9Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event
   // Interrupt where we will change the Compare Values
    EPwm9Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm9Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm9Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}
void setup_epwm10(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM10=1;
    // Setup TBCLK
    EPwm10Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm10Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm10Regs.TBCTR = 0x0000;                  // Clear counter
    // Set Compare values
    EPwm10Regs.CMPA.bit.CMPA = 10;    // Set compare A value
    EPwm10Regs.CMPB.bit.CMPB = 10;    // Set Compare B value
    // Setup counter mode
    EPwm10Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm10Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm10Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;   // Sync down-stream module
    EPwm10Regs.TBCTL.bit.PHSDIR=TB_UP;           // MODE up AFTER SYNC
    EPwm10Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm10Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    // Setup shadowing
    EPwm10Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm10Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm10Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm10Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    // Set actions
    EPwm10Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on event A, up count
    EPwm10Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, down count
    EPwm10Regs.AQCTLB.bit.CBU = AQ_SET;            // Set PWM1B on event B, up count
    EPwm10Regs.AQCTLB.bit.CBD = AQ_CLEAR;          // Clear PWM1B on event B,down count
    // Active Low PWMs - Setup Deadband
    // note: when using Deadband module, you can not set actions for EPwmxB.
    //at time, actions for EPwmxB will be setup inversion of EPwmxA as default
    EPwm10Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    EPwm10Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary --> deadtime
    EPwm10Regs.DBFED.all = 50; // FED = 50 TBCLKs
    EPwm10Regs.DBRED.all = 50; // RED = 50 TBCLKs

    // Trigger ADC SOC
     EPwm10Regs.ETSEL.bit.SOCAEN=1;
     EPwm10Regs.ETSEL.bit.SOCASEL=3;
     EPwm10Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event
   // Interrupt where we will change the Compare Values
    EPwm10Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm10Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm10Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}
void setup_epwm11(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM11=1;
    // Setup TBCLK
    EPwm11Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm11Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm11Regs.TBCTR = 0x0000;                  // Clear counter
    // Set Compare values
    EPwm11Regs.CMPA.bit.CMPA = 10;    // Set compare A value
    EPwm11Regs.CMPB.bit.CMPB = 10;    // Set Compare B value
    // Setup counter mode
    EPwm11Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm11Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm11Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;   // Sync down-stream module
    EPwm11Regs.TBCTL.bit.PHSDIR=TB_UP;           // MODE up AFTER SYNC
    EPwm11Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm11Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    // Setup shadowing
    EPwm11Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm11Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm11Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm11Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    // Set actions
    EPwm11Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on event A, up count
    EPwm11Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, down count
    EPwm11Regs.AQCTLB.bit.CBU = AQ_SET;            // Set PWM1B on event B, up count
    EPwm11Regs.AQCTLB.bit.CBD = AQ_CLEAR;          // Clear PWM1B on event B,down count

    // Active Low PWMs - Setup Deadband
    // note: when using Deadband module, you can not set actions for EPwmxB.
    //at time, actions for EPwmxB will be setup inversion of EPwmxA as default
    EPwm11Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    EPwm11Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary --> deadtime
    EPwm11Regs.DBFED.all = 50; // FED = 50 TBCLKs
    EPwm11Regs.DBRED.all = 50; // RED = 50 TBCLKs

    // Trigger ADC SOC
     EPwm11Regs.ETSEL.bit.SOCAEN=1;
     EPwm11Regs.ETSEL.bit.SOCASEL=3;
     EPwm11Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event

   // Interrupt where we will change the Compare Values
    EPwm11Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm11Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm11Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}
void setup_epwm12(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM12=1;
    // Setup TBCLK
    EPwm12Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm12Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm12Regs.TBCTR = 0x0000;                  // Clear counter
    // Set Compare values
    EPwm12Regs.CMPA.bit.CMPA = 10;    // Set compare A value
    EPwm12Regs.CMPB.bit.CMPB = 10;    // Set Compare B value
    // Setup counter mode
    EPwm12Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm12Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm12Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;   // Sync down-stream module
    EPwm12Regs.TBCTL.bit.PHSDIR=TB_UP;           // MODE up AFTER SYNC
    EPwm12Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm12Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    // Setup shadowing
    EPwm12Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm12Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm12Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm12Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    // Set actions
    EPwm12Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on event A, up count
    EPwm12Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, down count
    EPwm12Regs.AQCTLB.bit.CBU = AQ_SET;            // Set PWM1B on event B, up count
    EPwm12Regs.AQCTLB.bit.CBD = AQ_CLEAR;          // Clear PWM1B on event B,down count

    // Active Low PWMs - Setup Deadband
    // note: when using Deadband module, you can not set actions for EPwmxB.
    //at time, actions for EPwmxB will be setup inversion of EPwmxA as default
    EPwm12Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    EPwm12Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary --> deadtime
    EPwm12Regs.DBFED.all = 50; // FED = 50 TBCLKs
    EPwm12Regs.DBRED.all = 50; // RED = 50 TBCLKs

    // Trigger ADC SOC
     EPwm12Regs.ETSEL.bit.SOCAEN=1;
     EPwm12Regs.ETSEL.bit.SOCASEL=3;
     EPwm12Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event

   // Interrupt where we will change the Compare Values
    EPwm12Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm12Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm12Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}

