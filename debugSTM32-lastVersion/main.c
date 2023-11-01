/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <math.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TOP0        3599.0
//#define TOP0        449.0
#define F           50.0
#define SQRT3	    1.7320508075688772935274463415059
#define GAIN		3.325581		// Cau phan ap 10k // 4.3k
#define OFFSET		0.37			// Sut ap cau chinh luu
#define VDC        	42.42			// Nguon VDC nghich luu
#define ONE_DIV_VDC 0.0235737 		// 1/VDC
#define PU          0.000244140625
#define ONE_DIV_200	0.0050
#define UMAX		1.0
#define UMIN		0.05
#define VMIN		0.75
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */
volatile uint16_t i = 0, analogA5 = 0;

volatile float setupFrequence = 50.0f;	// Hz
volatile float setupTime = 20.0f;		// s: 1 -> setupFrequence (Hz) in setupTime (s)
volatile uint16_t periodMin;
volatile uint32_t periodNow = 65535;
volatile uint32_t pscNow = 65535;
volatile uint16_t delayTime;
volatile float frequenceNow = 0.0001f;

//volatile uint16_t S1, S2, S3;
volatile uint8_t STT;
volatile double t = 0.0, m = 1.0f, VAVG = 1000.0, sumAnalog = 0.0, VREF = 2.60, VREAL = 0.0;

