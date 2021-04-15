#include "HAL_GPIO.h"
#include <stdbool.h>
void delay(volatile uint32_t delay);
bool exti_flag=false;
int main()
{


	GPIO_TYPE gpio_exti, gpio_led;
	EXTI_TYPE exti_s;
	
	gpio_led.Init = PORTD;
	gpio_led.mode = GPIO_MODE_OUTPUT;
	gpio_led.pin = 15;
	gpio_led.pull = GPIO_PULL_DOWN;
	gpio_led.speed = GPIO_OUTPUT_SPEED_MED;
	gpio_led.type = GPIO_OUTPUT_TYPE_PP_RE;
	gpio_Init(gpio_led);
	
	gpio_exti.Init = PORTA;
	gpio_exti.mode = GPIO_MODE_AFIO;
	gpio_exti.pin = 0;
	gpio_exti.speed = GPIO_OUTPUT_SPEED_MED;
	gpio_exti.type = GPIO_OUTPUT_TYPE_PP_RE;
	gpio_exti.pull = GPIO_PULL_DOWN;
	gpio_Init(gpio_exti);
	exti_s.exti_val = EXTI_PORTA_PINS;
	conf_external_interupt(gpio_exti,0,0,exti_s); //configure register
	conf_edge(0,RISING_FALLING);
	enable_IRQ(0,EXTI0_IRQn);
	//conf_external_sw_interupt(0);
	while(1)
	{
		//GPIOD->ODR ^= 0x1<<15;
		//for(i=0;i<20000;i++);
		//GPIOD->BSRR = (uint32_t)0x1<<15;
		//delay(1000000);
		//GPIOD->BSRR = (uint32_t)0x1<<(15+16);
		//delay(1000000);
		delay(10000);
		if(true==exti_flag)
		{
			exti_flag = false;
			set_gpio_pin(15);
			delay(10000);
		}
		else
		{
			reset_gpio_pin(15);
			delay(10);
		}
	}
	return 0;
}
void EXTI0_IRQHandler(void)
{
	clear_interupt_irq(0);
	exti_flag=true;
}
void delay(volatile uint32_t delay)
{
	int i=0;
	for(i=0;i<(int)delay;i++);
}