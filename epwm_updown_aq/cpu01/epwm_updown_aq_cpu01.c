//###########################################################################
//
// FILE:   epwm_updown_aq_cpu01.c
//
// TITLE:  Action Qualifier Module - Using up/down count.
//
//! \addtogroup cpu01_example_list
//! <h1> EPWM Action Qualifier (epwm_updown_aq)</h1>
//!
//! This example configures ePWM1, ePWM2, ePWM3 to produce an
//! waveform with independent modulation on EPWMxA and
//! EPWMxB.
//!
//! The compare values CMPA and CMPB are modified within the ePWM's ISR.
//!
//! The TB counter is in up/down count mode for this example.
//!
//! View the EPWM1A/B(PA0_GPIO0 & PA1_GPIO1), EPWM2A/B(PA2_GPIO2 & PA3_GPIO3)
//! and EPWM3A/B(PA4_GPIO4 & PA5_GPIO5) waveforms via an oscilloscope.
//!
//
//###########################################################################
// $TI Release: F2837xD Support Library v210 $
// $Release Date: Tue Nov  1 14:46:15 CDT 2016 $
// $Copyright: Copyright (C) 2013-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

//
// Included Files
//
#include "F28x_Project.h"
#include "math.h"

//
// Defines
//

 double t = 0.0;
 double vaa, vbb, vcc;
volatile int STATUS = 0;

#define pi 3.1415926535897932384626433832795
#define F 50.0
#define LED1    58      // Xanh

#define EPWM1_TIMER_TBPRD  5000  // Period register
#define EPWM1_MAX_CMPA     1000
#define EPWM1_MIN_CMPA     1000
#define EPWM1_MAX_CMPB     1000
#define EPWM1_MIN_CMPB     1000

#define EPWM2_TIMER_TBPRD  5000  // Period register
#define EPWM2_MAX_CMPA     1000
#define EPWM2_MIN_CMPA     1000
#define EPWM2_MAX_CMPB     1000
#define EPWM2_MIN_CMPB     1000

#define EPWM3_TIMER_TBPRD  5000  // Period register
#define EPWM3_MAX_CMPA      1000
#define EPWM3_MIN_CMPA      1000
#define EPWM3_MAX_CMPB      1000
#define EPWM3_MIN_CMPB      1000

#define EPWM_CMP_UP           1
#define EPWM_CMP_DOWN         0

//
// Globals
//
typedef struct
{
    volatile struct EPWM_REGS *EPwmRegHandle;
    Uint16 EPwm_CMPA_Direction;
    Uint16 EPwm_CMPB_Direction;
    Uint16 EPwmTimerIntCount;
    Uint16 EPwmMaxCMPA;
    Uint16 EPwmMinCMPA;
    Uint16 EPwmMaxCMPB;
    Uint16 EPwmMinCMPB;
}EPWM_INFO;

EPWM_INFO epwm1_info;
EPWM_INFO epwm2_info;
EPWM_INFO epwm3_info;

//
// Function Prototypes
//
//void setupLEDGPIO(void);
__interrupt void cpuTimer0ISR(void);
void setup_DAC(void);
void InitEPwm1Example(void);
void InitEPwm2Example(void);
void InitEPwm3Example(void);
__interrupt void epwm1_isr(void);
__interrupt void epwm2_isr(void);
__interrupt void epwm3_isr(void);
//void update_compare(EPWM_INFO*);

