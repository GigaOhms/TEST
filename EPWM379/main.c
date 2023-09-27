#include "F28x_Project.h"
#include <math.h>
#include "RANDOM\header.h"


void main(void)
{
    InitSysCtrl();
    DINT;           // Disable CPU interrupts

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    InitEPwm1();
    InitEPwm2();
    setup_ADC();
//    setup_DAC();
    setup_gpio();
    setupLEDGPIO();
//    setupVAR();
    // End call----------------------------------------------------------------------------------------------------------------
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

    InitPieCtrl();  // Initialize the PIE control registers to their default state.

// Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();     // Initialize the PIE vector table with pointers to the shell Interrupt

// ------------------------------------------------ MAP ISR functions ----------------------------------------------------------------
    EALLOW; // This is needed to write to EALLOW protected registers
    PieVectTable.EPWM1_INT = &epwm1_isr;
    EDIS;   // This is needed to disable write to EALLOW protected registers

// Enable global Interrupts and higher priority real-time debug events: --------------------------------------------------------------
    IER |=M_INT3;
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;     // Enable the PIE block
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;      // Enable EPWM INTn in the PIE: Group 3 interrupt 1-3

    // enable PIE interrupt
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;

//    Enable 200Mhz ---------------------------------------------------------------------------------------------------------------------
    EALLOW;
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;
    EDIS;

    while(1){
        if (GPIO_ReadPin(BUTTON_TOP) == 0)
            CTR_STT = 1;
        if (GPIO_ReadPin(BUTTON_BOT) == 0)
            CTR_STT = 2;
        if (CTR_STT == 0)
            BLUE_LIGHT();
        else if (CTR_STT == 1)
            RED_LIGHT();
        else if (CTR_STT == 2)
            YELLOW_LIGHT();
    }
}


// -------------------------------------------------------- ePWM interrupt -----------------------------------------------------------
__interrupt void epwm1_isr(void)
{
    EPwm1Regs.ETCLR.bit.INT = 1;                // Clear INT flag for this timer
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;     // Acknowledge this interrupt to receive more interrupts from group 3

    readSensor();
    peakDETECT();
    CalibBAT();
//    CalibPFC();


//    PFC_Control();
//    BAT_Control();
}

void PFC_Control(void){
    IL = (ILmeas - ILoffset) * ILgain;

//    PFC1.ERROR = 1 - VPFC/VBUSref;
//    PFC1.INTEGRAL = PFC1.INTEGRAL + PFC1.KI*PFC1.ERROR*Ts;
//    PFC1.OUT = PFC1.KP*PFC1.ERROR + PFC1.INTEGRAL;
//    PFC1.OUT = (PFC1.OUT < PFC1.DMIN) ? PFC1.DMIN:PFC1.OUT;
//    PFC1.OUT = (PFC1.OUT > PFC1.DMAX) ? PFC1.DMAX:PFC1.OUT;
//
//    PFC2.ERROR = VAC/VACpeak * PFC1.OUT - IL;
//    PFC2.INTEGRAL = PFC2.INTEGRAL + PFC2.KI*PFC2.ERROR*Ts;
//    PFC2.OUT = PFC2.KP*PFC2.ERROR + PFC2.INTEGRAL;
//    PFC2.OUT = (PFC2.OUT < PFC2.DMIN) ? PFC2.DMIN:PFC2.OUT;
//    PFC2.OUT = (PFC2.OUT > PFC2.DMAX) ? PFC2.DMAX:PFC2.OUT;

    PFC1_OUT = DCL_runPIc(&PFC1, 1.0f, VPFC * divVPFCref);
    float ILref = VAC/VACpeak * PFC1_OUT;
    PFC2_OUT = DCL_runPIc(&PFC2, ILref, IL);

    EPwm2Regs.CMPA.bit.CMPA = PFC2_OUT * 1000;
//    EPwm2Regs.CMPB.bit.CMPB = PFC2.OUT * 1000;
}

void BAT_Control(void){
// IBAT = (IBATavg - IBAToffset) * IBATgain;
    if (CTR_STT == 0)
        isFULL();
    else if (CTR_STT == 1)
        BAT_CC();
    else if (CTR_STT == 2)
        BAT_CV();
}

void BAT_CC(void){
    IBAT = (IBATavg - IBAToffset) * IBATgain;

//    BAT.ERROR = IBATref - IBAT;
//    BAT.INTEGRAL = BAT.INTEGRAL + BAT.KI*BAT.ERROR*Ts;
//    BAT.OUT = BAT.KP*BAT.ERROR + BAT.INTEGRAL;
//
//    BAT.OUT = (BAT.OUT < BAT.DMIN) ? BAT.DMIN:BAT.OUT;
//    BAT.OUT = (BAT.OUT > BAT.DMAX) ? BAT.DMAX:BAT.OUT;
//
//    EPwm1Regs.CMPA.bit.CMPA = BAT.OUT * 1000;
//    EPwm1Regs.CMPB.bit.CMPB = BAT.OUT * 1000;

    // ------------- NEW CODE ---------------
    BAT_OUT = 1000 * DCL_runPIc(&BATTERY, IBATref, IBAT);
    EPwm1Regs.CMPA.bit.CMPA = BAT_OUT;
    EPwm1Regs.CMPB.bit.CMPB = BAT_OUT;
}

void BAT_CV(void){
//    BAT.ERROR = VBATref - VBAT;
//    BAT.INTEGRAL = BAT.INTEGRAL + BAT.KI*BAT.ERROR*Ts;
//    BAT.OUT = BAT.KP*BAT.ERROR + BAT.INTEGRAL;
//
//    BAT.OUT = (BAT.OUT < BAT.DMIN) ? BAT.DMIN:BAT.OUT;
//    BAT.OUT = (BAT.OUT > BAT.DMAX) ? BAT.DMAX:BAT.OUT;
//
//    EPwm1Regs.CMPA.bit.CMPA = BAT.OUT * 1000;
//    EPwm1Regs.CMPB.bit.CMPB = BAT.OUT * 1000;

    // ------------- NEW CODE ---------------
    BAT_OUT = 1000 * DCL_runPIc(&BATTERY, VBATref, VBAT);
    EPwm1Regs.CMPA.bit.CMPA = BAT_OUT;
    EPwm1Regs.CMPB.bit.CMPB = BAT_OUT;
}

void isFULL(void){
    BLUE_LIGHT();

    EPwm2Regs.CMPA.bit.CMPA = 0;
    EPwm2Regs.CMPB.bit.CMPB = 0;
    EPwm1Regs.CMPA.bit.CMPA = 0;
    EPwm1Regs.CMPB.bit.CMPB = 0;
}

void peakDETECT(void)
{
//    CalibVAC(); // ----------------------------------------------- OPEN COMMENT
    if (count > 1000){
        VPFCavg = sumVPFC/1000.0;
        VBATavg = sumVBAT/1000.0;
        VACrms = sqrt(sumVAC/1000.0);
        VACpeak = VACrms * SQRT2;
        IBATavg = sumIBAT/1000.0;


        count = 0;
        sumVAC = 0;
        sumVBAT = 0;
        sumVPFC = 0;
        sumIBAT = 0;

    }
    count++;
    sumVPFC += VPFCmeas;
    sumVBAT += VBATmeas;
    sumVAC += VAC*VAC;
    sumIBAT += IBATmeas;

}




