#include "stm32f407xx.h"

void USART1_init(void);
void led_play(uint32_t temp);
uint32_t USART1_rec(void);
void delay(int x);
int main()
{
	uint32_t c=0;
	RCC->AHB1ENR |= 0x1<<3; //enable portd 15 led
	GPIOD->MODER |= 0x1<<30; //enable output mode
	GPIOD->MODER &=~(0x1U<<31);
	USART1_init();
	
	while(1)
	{
		c=USART1_rec();
		delay(1000);
		led_play(c);
	}
}

void USART1_init(void)
{
	//configure clock for USART and its portA
	RCC->AHB1ENR |= 0x1<<0;
	RCC->APB2ENR |= 0x1<<4;
	
	//port pin PA10 mode  and alternate function
	GPIOA->AFR[1] = 0x7<<8;
	GPIOA->MODER &= ~(0x1U<<20);
	GPIOA->MODER |= (0x1<<21);
	
	//usart brr, cr 
	USART1->BRR = 0x008B; // 115200 baudrate
	USART1->CR1 = 0x1<<13; //unable uart
	USART1->CR1 |= 0x1<<2;  //unable receive
	
}
uint32_t USART1_rec(void)
{
	while(!(USART1->SR & 0x0020)){}
	return USART1->DR;
}
void led_play(uint32_t temp)
{
	temp %=16;
	for(;temp>0;temp--)
	{
	GPIOD->BSRR = 0x1<<15;
		delay(1000);
	GPIOD->BSRR = 0x1U<<31;
		delay(1000);
	}		
}

void delay(int x)
{
	int i=0;
	
	for(;x>0;x--)
	{
		for(;i<3195;i++);
	}
}