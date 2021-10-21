#include "HAL_GPIO.h"

static void conf_pin_mode(GPIO_TYPE port, uint32_t pinNum, uint32_t mode)
{
	if(mode == GPIO_MODE_INPUT)
	{
		if(port.Init == PORTA)
		{
					PORTA->MODER &= ~((uint32_t)0x11<<pinNum*2);
		}
		else if(port.Init == PORTB)
		{
					PORTB->MODER &= ~((uint32_t)0x11<<pinNum*2);
		}
		else if(port.Init == PORTC)
		{
					PORTC->MODER &= ~((uint32_t)0x11<<pinNum*2);
		}
		else if(port.Init == PORTD)
		{
					PORTD->MODER &= ~((uint32_t)0x11<<pinNum*2);
		}
	}
	else
	{
		if(port.Init == PORTA)
		{
					PORTA->MODER |= port.mode<<pinNum*2;
		}
		else if(port.Init == PORTB)
		{
			PORTB->MODER |= port.mode<<pinNum*2;
		}
		else if(port.Init == PORTC)
		{
					PORTC->MODER |= port.mode<<pinNum*2;
		}
		else if(port.Init == PORTD)
		{
					PORTD->MODER |= port.mode<<pinNum*2;
		}
	}
}

//configure type
static void conf_pin_type(GPIO_TYPE port, uint32_t pinNum, uint32_t type)
{
	if(type == GPIO_OUTPUT_TYPE_RE)
	{
		port.Init->OTYPER &= ~(type<<pinNum);
	}
	else
	{
		port.Init->OTYPER |= (type<<pinNum);
	}
}
//Speed
static void conf_pin_speed(GPIO_TYPE port, uint32_t pinNum, uint32_t speed)
{
	if(speed == GPIO_OUTPUT_SPEED_LOW)
	{
		if(port.Init == PORTA)
		{
					PORTA->OSPEEDR &= ~((uint32_t)0x11<<pinNum*2);
		}
		else if(port.Init == PORTB)
		{
					PORTB->OSPEEDR &= ~((uint32_t)0x11<<pinNum*2);
		}
		else if(port.Init == PORTC)
		{
					PORTC->OSPEEDR &= ~((uint32_t)0x11<<pinNum*2);
		}
		else if(port.Init == PORTD)
		{
					PORTD->OSPEEDR &= ~((uint32_t)0x11<<pinNum*2);
		}
	}
	else
	{
		if(port.Init == PORTA)
		{
					PORTA->OSPEEDR |= port.speed<<pinNum*2;
		}
		else if(port.Init == PORTB)
		{
			PORTB->OSPEEDR |= port.speed<<pinNum*2;
		}
		else if(port.Init == PORTC)
		{
					PORTC->OSPEEDR |= port.speed<<pinNum*2;
		}
		else if(port.Init == PORTD)
		{
					PORTD->OSPEEDR |= port.speed<<pinNum*2;
		}
	}
}

void gpio_Init(GPIO_TYPE port)
{
	if(port.Init == PORTA)
	{
		RCC_AHB1_CLK_GPIOAEN;
	}else if(port.Init == PORTB)
	{
		RCC_AHB1_CLK_GPIOBEN;
	}else if(port.Init == PORTC)
	{
		RCC_AHB1_CLK_GPIOCEN;
	}else if(port.Init == PORTD)
	{
		RCC_AHB1_CLK_GPIODEN;
	}
	conf_pin_mode(port,port.pin,port.mode);
	conf_pin_type(port,port.pin, port.type);
	conf_pin_speed(port,port.pin, port.speed);
}