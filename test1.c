#include <stdio.h>
#include <math.h>
#include <stdint.h>


// void show(int a[], int n){
//     for (int i = 0; i < n; i++)
//         printf("%d ", a[i]);
//     printf("\n");
// }

// --------------- Xoa phan tu 0 ----------------
// int main(){
//     int n, a[M];
//     scanf("%d", &n);
//     for (int i = 0; i < n; i++)
//         scanf("%d", &a[i]);
//     int j = 0;
//     for (int i = 0; i < n; i++){
//         while (a[j] == 0){
//             j++;
//             n--;
//         }
//         a[i] = a[j];
//         j++;
//     }
//     show(a, n);
//     return 0;
// }


// ------- Xoa phan tu trung -----------
// int main(){
//     int x, y, n, a[M];
//     scanf("%d", &n);
//     for (int i = 0; i < n; i++)
//         scanf("%d", &a[i]);
//     int j = 1;
//     for (int i = 0; i < n - 1; i++){
//         while (a[i] == a[j]){
//             j++;
//             n--;
//         }
//         a[i+1] = a[j];
//         j++;
//     }
//     show(a, n);
//     return 0;
// }

// ------------------------ Bitmask -------------


/*
    Nhập phần tử vào mảng
    In ra tất cả các số & số lần xuất hiện của số đó
    VD: int arr[7] = {1,4,6,4,4,6,1}
    1 xuat hien 2 lan
    4 xuat hien 3 lan
    6 xuat hien 2 lan
*/
/*
    pptr = &arr;
    *pptr = &element;
    **pptr = element;
*/


// void nhapMang(int *length, int **pptr){
//     do {
//         printf("Nhap so phan tu: ");
//         scanf("%d", length);
//     } while (*length < 1);

//     *pptr = (int *)malloc(*length * sizeof(int));
//     for (int i = 0; i < *length; i++){
//         printf("Nhap arr[%d]: ", i);
//         scanf("%d", (*pptr + i));
//     }
// }

// void xuatMang(int length, int arr[]){
//     printf("Mang la: ");
//     for (int i = 0; i < length; i++)
//         printf("\t%d", arr[i]);
// }
// // Hàm chèn phần tử vào mảng mẫu
// void increaseElement(int length, int arr[], int val){
//     for (int i = length; i > 0; i--)
//         arr[i] = arr[i - 1];
//     arr[0] = val;
// }

// void printNumber(int length, int arr[]){
//     int *exp = (int *)malloc(length * sizeof(int));
//     int change = 0;
//     int ct = 0;
//     // Tìm các gía trị có trong mảng & tạo ra mảng lưu giá trị đó
//     for (int i = 0; i < length; i++){
//         for (int j = 0; j < length; j++){
//             if (arr[i] == *(exp + j)){
//                 change = 0;
//                 break;
//             } else 
//                 change = 1;
//         }
//         if (change) {
//             increaseElement(length, exp, arr[i]);
//             change = 0;
//             ct++;
//         }
//     }
//     int length_new = (length - (length - ct));
//     exp = (int *)realloc(exp, sizeof(int) * length_new);
//     ct = 0;
//     //Đếm số lần xuất hiện của giá trị trong mảng
//     for (int i = 0; i < length_new; i++){
//         for (int j = 0; j < length; j++){
//             if (*(exp + i) == arr[j])
//                 ct++;
//         }
//         if (ct > 0)
//             printf("\n%d xuat hien %d lan", *(exp + i), ct);
//         ct = 0;
//     }
//     free(exp);
// }





#define pi 3.1415926535897932384626433832795
#define F 50.0
#define togiGain 1.0f
#define togiWTz 3.1415926535897932384626433832795e-2f // for 50Hz
#define M 1.0
#define SQRT3	1.7320508075688772935274463415059


void TOGI(float Vac, float *Vrms, float *Voffset)
{
    static float Vos = 0.0f;
    static float Vbe = 0.0f;
    static float Val = 0.0f;
    float eV, dVal, dVbe, dVos;

    eV = (Vac - Val)*togiGain;
    dVal = eV - Vbe;
    dVbe = Val;
    dVos = eV - Vos;

    Val += togiWTz*dVal;
    Vbe += togiWTz*dVbe;
    Vos += togiWTz*dVos;

    arm_sqrt_f32((Val*Val+(Vbe-Vos)*(Vbe-Vos))*0.5f, Vrms);
    *Voffset = Vos;
    return;
}

volatile float va, vb, vc;
volatile float t = 0.0;
volatile int	phi, sect;
volatile double	T1, T2, T0;
volatile double	S1, S2, S3;

void SVPWM(void);

void SVWave(void){
    printf("float va[201] = {");
    for (int i = 0; i < 200; i++){
        va = (sin(2.0*pi*F*t) + 1) / 2.0;
        vb = (sin(2.0*pi*F*t + 2.0*pi/3.0) + 1.0) / 2.0;
        vc = (sin(2.0*pi*F*t + 4.0*pi/3.0) + 1.0) / 2.0;
        SVPWM();
        printf("%.4f, ", S1);        
        t += 0.0001; // 0 <= t <= 0.02
    }
    printf("}\n");
}


int main(int argc, char const *argv[])
{
    SVWave();
    return 0;
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

    T1	=	M * sin((60 - phi) * pi /180);
    T2	=	M * sin(phi * pi/180);
    T0	=	1 - T1 - T2;

    switch (sect){
        case 1:
            S1	=	T1 + T2 + T0/2;
            S2	=	T2 + T0/2;
            S3	=	T0/2;
            break;

        case 2:
            S1	=	T1 + T0/2;
            S2	=	T1 + T2 + T0/2;
            S3	=	T0/2;
            break;

        case 3:
            S1	=	T0/2;
            S2	=	T1 + T2 + T0/2;
            S3	=	T2 + T0/2;
            break;

        case 4:
            S1	=	T0/2;
            S2	=	T1 + T0/2;
            S3	=	T1 + T2 + T0/2;
            break;

        case 5:
            S1	=	T2 + T0/2;
            S2	=	T0/2;
            S3	=	T1 + T2 + T0/2;
            break;

        case 6:
            S1	=	T1 + T2 + T0/2;
            S2	=	T0/2;
            S3	=	T1 + T0/2;
            break;
    }

}