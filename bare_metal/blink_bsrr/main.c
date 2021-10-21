#include "stm32f407xx.h"

int main()
{
	RCC->AHB1ENR |= 0x1<<3;
	//RCC->AHB1ENR |= 0x08;
	GPIOD->MODER |= 0x1<<30;
	
	while(1)
	{
		GPIOD->BSRR = 0x8000;
		for(int i=0; i<10000000; i++);
		GPIOD->BSRR = 0x80000000;
		for(int i=0; i<10000000; i++);
	}
	
	
}