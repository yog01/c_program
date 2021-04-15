#include "HAL_GPIO.h"
void delay(uint32_t delay);
int main()
{

	GPIO_TYPE gpio_s;
	
	gpio_s.Init = PORTD;
	gpio_s.mode = GPIO_MODE_OUTPUT;
	gpio_s.pin = 15;
	gpio_s.speed = GPIO_OUTPUT_SPEED_MED;
	gpio_s.type = GPIO_OUTPUT_TYPE_RE;
	gpio_Init(gpio_s);
	while(1)
	{
		//GPIOD->ODR ^= 0x1<<15;
		//for(i=0;i<20000;i++);
		GPIOD->BSRR = (uint32_t)0x1<<15;
		delay(1000000);
		GPIOD->BSRR = (uint32_t)0x1<<(15+16);
		delay(1000000);
	}
	
	return 0;
}
void delay(uint32_t delay)
{
	int i=0;
	for(i=0;i<(int)delay;i++);
}