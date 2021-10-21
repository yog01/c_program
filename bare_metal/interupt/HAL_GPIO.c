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
				PORTA->MODER |= mode<<(pinNum*2);
		}
		else if(port.Init == PORTB)
		{
			PORTB->MODER |= mode<<pinNum*2;
		}
		else if(port.Init == PORTC)
		{
					PORTC->MODER |= mode<<pinNum*2;
		}
		else if(port.Init == PORTD)
		{
					PORTD->MODER |= mode<<pinNum*2;
		}
	}
}

//configure type
static void conf_pin_type(GPIO_TYPE port, uint32_t pinNum, uint32_t type)
{
	if(type == GPIO_OUTPUT_TYPE_PP_RE)
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
					PORTA->OSPEEDR |= speed<<pinNum*2;
		}
		else if(port.Init == PORTB)
		{
			PORTB->OSPEEDR |= speed<<pinNum*2;
		}
		else if(port.Init == PORTC)
		{
					PORTC->OSPEEDR |= speed<<pinNum*2;
		}
		else if(port.Init == PORTD)
		{
					PORTD->OSPEEDR |= speed<<pinNum*2;
		}
	}
}
//pullup or pulldown
static void conf_pin_pull(GPIO_TYPE port, uint32_t pinNum, uint32_t pull)
{
	if(pull == GPIO_NO_PULL_UPDOWN)
	{
		if(port.Init == PORTA)
		{
			PORTA->PUPDR &=~(pull<<pinNum*2);
		}else if(port.Init == PORTB)
		{
			PORTB->PUPDR &=~(pull<<pinNum*2);
		}else if(port.Init == PORTC)
		{
			PORTC->PUPDR &=~(pull<<pinNum*2);
		}else if(port.Init == PORTD)
		{
			PORTD->PUPDR &=~(pull<<pinNum*2);
		}
	}
	else
	{
		if(port.Init == PORTA)
		{
			PORTA->PUPDR |= pull<<pinNum*2;
		}else if(port.Init == PORTB)
		{
			PORTB->PUPDR |= pull<<pinNum*2;
		}else if(port.Init == PORTC)
		{
			PORTC->PUPDR |= pull<<pinNum*2;
		}else if(port.Init == PORTD)
		{
			PORTD->PUPDR |= pull<<pinNum*2;
		}
	}
	
}
//pinNum range from 0 to 3
//exti[4]
void conf_external_interupt(GPIO_TYPE port, uint32_t exti_no, uint32_t pinNum, EXTI_TYPE exti)
{
	RCC->APB2ENR |= RCC_CLK_SYSCFGR_EN<<14;
	if(exti.exti_val == EXTI_PORTA_PINS)
	{
		if(port.Init == PORTA)
		{
			SYSCFG->EXTICR[exti_no] &= ~(exti.exti_val<<pinNum*4); 
		}else if(port.Init == PORTB)
		{
			SYSCFG->EXTICR[exti_no] &= ~(exti.exti_val<<pinNum*4); 
		}else if(port.Init == PORTC)
		{
			SYSCFG->EXTICR[exti_no] &= ~(exti.exti_val<<pinNum*4); 
		}else if(port.Init == PORTD)
		{
			SYSCFG->EXTICR[exti_no] &= ~(exti.exti_val<<pinNum*4); 
		}
	}
	else
	{
		if(port.Init == PORTA)
		{
			SYSCFG->EXTICR[exti_no] |= exti.exti_val<<pinNum*4; 
		}else if(port.Init == PORTB)
		{
			SYSCFG->EXTICR[exti_no] |= exti.exti_val<<pinNum*4; 
		}else if(port.Init == PORTC)
		{
			SYSCFG->EXTICR[exti_no] |= exti.exti_val<<pinNum*4; 
		}else if(port.Init == PORTD)
		{
			SYSCFG->EXTICR[exti_no] |= exti.exti_val<<pinNum*4; 
		}
  }
	
}
void conf_external_sw_interupt(uint32_t pinNum)
{
	EXTI->SWIER |= EXTI_SW_EN<<pinNum;
}
void conf_edge(uint32_t pinNum, EDGE_TYPE edge)
{
	if(edge == RISING)
	{
		EXTI->RTSR |= RISING_EDGE_ENABLE<<pinNum;
	}
	else if(edge == FALLING)
	{
		EXTI->FTSR |= FALLING_EDGE_ENABLE<<pinNum;
	}
	else if(edge == RISING_FALLING)
	{
		EXTI->RTSR |= RISING_EDGE_ENABLE<<pinNum;
		EXTI->FTSR |= FALLING_EDGE_ENABLE<<pinNum;
	}
}
void enable_IRQ(uint32_t pinNum, IRQn_Type irq)
{
	EXTI->IMR |= 0x1<<pinNum;
	NVIC_EnableIRQ(irq);
}
void set_gpio_pin(uint32_t pinNum)
{
	GPIOD->BSRR |= (uint32_t)0x1<<pinNum;
}
void reset_gpio_pin(uint32_t pinNum)
{
	GPIOD->BSRR = (uint32_t)0x1<<(pinNum+16);
}
void clear_interupt_irq(uint32_t pinNum)
{
	EXTI->PR |= 0x1<<pinNum;
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
	conf_pin_pull(port,port.pin, port.pull);
}
