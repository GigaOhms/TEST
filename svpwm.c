#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define pi 3.1415926535897932384626433832795f
#define F 50.0f
#define M 0.71
#define SQRT3	1.7320508075688772935274463415059f
#define TOP0 3599.0f

volatile double va, vb, vc;
volatile double t = 0.0;
volatile int16_t	phi, sect;
volatile double	T1, T2, T0;
volatile double	S1, S2, S3;
volatile uint16_t S11, S22, S33;


void SVPWM(void);
void SVWave(void);



int main(int argc, char const *argv[])
{
    SVWave();
    return 0;
}

void SVWave(void){
    printf("float va[201] = {");
    for (int i = 0; i < 200; i++){
        va = (sin(2.0*pi*F*t) + 1) / 2.0;
        vb = (sin(2.0*pi*F*t + 2.0*pi/3.0) + 1.0) / 2.0;
        vc = (sin(2.0*pi*F*t + 4.0*pi/3.0) + 1.0) / 2.0;

        SVPWM();

        S11 = round(((S1 - 0.5) * 1.0f + 0.5) * TOP0 + 0.5);
        S22 = round(((S2 - 0.5) * 1.0f + 0.5) * TOP0 + 0.5);
        S33 = round(((S3 - 0.5) * 1.0f + 0.5) * TOP0 + 0.5);
        printf("%d\n", S33);      // Updata M: (S3 - 0.5) * M + 0.5  
        t += 0.0001; // 0 <= t <= 0.02
    }
    printf("}\n");
}

void SVPWM(void){
    double	v_alpha	= 	sqrt(2.0 / 3.0) * (va - 0.5*vb - 0.5*vc);
    double	v_beta	=	sqrt(2.0 / 3.0) * SQRT3 / 2.0 * (vb - vc);
    double	theta	= 	atan2(v_beta, v_alpha) * 180/pi;

    if (theta > 0 && theta <= 60){
        phi	=	theta;
        sect	=	1;
    }
    else if (theta > 60 && theta <= 120){
        phi	=	theta - 60;
        sect	=	2;
    }
    else if (theta > 120 && theta <= 180){
        phi	=	theta - 120;
        sect	=	3;
    }
    else if (theta > -180 && theta <= -120){
        phi	=	theta + 180;
        sect	=	4;
    }
    else if (theta > -120 && theta <= -60){
        phi	=	theta + 120;
        sect	=	5;
    }
    else {
        phi	=	theta + 60;
        sect	=	6;
    }

    T1	=	1.0 * sin((60 - phi) * pi /180);        // 1.0 is M
    T2	=	1.0 * sin(phi * pi/180);                // 1.0 is M
    T0	=	1 - T1 - T2;

    switch (sect){
        case 1:
            S1	=	T1 + T2 + T0/2.0;
            S2	=	T2 + T0/2.0;
            S3	=	T0/2.0;
            break;

        case 2:
            S1	=	T1 + T0/2.0;
            S2	=	T1 + T2 + T0/2.0;
            S3	=	T0/2.0;
            break;

        case 3:
            S1	=	T0/2.0;
            S2	=	T1 + T2 + T0/2.0;
            S3	=	T2 + T0/2;
            break;

        case 4:
            S1	=	T0/2.0;
            S2	=	T1 + T0/2.0;
            S3	=	T1 + T2 + T0/2.0;
            break;

        case 5:
            S1	=	T2 + T0/2.0;
            S2	=	T0/2.0;
            S3	=	T1 + T2 + T0/2.0;
            break;

        case 6:
            S1	=	T1 + T2 + T0/2.0;
            S2	=	T0/2.0;
            S3	=	T1 + T0/2.0;
            break;
    }

}