// array 200 element
//float va[201] = {0.5000, 0.5302, 0.5604, 0.5905, 0.6205, 0.6355, 0.6652, 0.6948, 0.7241, 0.7532, 0.7676, 0.7962, 0.8244, 0.8522, 0.8796, 0.8932, 0.9199, 0.9373, 0.9455, 0.9532, 0.9568, 0.9636, 0.9698, 0.9755, 0.9806, 0.9830, 0.9872, 0.9908, 0.9938, 0.9963, 0.9981, 0.9988, 0.9997, 1.0000, 0.9997, 0.9988, 0.9981, 0.9963, 0.9938, 0.9908, 0.9872, 0.9851, 0.9806, 0.9755, 0.9698, 0.9668, 0.9603, 0.9532, 0.9455, 0.9373, 0.9330, 0.9415, 0.9494, 0.9568, 0.9636, 0.9668, 0.9728, 0.9782, 0.9830, 0.9872, 0.9891, 0.9924, 0.9951, 0.9973, 0.9988, 0.9993, 0.9999, 0.9999, 0.9993, 0.9981, 0.9973, 0.9951, 0.9924, 0.9891, 0.9851, 0.9830, 0.9782, 0.9728, 0.9668, 0.9603, 0.9568, 0.9494, 0.9415, 0.9330, 0.9066, 0.8932, 0.8660, 0.8384, 0.8104, 0.7820, 0.7676, 0.7387, 0.7095, 0.6801, 0.6504, 0.6355, 0.6055, 0.5755, 0.5453, 0.5151, 0.5000, 0.4698, 0.4396, 0.4095, 0.3795, 0.3645, 0.3348, 0.3052, 0.2759, 0.2468, 0.2324, 0.2038, 0.1756, 0.1478, 0.1204, 0.1068, 0.0801, 0.0627, 0.0545, 0.0468, 0.0432, 0.0364, 0.0302, 0.0245, 0.0194, 0.0170, 0.0128, 0.0092, 0.0062, 0.0037, 0.0027, 0.0012, 0.0003, 0.0000, 0.0003, 0.0007, 0.0019, 0.0037, 0.0062, 0.0092, 0.0109, 0.0149, 0.0194, 0.0245, 0.0302, 0.0332, 0.0397, 0.0468, 0.0545, 0.0627, 0.0670, 0.0585, 0.0506, 0.0432, 0.0364, 0.0332, 0.0272, 0.0218, 0.0170, 0.0128, 0.0109, 0.0076, 0.0049, 0.0027, 0.0012, 0.0007, 0.0001, 0.0001, 0.0007, 0.0019, 0.0027, 0.0049, 0.0076, 0.0109, 0.0149, 0.0170, 0.0218, 0.0272, 0.0332, 0.0397, 0.0432, 0.0506, 0.0585, 0.0670, 0.0934, 0.1068, 0.1340, 0.1616, 0.1896, 0.2180, 0.2324, 0.2613, 0.2905, 0.3199, 0.3496, 0.3645, 0.3945, 0.4245, 0.4547, 0.4849};
//float vb[201] = {1.0000, 0.9997, 0.9988, 0.9973, 0.9951, 0.9938, 0.9908, 0.9872, 0.9830, 0.9782, 0.9755, 0.9698, 0.9636, 0.9568, 0.9494, 0.9455, 0.9373, 0.9199, 0.8932, 0.8660, 0.8522, 0.8244, 0.7962, 0.7676, 0.7387, 0.7241, 0.6948, 0.6652, 0.6355, 0.6055, 0.5755, 0.5604, 0.5302, 0.5000, 0.4698, 0.4396, 0.4245, 0.3945, 0.3645, 0.3348, 0.3052, 0.2905, 0.2613, 0.2324, 0.2038, 0.1896, 0.1616, 0.1340, 0.1068, 0.0801, 0.0670, 0.0585, 0.0506, 0.0432, 0.0364, 0.0332, 0.0272, 0.0218, 0.0170, 0.0128, 0.0109, 0.0076, 0.0049, 0.0027, 0.0012, 0.0007, 0.0001, 0.0001, 0.0007, 0.0019, 0.0027, 0.0049, 0.0076, 0.0109, 0.0149, 0.0170, 0.0218, 0.0272, 0.0332, 0.0397, 0.0432, 0.0506, 0.0585, 0.0670, 0.0585, 0.0545, 0.0468, 0.0397, 0.0332, 0.0272, 0.0245, 0.0194, 0.0149, 0.0109, 0.0076, 0.0062, 0.0037, 0.0019, 0.0007, 0.0001, 0.0000, 0.0003, 0.0012, 0.0027, 0.0049, 0.0062, 0.0092, 0.0128, 0.0170, 0.0218, 0.0245, 0.0302, 0.0364, 0.0432, 0.0506, 0.0545, 0.0627, 0.0801, 0.1068, 0.1340, 0.1478, 0.1756, 0.2038, 0.2324, 0.2613, 0.2759, 0.3052, 0.3348, 0.3645, 0.3945, 0.4095, 0.4396, 0.4698, 0.5000, 0.5302, 0.5453, 0.5755, 0.6055, 0.6355, 0.6652, 0.6801, 0.7095, 0.7387, 0.7676, 0.7962, 0.8104, 0.8384, 0.8660, 0.8932, 0.9199, 0.9330, 0.9415, 0.9494, 0.9568, 0.9636, 0.9668, 0.9728, 0.9782, 0.9830, 0.9872, 0.9891, 0.9924, 0.9951, 0.9973, 0.9988, 0.9993, 0.9999, 0.9999, 0.9993, 0.9981, 0.9973, 0.9951, 0.9924, 0.9891, 0.9851, 0.9830, 0.9782, 0.9728, 0.9668, 0.9603, 0.9568, 0.9494, 0.9415, 0.9330, 0.9415, 0.9455, 0.9532, 0.9603, 0.9668, 0.9728, 0.9755, 0.9806, 0.9851, 0.9891, 0.9924, 0.9938, 0.9963, 0.9981, 0.9993, 0.9999};
//float vc[201] = {0.0000, 0.0003, 0.0012, 0.0027, 0.0049, 0.0062, 0.0092, 0.0128, 0.0170, 0.0218, 0.0245, 0.0302, 0.0364, 0.0432, 0.0506, 0.0545, 0.0627, 0.0627, 0.0545, 0.0468, 0.0432, 0.0364, 0.0302, 0.0245, 0.0194, 0.0170, 0.0128, 0.0092, 0.0062, 0.0037, 0.0019, 0.0012, 0.0003, 0.0000, 0.0003, 0.0012, 0.0019, 0.0037, 0.0062, 0.0092, 0.0128, 0.0149, 0.0194, 0.0245, 0.0302, 0.0332, 0.0397, 0.0468, 0.0545, 0.0627, 0.0670, 0.0934, 0.1204, 0.1478, 0.1756, 0.1896, 0.2180, 0.2468, 0.2759, 0.3052, 0.3199, 0.3496, 0.3795, 0.4095, 0.4396, 0.4547, 0.4849, 0.5151, 0.5453, 0.5755, 0.5905, 0.6205, 0.6504, 0.6801, 0.7095, 0.7241, 0.7532, 0.7820, 0.8104, 0.8384, 0.8522, 0.8796, 0.9066, 0.9330, 0.9415, 0.9455, 0.9532, 0.9603, 0.9668, 0.9728, 0.9755, 0.9806, 0.9851, 0.9891, 0.9924, 0.9938, 0.9963, 0.9981, 0.9993, 0.9999, 1.0000, 0.9997, 0.9988, 0.9973, 0.9951, 0.9938, 0.9908, 0.9872, 0.9830, 0.9782, 0.9755, 0.9698, 0.9636, 0.9568, 0.9494, 0.9455, 0.9373, 0.9373, 0.9455, 0.9532, 0.9568, 0.9636, 0.9698, 0.9755, 0.9806, 0.9830, 0.9872, 0.9908, 0.9938, 0.9963, 0.9973, 0.9988, 0.9997, 1.0000, 0.9997, 0.9993, 0.9981, 0.9963, 0.9938, 0.9908, 0.9891, 0.9851, 0.9806, 0.9755, 0.9698, 0.9668, 0.9603, 0.9532, 0.9455, 0.9373, 0.9330, 0.9066, 0.8796, 0.8522, 0.8244, 0.8104, 0.7820, 0.7532, 0.7241, 0.6948, 0.6801, 0.6504, 0.6205, 0.5905, 0.5604, 0.5453, 0.5151, 0.4849, 0.4547, 0.4245, 0.4095, 0.3795, 0.3496, 0.3199, 0.2905, 0.2759, 0.2468, 0.2180, 0.1896, 0.1616, 0.1478, 0.1204, 0.0934, 0.0670, 0.0585, 0.0545, 0.0468, 0.0397, 0.0332, 0.0272, 0.0245, 0.0194, 0.0149, 0.0109, 0.0076, 0.0062, 0.0037, 0.0019, 0.0007, 0.0001};
// array 100 element
// float va[101] = {0.5000, 0.5604, 0.6205, 0.6652, 0.7241, 0.7676, 0.8244, 0.8796, 0.9199, 0.9455, 0.9568, 0.9698, 0.9806, 0.9872, 0.9938, 0.9973, 0.9997, 0.9997, 0.9981, 0.9938, 0.9891, 0.9806, 0.9698, 0.9603, 0.9455, 0.9330, 0.9494, 0.9636, 0.9728, 0.9830, 0.9908, 0.9951, 0.9988, 0.9999, 0.9993, 0.9963, 0.9924, 0.9851, 0.9782, 0.9668, 0.9532, 0.9415, 0.9066, 0.8660, 0.8104, 0.7676, 0.7095, 0.6504, 0.6055, 0.5453, 0.5000, 0.4396, 0.3795, 0.3348, 0.2759, 0.2324, 0.1756, 0.1204, 0.0801, 0.0545, 0.0432, 0.0302, 0.0194, 0.0128, 0.0062, 0.0027, 0.0003, 0.0003, 0.0019, 0.0062, 0.0109, 0.0194, 0.0302, 0.0397, 0.0545, 0.0670, 0.0506, 0.0364, 0.0272, 0.0170, 0.0109, 0.0049, 0.0012, 0.0001, 0.0007, 0.0027, 0.0076, 0.0149, 0.0218, 0.0332, 0.0432, 0.0585, 0.0934, 0.1340, 0.1896, 0.2324, 0.2905, 0.3496, 0.3945, 0.4547};
// float vb[101] = {1.0000, 0.9988, 0.9951, 0.9908, 0.9830, 0.9755, 0.9636, 0.9494, 0.9373, 0.8932, 0.8522, 0.7962, 0.7387, 0.6948, 0.6355, 0.5905, 0.5302, 0.4698, 0.4245, 0.3645, 0.3199, 0.2613, 0.2038, 0.1616, 0.1068, 0.0670, 0.0506, 0.0364, 0.0272, 0.0170, 0.0092, 0.0049, 0.0012, 0.0001, 0.0007, 0.0037, 0.0076, 0.0149, 0.0218, 0.0332, 0.0468, 0.0585, 0.0585, 0.0468, 0.0332, 0.0245, 0.0149, 0.0076, 0.0037, 0.0007, 0.0000, 0.0012, 0.0049, 0.0092, 0.0170, 0.0245, 0.0364, 0.0506, 0.0627, 0.1068, 0.1478, 0.2038, 0.2613, 0.3052, 0.3645, 0.4095, 0.4698, 0.5302, 0.5755, 0.6355, 0.6801, 0.7387, 0.7962, 0.8384, 0.8932, 0.9330, 0.9494, 0.9636, 0.9728, 0.9830, 0.9891, 0.9951, 0.9988, 0.9999, 0.9993, 0.9973, 0.9924, 0.9851, 0.9782, 0.9668, 0.9568, 0.9415, 0.9415, 0.9532, 0.9668, 0.9755, 0.9851, 0.9924, 0.9963, 0.9993};
// float vc[101] = {0.0000, 0.0012, 0.0049, 0.0092, 0.0170, 0.0245, 0.0364, 0.0506, 0.0627, 0.0545, 0.0432, 0.0302, 0.0194, 0.0128, 0.0062, 0.0027, 0.0003, 0.0003, 0.0019, 0.0062, 0.0109, 0.0194, 0.0302, 0.0397, 0.0545, 0.0670, 0.1204, 0.1756, 0.2180, 0.2759, 0.3348, 0.3795, 0.4396, 0.4849, 0.5453, 0.6055, 0.6504, 0.7095, 0.7532, 0.8104, 0.8660, 0.9066, 0.9415, 0.9532, 0.9668, 0.9755, 0.9851, 0.9924, 0.9963, 0.9993, 1.0000, 0.9988, 0.9951, 0.9908, 0.9830, 0.9755, 0.9636, 0.9494, 0.9373, 0.9455, 0.9568, 0.9698, 0.9806, 0.9872, 0.9938, 0.9973, 0.9997, 0.9997, 0.9981, 0.9938, 0.9891, 0.9806, 0.9698, 0.9603, 0.9455, 0.9330, 0.8796, 0.8244, 0.7820, 0.7241, 0.6801, 0.6205, 0.5604, 0.5151, 0.4547, 0.4095, 0.3496, 0.2905, 0.2468, 0.1896, 0.1478, 0.0934, 0.0585, 0.0468, 0.0332, 0.0245, 0.0149, 0.0076, 0.0037, 0.0007};

