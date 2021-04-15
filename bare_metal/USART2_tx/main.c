#include "stm32f407xx.h"

void USART2_Init(void);
void USART2_tx(char c);
void delay(int d);
int main()
{
	USART2_Init();
		while(1)
		{
			USART2_tx('a');
			delay(1000);
			USART2_tx('o');
			delay(1000);
		}
	return 0;
}
void USART2_tx(char c)
{
	while(!(USART2->SR & 0x0080));
	USART2->DR = (c & 0xFF);
}
void USART2_Init(void)
{
	//Enable APB1 for usart2
	RCC->APB1ENR |= 0x1<<17;
	//Enable Poart A clk
	RCC->AHB1ENR |= 0x1<<0;
	
	//Mode for alternate function PA2
	GPIOA->MODER &=~(0x1U<<4);
	GPIOA->MODER |=(0x1<<5);
	
	//alternate function for PA2
	GPIOA->AFR[0] |= (0x7<<8); 
	
	//USART2 Baudrate
	USART2->BRR = 0x0683; //9600
	USART2->CR1 |= 0x1<<3;
	USART2->CR1 |= 0x1<<13;
}

void delay(int d)
{
	int i=0;
	for(;d>0;d--)
	{
		for(;i<3915;i++);
	}
}