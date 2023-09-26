#include "F28x_Project.h"
#include <math.h>
#include "./RANDOM/header.h"


double t, va1;
int period_sine;
int a, j , i;

void main(void)
{
    Init_Func();
    //t = 0.015;
    while(1)
        {

      //  }
        va1 = sin(w*t);

              // for (i = 0; i < 1000000; i++){
               DacaRegs.DACVALS.all = (va1 + 1) *4095 /2;
        //asm ("    NOP");
    }
}




// -------------------------------------------------------- ePWM interrupt -----------------------------------------------------------
__interrupt void epwm1_isr(void)
{
    AdcaRegs.ADCSOCFRC1.bit.SOC1 = 1; // select SOC_Number  // associated with pin ADCINA1, on INT1 (ADCINA0-INT0 = 0; ADCINA2-INT2 = 2)
    AdcaRegs.ADCSOCFRC1.bit.SOC2 = 1; // select SOC_Number  // associated with pin ADCINA1, on INT1 (ADCINA0-INT0 = 0; ADCINA2-INT2 = 2)
    AdcaRegs.ADCSOCFRC1.bit.SOC5 = 1; // select SOC_Number  // associated with pin ADCINA1, on INT1 (ADCINA0-INT0 = 0; ADCINA2-INT2 = 2)

    EPwm1Regs.ETCLR.bit.INT = 1;        // Clear INT flag for this timer

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;     // Acknowledge this interrupt to receive more interrupts from group 3

    a = AdcaResultRegs.ADCRESULT1;
    EPwm1Regs.CMPA.bit.CMPA = a/4095 * 5000;
}

//---------------------------------------------------------- Timer interrupt ----------------------------------------------------------