volatile uint16_t S1[201] = {1854, 1909, 2017, 2126, 2234, 2341, 2395, 2501, 2607, 2711, 2815, 2866, 2968, 3068, 3166, 3215, 3311, 3374, 3403, 3431, 3444, 3468, 3491, 3511, 3530, 3538, 3553, 3566, 3577, 3586, 3593, 3595, 3598, 3600, 3598, 3595, 3593, 3586, 3577, 3566, 3553, 3546, 3530, 3511, 3491, 3468, 3456, 3431, 3403, 3374, 3358, 3389, 3417, 3444, 3468, 3480, 3501, 3521, 3538, 3553, 3560, 3572, 3582, 3590, 3595, 3597, 3599, 3599, 3597, 3593, 3590, 3582, 3572, 3560, 3546, 3538, 3521, 3501, 3480, 3456, 3444, 3417, 3389, 3358, 3263, 3215, 3117, 3018, 2917, 2815, 2763, 2659, 2554, 2448, 2341, 2288, 2180, 2072, 1963, 1854, 1800, 1691, 1583, 1474, 1366, 1312, 1205, 1099, 993, 889, 837, 734, 632, 532, 434, 385, 289, 226, 197, 169, 156, 132, 109, 89, 70, 62, 47, 34, 23, 14, 10, 5, 2, 0, 2, 3, 7, 14, 23, 34, 40, 54, 70, 89, 109, 120, 144, 169, 197, 226, 242, 211, 183, 156, 132, 120, 99, 79, 62, 47, 40, 28, 18, 10, 5, 3, 1, 1, 3, 7, 10, 18, 28, 40, 54, 62, 79, 99, 120, 144, 156, 183, 211, 242, 337, 385, 483, 582, 683, 785, 837, 941, 1046, 1152, 1259, 1312, 1420, 1528, 1637, 1746};

