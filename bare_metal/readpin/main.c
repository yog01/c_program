#include "stm32f407xx.h"

int main()
{
	uint32_t bit;
	RCC->AHB1ENR |=(1<<0);
	RCC->AHB1ENR |=(1<<3);
	GPIOD->MODER |=(1<<30);
	GPIOD->MODER &=~(0x1U<<31);
	while(1)
	{
		bit= GPIOA->IDR & 0x1;
			if(bit == 0x1)
				GPIOD->BSRR |=0x1U<<15;
			else
				GPIOD->BSRR |=0x1U<<31;
	}
}