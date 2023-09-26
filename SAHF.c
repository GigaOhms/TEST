`/****************vbg***************************************************************
 * @filename: SAHF.c
 * @Author  : phongnh3289
 * @Date    : Nov 24, 2019
 * @Email   : phongnh3289@gmail.com
 ******************************************************************************/

/*******************************************************************************
 *                               HISTORY
 *  1.0.0   Nov 24, 2019     phongnh3289        : Initial revision
 *
 ******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/

#include "PeripheralHeaderIncludes.h"
#include "SAHF-Settings.h"
#include "IQmathLib.h"
#include <math.h>
#include "Solar_F.h"
#include "SAHF.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/
extern unsigned int RamfuncsLoadStart;
extern unsigned int RamfuncsLoadEnd;
extern unsigned int RamfuncsRunStart;

interrupt void MainISR(void);
interrupt void OffsetISR(void);
// Functions that will be run from RAM
#pragma CODE_SECTION(MainISR,"ramfuncs");
#pragma CODE_SECTION(OffsetISR,"ramfuncs");
/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/
PWMGEN pwm1 = PWMGEN_DEFAULTS;
SVGEN svgen1 = SVGEN_DEFAULTS;
PIDATA pi_id = PIDATA_DEFAULTS;
PIDATA pi_iq = PIDATA_DEFAULTS;
PIDATA pi_vdc = PIDATA_DEFAULTS;
DLOG_4CH dlog = DLOG_4CH_DEFAULTS;
DATA3P vref = DATA3P_DEFAULTS;
DATA3P cur_load = DATA3P_DEFAULTS;
DATA3P cur_inv = DATA3P_DEFAULTS;
DATA3P vols = DATA3P_DEFAULTS;
SPLL_3ph_SRF_F pll;
RMPCNTL rc1 = RMPCNTL_DEFAULTS;

void main(void)
{
    DeviceInit();
    en_driver(0);
    CpuTimer0Regs.PRD.all =  mSec1;     // A tasks
    CpuTimer1Regs.PRD.all =  mSec50*8;     // B tasks
    CpuTimer2Regs.PRD.all =  mSec50*4;  // C tasks
    Alpha_State_Ptr = &A0;
// Initialize PWM module
    pwm1.PeriodMax = SYSTEM_FREQUENCY*1000000*T/2;  // Prescaler X1 (T1), ISR period = T x 1
    pwm1.HalfPerMax=pwm1.PeriodMax/2;
    pwm1.Deadband  = 2.0*SYSTEM_FREQUENCY;          // 120 counts -> 2.0 usec for TBCLK = SYSCLK/1
    PWM_INIT_MACRO(1,2,3,pwm1)
// Initialize DATALOG module
    dlog.iptr1 = &DlogCh1;
    dlog.iptr2 = &DlogCh2;
    dlog.iptr3 = &DlogCh3;
    dlog.iptr4 = &DlogCh4;
    dlog.trig_value = 0x1;
    dlog.size = 0x0C8;
    dlog.prescalar = 5;
    dlog.init(&dlog);
    SPLL_3ph_SRF_F_init(GRID_FREQ,((float)(0.001/ISR_FREQUENCY)),&pll);
    pi_id.Kp=_IQ(10);//1 for 3mH inductor
    pi_id.Ki=_IQ(0.1);//0.01 for 3mH inductor
    pi_id.Umax =_IQ(100.0);
    pi_id.Umin =_IQ(-100.0);
// Initialize the RAMPGEN module
    pi_iq.Kp=_IQ(10);//1 for 3mH inductor
    pi_iq.Ki=_IQ(0.1);//0.01 for 3mH inductor
    pi_iq.Umax =_IQ(100.0);
    pi_iq.Umin =_IQ(-100.0);
    pi_vdc.Kp=_IQ(0.1);//1 for 3mH inductor
    pi_vdc.Ki=_IQ(5);
    pi_vdc.Umax =_IQ(10.0);
    pi_vdc.Umin =_IQ(-10.0);
    pi_id.Ref = _IQ(2.0);
    pi_iq.Ref = _IQ(0.0);
// Initialize ADC for DMC Kit Rev 1.1
    ChSel[0]=1;     // Dummy meas. avoid 1st sample issue Rev0 Picollo*/
    ChSel[1]=1;     // ChSelect: ADC A1-> Phase A source Current
    ChSel[2]=2;     // ChSelect: ADC A2-> Phase B source Current
    ChSel[3]=3;     // ChSelect: ADC A3-> Phase A Voltage
    ChSel[4]=4; // ChSelect: ADC A4-> Phase B Voltage
    ChSel[5]=5; // ChSelect: ADC A5-> Phase A load Current
    ChSel[6]=6; // ChSelect: ADC A6-> Phase B load Current
    ChSel[7]=7;     // ChSelect: ADC A7-> DC Bus  Voltage
// Initialize ADC module
    ADC_MACRO_INIT(ChSel,TrigSel,ACQPS)
    EALLOW; // This is needed to write to EALLOW protected registers
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;
    PieVectTable.EPWM1_INT = &OffsetISR;
    EDIS;
//  Enable PIE group 3 interrupt 1 for EPWM1_INT
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
//  Enable CNT_zero interrupt using EPWM1 Time-base
    EPwm1Regs.ETSEL.bit.INTEN = 1;   // Enable EPWM1INT generation
    EPwm1Regs.ETSEL.bit.INTSEL = 2;  // Enable interrupt period event
    EPwm1Regs.ETPS.bit.INTPRD = 1;   // Generate interrupt on the 1st event
    EPwm1Regs.ETCLR.bit.INT = 1;     // Enable more interrupts
// Enable CPU INT3 for EPWM1_INT:
    IER |= M_INT3;
// Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
// IDLE loop. Just sit and loop forever:

    for(;;)  //infinite loop
    {
        // State machine entry & exit point
        //===========================================================
        (*Alpha_State_Ptr)();   // jump to an Alpha state (A0,B0,...)
        //===========================================================

    }
} //END MAIN CODE

//--------------------------------- FRAMEWORK -------------------------------------
void A0(void)
{
    // loop rate synchronizer for A-tasks
    if(CpuTimer0Regs.TCR.bit.TIF == 1)
    {
        CpuTimer0Regs.TCR.bit.TIF = 1;  // clear flag
    }

    Alpha_State_Ptr = &B0;      // Comment out to allow only A tasks
}

void B0(void)
{
    // loop rate synchronizer for B-tasks
    if(CpuTimer1Regs.TCR.bit.TIF == 1)
    {
        CpuTimer1Regs.TCR.bit.TIF = 1;              // clear flag
        GpioDataRegs.GPBTOGGLE.bit.GPIO61=1;
    }

    Alpha_State_Ptr = &C0;      // Allow C state tasks
}

void C0(void)
{
    // loop rate synchronizer for C-tasks
    if(CpuTimer2Regs.TCR.bit.TIF == 1)
    {
        CpuTimer2Regs.TCR.bit.TIF = 1;              // clear flag
        GpioDataRegs.GPBTOGGLE.bit.GPIO60=1;
        //PrintNumber(number_count);
        if((OverVoltage_DC_Bus==1)||(OverCurrent_Inverter==1)){
        number_count++;
        OverVoltage_DC_Bus=0;
        OverCurrent_Inverter=0;
        if(number_count>10){
            number_count=0;
            en_pwm = 0;
        }
       }
    }
    Alpha_State_Ptr = &A0;  // Back to State A0
}

interrupt void MainISR(void)
{
//GpioDataRegs.GPASET.bit.GPIO12=1;
        if(en_pwm==1){
            en_driver(1);en_pwm=3;
            pi_id.Ref = 0;pi_iq.Ref = 0;
            pi_id.Out = 0; pi_id.ui = 0; pi_id.i1 = 0;pi_vdc.i1 = 0;
            pi_iq.Out = 0; pi_iq.ui = 0; pi_iq.i1 = 0;pi_vdc.ui = 0;
            vref.Ds = 0;   vref.Qs = 0;
        }
        if(en_pwm==0){en_driver(0);en_pwm=3;}
        cur_inv.As=((AdcMirror.ADCRESULT5)*3.3/4096-offsetC)*5.428;     //Load curr Phase A
        cur_inv.Bs=((AdcMirror.ADCRESULT6)*3.3/4096-offsetD)*5.428;     //Load curr Phase B
        vols.Bs=((AdcMirror.ADCRESULT3)*3.3/4096-offsetA)*62;        //Volt Phase A
        vols.As=((AdcMirror.ADCRESULT4)*3.3/4096-offsetB)*62;        //Volt Phase B
        cur_load.As=((AdcMirror.ADCRESULT1)*3.3/4096-offsetE)*5.428;       //Source curr Phase A
        cur_load.Bs=((AdcMirror.ADCRESULT2)*3.3/4096-offsetF)*5.428;       //Source curr Phase B
//--------------------------------------------------------------
        vols.Bs = fir(vols.Bs,VbDelay,coeffs);
        vols.As = fir(vols.As,VaDelay,coeffs);
        //cur_inv.As = fir(cur_inv.As,IaDelay,coeffs);
        //cur_inv.Bs = fir(cur_inv.Bs,IbDelay,coeffs);
        //cur_load.As = fir(cur_load.As,Il_aDelay,coeffs);
        //cur_load.Bs = fir(cur_load.Bs,Il_bDelay,coeffs);
        //if((cur_source.As>6)||(cur_source.As<-6)||(cur_source.Bs>6)||(cur_source.Bs<-6)){en_driver(0);en_pwm=0;}
        vols.Cs = 0 -vols.As - vols.Bs;
        cur_inv.Cs = 0 - cur_inv.As - cur_inv.Bs;
        cur_load.Cs = 0 -cur_load.As - cur_load.Bs;
        //Vdc link--------------------------------------------------------------
        Vdc_Real=(AdcMirror.ADCRESULT7)*3.3*K_VDC/4096;
        Vdc_Real = fir(Vdc_Real,VdcDelay,coeffsDC);
        pi_vdc.Ref = Vdc_ref;
        pi_vdc.Fbk = Vdc_Real;
        PI_CONTROLER(&pi_vdc);
//Protection over voltage bus DC, current inverter with time delay approximate 1 second
        if(Vdc_Real>130)OverVoltage_DC_Bus=1;
        if((cur_inv.As>6)||(cur_inv.Bs>6)||(cur_inv.Cs>6))OverCurrent_Inverter=1;
        //rc1.TargetValue = IdRef;
        //RC_MACRO(rc1)
        //rg1.Freq = rc1.SetpointValue;
        //RG_MACRO(rg1)
        vols.Sin = (float)sin(pll.theta[1]);
        vols.Cos = (float)cos(pll.theta[1]);
        ABC_DQ(&vols);
//--------------------------------------------------------------
        cur_inv.Sin = (float)sin(pll.theta[1]-cos_phi);
        cur_inv.Cos = (float)cos(pll.theta[1]-cos_phi);
        ABC_DQ(&cur_inv);
//--------------------------------------------------------------
        cur_load.Sin = (float)sin(pll.theta[1]);
        cur_load.Cos = (float)cos(pll.theta[1]);
        ABC_DQ(&cur_load);
//--------------------------------------------------------------
        pll.v_q[0] = vols.Qs;
        SPLL_3ph_SRF_F_FUNC(&pll);
//--------------------------------------------------------------
        pi_vdc.Out=fir(pi_vdc.Out,VdcOutDelay,coeffsDC);
        pi_id.Fbk = cur_inv.Ds;
        cur_load.DsF=fir(cur_load.Ds,IdcDelay,coeffsDC);
        pi_id.Ref = -(cur_load.Ds  - cur_load.DsF - pi_vdc.Out);
        //pi_id.Ref = curr_test.Ds- pi_vdc.Out;
        pi_id.Ref = (pi_id.Ref>_IQ(15))?_IQ(15):pi_id.Ref;
        pi_id.Ref = (pi_id.Ref<_IQ(-15))?_IQ(-15):pi_id.Ref;
        PI_CONTROLER(&pi_id);
//--------------------------------------------------------------
        pi_iq.Fbk = cur_inv.Qs;
        cur_load.QsF=fir(cur_load.Qs,IqcDelay,coeffsDC);
        pi_iq.Ref = -(cur_load.Qs-cur_load.QsF);
        pi_iq.Ref = (pi_iq.Ref>_IQ(15))?_IQ(15):pi_iq.Ref;
        pi_iq.Ref = (pi_iq.Ref<_IQ(-15))?_IQ(-15):pi_iq.Ref;
        PI_CONTROLER(&pi_iq);
//--------------------------------------------------------------
        vref.Ds = pi_id.Out - _IQmpy(_IQ(ZLpu),cur_inv.Qs) + vols.Ds;
        vref.Ds = (vref.Ds>_IQ(200))?_IQ(200):vref.Ds;
        vref.Qs = pi_iq.Out + _IQmpy(_IQ(ZLpu),cur_inv.Ds) + vols.Qs;
        vref.Qs = (vref.Qs>_IQ(200))?_IQ(200):vref.Qs;
        vref.Qs = (vref.Qs<_IQ(-200))?_IQ(-200):vref.Qs;
        vref.Cos = vols.Cos;
        vref.Sin = vols.Sin;
        DQ_ABC(&vref);
//--------------------------------------------------------------
        M_DieuChe=sqrt(3)*sqrt(vref.Alpha*vref.Alpha+vref.Beta*vref.Beta)/Vdc_Real;
        if(M_DieuChe>=0.95)M_DieuChe=0.95;
//--------------------------------------------------------------
        svgen1.Ualpha = vref.Alpha*M_DieuChe/sqrt(vref.Alpha*vref.Alpha+vref.Beta*vref.Beta);
        svgen1.Ubeta = vref.Beta*M_DieuChe/sqrt(vref.Alpha*vref.Alpha+vref.Beta*vref.Beta);
        SVGENDQ_MACRO(svgen1)
//--------------------------------------------------------------
        pwm1.MfuncC1 = svgen1.Ta;
        pwm1.MfuncC2 = svgen1.Tb;
        pwm1.MfuncC3 = svgen1.Tc;
        PWM_MACRO(1,2,3,pwm1)
//Datalogger
        DlogCh1 = _IQtoQ10(vols.As);
        DlogCh2 = _IQtoQ10(cur_load.As);
        DlogCh3 = _IQtoQ10(cur_inv.Bs);
        DlogCh4 = _IQtoQ10(pll.theta[1]);
        dlog.update(&dlog);
// Enable more interrupts from this timer
    EPwm1Regs.ETCLR.bit.INT = 1;
// Acknowledge interrupt to recieve more interrupts from PIE group 3
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    GpioDataRegs.GPACLEAR.bit.GPIO12=1;
}// MainISR Ends Here

interrupt void OffsetISR(void)
{
// Verifying the ISR
    IsrTicker++;
// DC offset measurement for ADC
    if (IsrTicker>=5000)
        {
            offsetA= K1*offsetA + K2*(AdcMirror.ADCRESULT3)*3.3/4096;           //Volt Phase A offset
            offsetB= K1*offsetB + K2*(AdcMirror.ADCRESULT4)*3.3/4096;           //Volt Phase B offset
            offsetC= K1*offsetC + K2*(AdcMirror.ADCRESULT5)*3.3/4096;           //Load curr Phase B offset
            offsetD= K1*offsetD + K2*(AdcMirror.ADCRESULT6)*3.3/4096;           //Load curr Phase B offset
            offsetE= K1*offsetE + K2*(AdcMirror.ADCRESULT1)*3.3/4096;           //Source curr Phase A offset
            offsetF= K1*offsetF + K2*(AdcMirror.ADCRESULT2)*3.3/4096;            //Source curr Phase B offset
        }
    if (IsrTicker > 25000)
    {
        //en_pwm=1;
        EALLOW;
        PieVectTable.EPWM1_INT = &MainISR;
        EDIS;
    }
// Enable more interrupts from this timer
    EPwm1Regs.ETCLR.bit.INT = 1;
// Acknowledge interrupt to recieve more interrupts from PIE group 3
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

void HVDMC_Protection(void)
{
     EALLOW;
      EPwm1Regs.TZSEL.bit.CBC6=0x1;
      EPwm2Regs.TZSEL.bit.CBC6=0x1;
      EPwm3Regs.TZSEL.bit.CBC6=0x1;

      EPwm1Regs.TZSEL.bit.OSHT1   = 1;  //enable TZ1 for OSHT
      EPwm2Regs.TZSEL.bit.OSHT1   = 1;  //enable TZ1 for OSHT
      EPwm3Regs.TZSEL.bit.OSHT1   = 1;  //enable TZ1 for OSHT


      EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
      EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low

      EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
      EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low

      EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
      EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low
      EDIS;
      EPwm1Regs.TZCLR.bit.OST = 1;
      EPwm2Regs.TZCLR.bit.OST = 1;
      EPwm3Regs.TZCLR.bit.OST = 1;
}

/******************************************************************************/
/**!                           END OF FILE                                    */
/******************************************************************************/
/*                   All rights reserved 2019 PHONGNH3289                  */
/******************************************************************************/