//volatile uint16_t S1[201] = {1844, 1887, 1974, 2061, 2147, 2233, 2276, 2361, 2445, 2529, 2612, 2653, 2734, 2814, 2893, 2932, 3009, 3059, 3083, 3105, 3115, 3135, 3153, 3169, 3184, 3191, 3203, 3213, 3222, 3229, 3234, 3236, 3239, 3240, 3239, 3236, 3234, 3229, 3222, 3213, 3203, 3197, 3184, 3169, 3153, 3135, 3125, 3105, 3083, 3059, 3047, 3071, 3094, 3115, 3135, 3144, 3161, 3177, 3191, 3203, 3208, 3218, 3226, 3232, 3236, 3238, 3239, 3239, 3238, 3234, 3232, 3226, 3218, 3208, 3197, 3191, 3177, 3161, 3144, 3125, 3115, 3094, 3071, 3047, 2971, 2932, 2854, 2774, 2694, 2612, 2571, 2487, 2403, 2318, 2233, 2190, 2104, 2017, 1930, 1844, 1800, 1713, 1626, 1539, 1453, 1410, 1324, 1239, 1155, 1071, 1029, 947, 866, 786, 707, 668, 591, 541, 517, 495, 485, 465, 447, 431, 416, 409, 397, 387, 378, 371, 368, 364, 361, 360, 361, 362, 366, 371, 378, 387, 392, 403, 416, 431, 447, 456, 475, 495, 517, 541, 553, 529, 506, 485, 465, 456, 439, 423, 409, 397, 392, 382, 374, 368, 364, 362, 361, 361, 362, 366, 368, 374, 382, 392, 403, 409, 423, 439, 456, 475, 485, 506, 529, 553, 629, 668, 746, 826, 906, 988, 1029, 1113, 1197, 1282, 1367, 1410, 1496, 1583, 1670, 1756};
volatile uint16_t S2[201] = {3599, 3598, 3595, 3590, 3582, 3572, 3566, 3553, 3538, 3521, 3501, 3491, 3468, 3444, 3417, 3403, 3374, 3311, 3215, 3117, 3068, 2968, 2866, 2763, 2659, 2607, 2501, 2395, 2288, 2180, 2072, 2017, 1909, 1800, 1691, 1583, 1528, 1420, 1312, 1205, 1099, 1046, 941, 837, 734, 632, 582, 483, 385, 289, 242, 211, 183, 156, 132, 120, 99, 79, 62, 47, 40, 28, 18, 10, 5, 3, 1, 1, 3, 7, 10, 18, 28, 40, 54, 62, 79, 99, 120, 144, 156, 183, 211, 242, 211, 197, 169, 144, 120, 99, 89, 70, 54, 40, 28, 23, 14, 7, 3, 1, 0, 2, 5, 10, 18, 23, 34, 47, 62, 79, 89, 109, 132, 156, 183, 197, 226, 289, 385, 483, 532, 632, 734, 837, 941, 993, 1099, 1205, 1312, 1420, 1474, 1583, 1691, 1800, 1909, 1963, 2072, 2180, 2288, 2395, 2448, 2554, 2659, 2763, 2866, 2917, 3018, 3117, 3215, 3311, 3358, 3389, 3417, 3444, 3468, 3480, 3501, 3521, 3538, 3553, 3560, 3572, 3582, 3590, 3595, 3597, 3599, 3599, 3597, 3593, 3590, 3582, 3572, 3560, 3546, 3538, 3521, 3501, 3480, 3456, 3444, 3417, 3389, 3358, 3389, 3403, 3431, 3456, 3480, 3501, 3511, 3530, 3546, 3560, 3572, 3577, 3586, 3593, 3597, 3599};
volatile uint16_t S3[201] = {1, 2, 5, 10, 18, 28, 34, 47, 62, 79, 99, 109, 132, 156, 183, 197, 226, 226, 197, 169, 156, 132, 109, 89, 70, 62, 47, 34, 23, 14, 7, 5, 2, 0, 2, 5, 7, 14, 23, 34, 47, 54, 70, 89, 109, 132, 144, 169, 197, 226, 242, 337, 434, 532, 632, 683, 785, 889, 993, 1099, 1152, 1259, 1366, 1474, 1583, 1637, 1746, 1854, 1963, 2072, 2126, 2234, 2341, 2448, 2554, 2607, 2711, 2815, 2917, 3018, 3068, 3166, 3263, 3358, 3389, 3403, 3431, 3456, 3480, 3501, 3511, 3530, 3546, 3560, 3572, 3577, 3586, 3593, 3597, 3599, 3600, 3598, 3595, 3590, 3582, 3577, 3566, 3553, 3538, 3521, 3511, 3491, 3468, 3444, 3417, 3403, 3374, 3374, 3403, 3431, 3444, 3468, 3491, 3511, 3530, 3538, 3553, 3566, 3577, 3586, 3590, 3595, 3598, 3600, 3598, 3597, 3593, 3586, 3577, 3566, 3560, 3546, 3530, 3511, 3491, 3480, 3456, 3431, 3403, 3374, 3358, 3263, 3166, 3068, 2968, 2917, 2815, 2711, 2607, 2501, 2448, 2341, 2234, 2126, 2017, 1963, 1854, 1746, 1637, 1528, 1474, 1366, 1259, 1152, 1046, 993, 889, 785, 683, 582, 532, 434, 337, 242, 211, 197, 169, 144, 120, 99, 89, 70, 54, 40, 28, 23, 14, 7, 3, 1};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void delay_us (uint16_t us);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_Base_Start(&htim4);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  HAL_ADC_Start(&hadc1);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0x0);

  periodMin = (uint16_t)(72000000.0 / (200.0 * setupFrequence)) - 1;
  delayTime = (uint16_t)(0.96 * setupTime * 1000000.0f / setupFrequence / 100);
