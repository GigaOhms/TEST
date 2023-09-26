#include "stm32f10x.h"

void GPIO_Config(void);
void Delay(__IO uint32_t nCount);

int main(void){
	GPIO_Config();
	
	while(1){
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		Delay(1000000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		Delay(1000000);
	}
}

void GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 	// Enable clock port C (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		// Mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				// Select Pin (GPIO_Pin_13 | GPIO_Pin_1)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// Select speed Pin
	GPIO_Init(GPIOC, &GPIO_InitStructure);					// Init cac thong so da khai bao
}

void Delay(__IO uint32_t nCount){
	while (nCount--){}
}