//
// Main
//
void main(void)
{
//
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xD_SysCtrl.c file.
//



    InitSysCtrl();

//
// Step 2. Initialize GPIO:
// This example function is found in the F2837xD_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
//
//    InitGpio();

//
// enable PWM1, PWM2 and PWM3
//
    CpuSysRegs.PCLKCR2.bit.EPWM1=1;
    CpuSysRegs.PCLKCR2.bit.EPWM2=1;
    CpuSysRegs.PCLKCR2.bit.EPWM3=1;

//
// For this case just init GPIO pins for ePWM1, ePWM2, ePWM3
// These functions are in the F2837xD_EPwm.c file
//
//    setupLEDGPIO();
    setup_DAC();
    InitEPwm1Gpio();
    InitEPwm2Gpio();
    InitEPwm3Gpio();

//
// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
//
    DINT;

//
// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the F2837xD_PieCtrl.c file.
//
    InitPieCtrl();

//
// Disable CPU interrupts and clear all CPU interrupt flags:
//
    IER = 0x0000;
    IFR = 0x0000;

//
// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2837xD_DefaultIsr.c.
// This function is found in F2837xD_PieVect.c.
//
    InitPieVectTable();

//
// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
//
    EALLOW; // This is needed to write to EALLOW protected registers
    PieVectTable.EPWM1_INT = &epwm1_isr;
    PieVectTable.TIMER0_INT = &cpuTimer0ISR;

   //PieVectTable.EPWM2_INT = &epwm2_isr;
  //  PieVectTable.EPWM3_INT = &epwm3_isr;
    EDIS;   // This is needed to disable write to EALLOW protected registers

    InitCpuTimers();   // CPU Timers.
    ConfigCpuTimer(&CpuTimer0, 200, 50); // interrupt every second: 100MHz CPU Freq, 1 second Period (in uSeconds)


//
// For this example, only initialize the ePWM
//
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    InitEPwm1Example();
    InitEPwm2Example();
    InitEPwm3Example();

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;     // Enable the PIE block

    PieCtrlRegs.PIEIER6.bit.INTx1 = 1;     // Enable PIE Group 6, INT 1
    PieCtrlRegs.PIEIER6.bit.INTx2 = 1;     // Enable PIE Group 6, INT 2
    // Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;  // Enable TINT0 in the PIE: Group 1 interrupt 7
      // Enable CPU INT3 which is connected to EPWM1-3 INT:
     IER |= M_INT6;  //CPU int1 -- CPU-Timer 0,
     IER |= M_INT1;  //CPU int1 -- CPU-Timer 0,

     EINT;                                  // Enable Global Interrupts

     CpuTimer0Regs.TCR.all = 0x4000;

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

//
// Step 4. User specific code, enable interrupts:
//

//
// Enable CPU INT3 which is connected to EPWM1-3 INT:
//
    IER |= M_INT3;

//
// Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
//
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
   // PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
   // PieCtrlRegs.PIEIER3.bit.INTx3 = 1;

//
// Enable global Interrupts and higher priority real-time debug events:
//
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

//
// Step 5. IDLE loop. Just sit and loop forever (optional):
//

    EALLOW;
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;
    EDIS;

 while(1)
    {
        vaa = (sin(2.0*pi*F*t) + 1.0) / 2.0;
        vbb = (sin(2.0*pi*F*t + 2.0*pi/3.0) + 1.0) / 2.0;
        vcc = (sin(2.0*pi*F*t + 4.0*pi/3.0) + 1.0) / 2.0;


        EPwm1Regs.CMPA.bit.CMPA = vaa * EPWM1_TIMER_TBPRD;
        EPwm1Regs.CMPB.bit.CMPB = vaa * EPWM1_TIMER_TBPRD;

        EPwm2Regs.CMPA.bit.CMPA = vbb * EPWM1_TIMER_TBPRD;
        EPwm2Regs.CMPB.bit.CMPB = vbb * EPWM1_TIMER_TBPRD;

        EPwm3Regs.CMPA.bit.CMPA = vcc * EPWM1_TIMER_TBPRD;
        EPwm3Regs.CMPB.bit.CMPB = vcc * EPWM1_TIMER_TBPRD;
//        asm ("    NOP");
    }
}


void setup_DAC(void){
    EALLOW;
    DacaRegs.DACCTL.bit.DACREFSEL = 1;
    DacaRegs.DACOUTEN.bit.DACOUTEN = 1;
    DacaRegs.DACVALS.all = 0;
    DELAY_US(10); // Delay for buffered DAC to power up
    EDIS;
}