//  frequenceNow = 360000.0 / (periodNow + 1); // 360000 = 72000000/200

  TIM1->ARR = periodNow;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//    analogA5 = HAL_ADC_GetValue(&hadc1);
//    HAL_ADC_Stop(&hadc1);
//	VREF = (VAVG + 93.286) * 0.00081606;
//	VREAL = (VREF + OFFSET);
//	VREAL = VREAL <= VMIN ? 0 : VREAL;
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (frequenceNow <= setupFrequence){
		  frequenceNow = frequenceNow + 0.01;
		  if (frequenceNow < 5.50f){
			  TIM1->PSC = (uint32_t)(5.493164063f / (frequenceNow)) - 1ULL; // 5.493164063f = 72000000/200/(65535+1)
//			  TIM1->PSC = pscNow;
		  } else {
			  TIM1->ARR = (uint32_t)(360000.0 / (frequenceNow)) - 1ULL;
//			  TIM1->ARR = periodNow;
		  }
//		  frequenceNow = 360000.0 / (periodNow + 1);
	  }
	  delay_us(delayTime);
//	  htim1.Init.Period = periodNow;



  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 7199;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim2.Init.Period = 3599;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 1000;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 1500;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 71;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//	m = VREAL * GAIN * SQRT3 * ONE_DIV_VDC * 2.0;
