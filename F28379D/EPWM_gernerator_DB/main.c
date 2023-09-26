//
// Included Files
//
#include "F28x_Project.h"
#include "main.h"
#include "math.h"
#include "setup_epwm.h"


#define     VACmeas     (AdcaResultRegs.ADCRESULT0)
#define     VPFCmeas    (AdccResultRegs.ADCRESULT1)
#define     VBATmeas    (AdcbResultRegs.ADCRESULT0)
#define     ILmeas      (AdcaResultRegs.ADCRESULT1)
#define     IBATmeas    (AdccResultRegs.ADCRESULT0)



//
// Main
//
void main(void)
{

   InitSysCtrl();
//
//   InitSpiaGpio();
// Step 3. Initialize PIE vector table:
// Disable and clear all CPU interrupts
//
   DINT;
   IER = 0x0000;
   IFR = 0x0000;

//
// Initialize PIE control registers to their default state:
// This function is found in the F2837xD_PieCtrl.c file.
//
   InitPieCtrl();
   InitPieVectTable();
// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected registers
   //PieVectTable.SPIB_RX_INT = &spiRxFifoIsr;
  // PieVectTable.SPIB_TX_INT = &spiTxFifoIsr;
   PieVectTable.EPWM1_INT = &epwm1_isr;
   EDIS;    // This is needed to disable write to EALLOW protected registers

   EALLOW;
   CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   EDIS;

   setup_epwm();
   EALLOW;
   CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
   EDIS;
//
// Step 5. User specific code, enable interrupts:
//
// Enable interrupts required for this example
//
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;     // Enable the PIE block

   // Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
   PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;  // Enable TINT0 in the PIE: Group 1 interrupt 7


    EINT;                                  // Enable Global Interrupts

   // 200 MHz enable
   EALLOW;
   ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;
   EDIS;

    while(1)
    {
        EPwm1Regs.CMPA.bit.CMPA = 0.5*EPWM1_TIMER_TBPRD;    // Set compare A value
        EPwm1Regs.CMPB.bit.CMPB = 0.5*EPWM1_TIMER_TBPRD;    // Set Compare B value
        EPwm2Regs.CMPA.bit.CMPA = 0.5*EPWM1_TIMER_TBPRD;    // Set compare A value
    }
}




//

__interrupt void epwm1_isr(void)
{
    // Clear INT flag for this timer
    //
    EPwm1Regs.ETCLR.bit.INT = 1;
    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