__interrupt void cpuTimer0ISR(void)
{
    DacaRegs.DACVALS.all = vaa * 4095;
    t += 0.0001;
    if (t >= 0.0199)
        t = 0.0;

    CpuTimer0.InterruptCount++;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//
// epwm1_isr - EPWM1 ISR
//
__interrupt void epwm1_isr(void)
{

//        GPIO_WritePin(LED1, STATUS);
//        STATUS = ~STATUS;


    //
    // Update the CMPA and CMPB values
    //
//    update_compare(&epwm1_info);

    //
    // Clear INT flag for this timer
    //
    EPwm1Regs.ETCLR.bit.INT = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

//
// epwm2_isr - EPWM2 ISR
//
__interrupt void epwm2_isr(void)
{
    //
    // Update the CMPA and CMPB values
    //
//    update_compare(&epwm2_info);

    //
    // Clear INT flag for this timer
    //
    EPwm2Regs.ETCLR.bit.INT = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

//
// epwm3_isr - EPWM3 ISR
//
__interrupt void epwm3_isr(void)
{
    //
    // Update the CMPA and CMPB values
    //
//    update_compare(&epwm3_info);

    //
    // Clear INT flag for this timer
    //
    EPwm3Regs.ETCLR.bit.INT = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

//
// InitEPwm1Example - Initialize EPWM1 configuration
//
void InitEPwm1Example(void)
{
    //
    // Setup TBCLK
    //
    EPwm1Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm1Regs.TBCTR = 0x0000;                  // Clear counter

    //
    // Set Compare values
    //
    EPwm1Regs.CMPA.bit.CMPA = EPWM1_MAX_CMPA;    // Set compare A value
    EPwm1Regs.CMPB.bit.CMPB = EPWM1_MAX_CMPB;    // Set Compare B value

    //
    // Setup counter mode
    //
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
//    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;

    //
    // Setup shadowing
    //
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Set actions
    //
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // Set PWM1A on event A, up
                                                  // count
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;          // Clear PWM1A on event A,
                                                  // down count

    EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // Set PWM1B on event B, up
                                                  // count
    EPwm1Regs.AQCTLB.bit.CBD = AQ_SET;          // Clear PWM1B on event B,
                                                  // down count

    //
    // Interrupt where we will change the Compare Values
    //
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;     // Select INT on Zero event
    EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event
}

void InitEPwm2Example()
{
    //
    // Setup TBCLK
    //
    EPwm2Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm2Regs.TBCTR = 0x0000;                  // Clear counter

    //
    // Set Compare values
    //
    EPwm2Regs.CMPA.bit.CMPA = EPWM1_MAX_CMPA;    // Set compare A value
    EPwm2Regs.CMPB.bit.CMPB = EPWM1_MAX_CMPB;    // Set Compare B value

    //
    // Setup counter mode
    //
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;
//    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;

    //
    // Setup shadowing
    //
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Set actions
    //
    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // Set PWM1A on event A, up
                                                  // count
    EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;          // Clear PWM1A on event A,
                                                  // down count

    EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // Set PWM1B on event B, up
                                                  // count
    EPwm2Regs.AQCTLB.bit.CBD = AQ_SET;          // Clear PWM1B on event B,
                                                  // down count

    //
    // Interrupt where we will change the Compare Values
    //
    EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm2Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm2Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event
}

void InitEPwm3Example()
{
    //
    // Setup TBCLK
    //
    EPwm3Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm3Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm3Regs.TBCTR = 0x0000;                  // Clear counter

    //
    // Set Compare values
    //
    EPwm3Regs.CMPA.bit.CMPA = EPWM1_MAX_CMPA;    // Set compare A value
    EPwm3Regs.CMPB.bit.CMPB = EPWM1_MAX_CMPB;    // Set Compare B value

    //
    // Setup counter mode
    //
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;
//    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;

    //
    // Setup shadowing
    //
    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Set actions
    //
    EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // Set PWM1A on event A, up
                                                  // count
    EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;          // Clear PWM1A on event A,
                                                  // down count

    EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // Set PWM1B on event B, up
                                                  // count
    EPwm3Regs.AQCTLB.bit.CBD = AQ_SET;          // Clear PWM1B on event B,
                                                  // down count

    //
    // Interrupt where we will change the Compare Values
    //
    EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm3Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm3Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event
}


//
// InitEPwm2Example - Initialize EPWM2 configuration
//
//void InitEPwm2Example()
//{
//    //
//    // Setup TBCLK
//    //
//    EPwm2Regs.TBPRD = EPWM2_TIMER_TBPRD;         // Set timer period 801 TBCLKs
//    EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;          // Phase is 0
//    EPwm2Regs.TBCTR = 0x0000;                    // Clear counter
//
//    //
//    // Set Compare values
//    //
//    EPwm2Regs.CMPA.bit.CMPA = EPWM2_MAX_CMPA;    // Set compare A value
//    EPwm2Regs.CMPB.bit.CMPB = EPWM2_MAX_CMPB;    // Set Compare B value
//
//    //
//    // Setup counter mode
//    //
//    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
//    EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
//    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
//    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;
//
//    //
//    // Setup shadowing
//    //
//    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
//    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
//    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
//    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
//
//    //
//    // Set actions
//    //
//    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;         // Set PWM2A on event A, up
//                                               // count
//    EPwm2Regs.AQCTLA.bit.CBD = AQ_SET;       // Clear PWM2A on event B, down
//                                               // count
//
//    EPwm2Regs.AQCTLB.bit.ZRO = AQ_CLEAR;       // Clear PWM2B on zero
//    EPwm2Regs.AQCTLB.bit.PRD = AQ_SET;         // Set PWM2B on period
//
//    //
//    // Interrupt where we will change the Compare Values
//    //
//    EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;    // Select INT on Zero event
//    EPwm2Regs.ETSEL.bit.INTEN = 1;               // Enable INT
//    EPwm2Regs.ETPS.bit.INTPRD = ET_3RD;          // Generate INT on 3rd event
//}
//
////
//// InitEPwm3Example - Initialize EPWM3 configuration
////
//void InitEPwm3Example(void)
//{
//    //
//    // Setup TBCLK
//    //
//    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up/down and down
//    EPwm3Regs.TBPRD = EPWM3_TIMER_TBPRD;           // Set timer period
//    EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
//    EPwm3Regs.TBPHS.bit.TBPHS = 0x0000;            // Phase is 0
//    EPwm3Regs.TBCTR = 0x0000;                      // Clear counter
//    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
//    EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;
//
//    //
//    // Setup shadow register load on ZERO
//    //
//    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
//    EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
//    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
//    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
//
//    //
//    // Set Compare values
//    //
//    EPwm3Regs.CMPA.bit.CMPA = EPWM3_MAX_CMPA;   // Set compare A value
//    EPwm3Regs.CMPB.bit.CMPB = EPWM3_MAX_CMPB;   // Set Compare B value
//
//    //
//    // Set Actions
//    //
//    EPwm3Regs.AQCTLA.bit.PRD = AQ_CLEAR;         // Set PWM3A on period
//    EPwm3Regs.AQCTLA.bit.CBD = AQ_SET;       // Clear PWM3A on event B, down
//                                               // count
//
//    EPwm3Regs.AQCTLB.bit.PRD = AQ_CLEAR;       // Clear PWM3A on period
//    EPwm3Regs.AQCTLB.bit.CAU = AQ_SET;         // Set PWM3A on event A, up
//                                               // count
//
//    //
//    // Interrupt where we will change the Compare Values
//    //
//    EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;    // Select INT on Zero event
//    EPwm3Regs.ETSEL.bit.INTEN = 1;               // Enable INT
//    EPwm3Regs.ETPS.bit.INTPRD = ET_3RD;          // Generate INT on 3rd event
//}

//
// update_compare - Update the PWM compare values
//
//void update_compare(EPWM_INFO *epwm_info)
//{
//    //
//    // Every 10'th interrupt, change the CMPA/CMPB values
//    //
//    if(epwm_info->EPwmTimerIntCount == 10)
//    {
//        epwm_info->EPwmTimerIntCount = 0;
//
//        //
//        // If we were increasing CMPA, check to see if
//        // we reached the max value.  If not, increase CMPA
//        // else, change directions and decrease CMPA
//        //
//        if(epwm_info->EPwm_CMPA_Direction == EPWM_CMP_UP)
//        {
//            if(epwm_info->EPwmRegHandle->CMPA.bit.CMPA <
//               epwm_info->EPwmMaxCMPA)
//            {
//                epwm_info->EPwmRegHandle->CMPA.bit.CMPA++;
//            }
//            else
//            {
//                epwm_info->EPwm_CMPA_Direction = EPWM_CMP_DOWN;
//                epwm_info->EPwmRegHandle->CMPA.bit.CMPA--;
//            }
//        }
//
//        //
//        // If we were decreasing CMPA, check to see if
//        // we reached the min value.  If not, decrease CMPA
//        // else, change directions and increase CMPA
//        //
//        else
//        {
//            if(epwm_info->EPwmRegHandle->CMPA.bit.CMPA ==
//               epwm_info->EPwmMinCMPA)
//            {
//                epwm_info->EPwm_CMPA_Direction = EPWM_CMP_UP;
//                epwm_info->EPwmRegHandle->CMPA.bit.CMPA++;
//            }
//            else
//            {
//                epwm_info->EPwmRegHandle->CMPA.bit.CMPA--;
//            }
//        }
//
//        //
//        // If we were increasing CMPB, check to see if
//        // we reached the max value.  If not, increase CMPB
//        // else, change directions and decrease CMPB
//        //
//        if(epwm_info->EPwm_CMPB_Direction == EPWM_CMP_UP)
//        {
//            if(epwm_info->EPwmRegHandle->CMPB.bit.CMPB < epwm_info->EPwmMaxCMPB)
//            {
//                epwm_info->EPwmRegHandle->CMPB.bit.CMPB++;
//            }
//            else
//            {
//                epwm_info->EPwm_CMPB_Direction = EPWM_CMP_DOWN;
//                epwm_info->EPwmRegHandle->CMPB.bit.CMPB--;
//            }
//        }
//
//        //
//        // If we were decreasing CMPB, check to see if
//        // we reached the min value.  If not, decrease CMPB
//        // else, change directions and increase CMPB
//        //
//        else
//        {
//            if(epwm_info->EPwmRegHandle->CMPB.bit.CMPB == epwm_info->EPwmMinCMPB)
//            {
//                epwm_info->EPwm_CMPB_Direction = EPWM_CMP_UP;
//                epwm_info->EPwmRegHandle->CMPB.bit.CMPB++;
//            }
//            else
//            {
//                epwm_info->EPwmRegHandle->CMPB.bit.CMPB--;
//            }
//        }
//    }
//    else
//    {
//        epwm_info->EPwmTimerIntCount++;
//    }
//
//    return;
//}

//
// End of file
//