//	//m = VREF * 0.3030303030;
//    m = m > UMAX ? UMAX : m;
//    m = m < UMIN ? UMIN : m;

//    S1 = ((va[i] - 0.5) * m + 0.5) * TOP0 + 0.5;           // PWM Pin A0
//    S2 = ((vb[i] - 0.5) * m + 0.5) * TOP0 + 0.5;           // PWM Pin A1
//    S3 = ((vc[i] - 0.5) * m + 0.5) * TOP0 + 0.5;           // PWM Pin A2

		TIM2->CCR1 = S1[i];
		TIM2->CCR2 = S2[i];
		TIM2->CCR3 = S3[i];

//	TIM2->CCR1 = S1;
//    TIM2->CCR2 = S2;
//    TIM2->CCR3 = S3;
        // TIM2->CNT = ;

//	sumAnalog = sumAnalog + analogA5;
//	i = (i == 199) ? 0 : i + 1;
        i++;
    if (i >= 200){
        i = 0;
//		VAVG = sumAnalog * ONE_DIV_200;
//		sumAnalog = 0.0;
		STT = ~STT;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, STT);


    }
}

void delay_us (uint16_t us)
{
	TIM4->CNT = 0;
	while (TIM4->CNT < us);  // wait
//	__HAL_TIM_SET_COUNTER(&htim1,0);  // set the counter value a 0
//	while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // wait for the counter to reach the us input in the parameter
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
