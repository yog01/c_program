#ifndef HAL_GPIO
#define HAL_GPIO 1


#include<stdio.h>
#include "stm32f407xx.h"

#define PORTA GPIOA
#define PORTB GPIOB
#define PORTC GPIOC
#define PORTD GPIOD

// clock enable
#define RCC_AHB1_CLK_GPIOAEN (RCC->AHB1ENR |= (uint32_t)0x1<<0)
#define RCC_AHB1_CLK_GPIOBEN (RCC->AHB1ENR |= (uint32_t)0x1<<1)
#define RCC_AHB1_CLK_GPIOCEN (RCC->AHB1ENR |= (uint32_t)0x1<<2)
#define RCC_AHB1_CLK_GPIODEN (RCC->AHB1ENR |= (uint32_t)0x1<<3)

//mode configure
#define GPIO_MODE_INPUT  0x00 //Input (reset state)
#define GPIO_MODE_OUTPUT 0x01 // General purpose output mode
#define GPIO_MODE_AFIO 	 0x02 //Alternate function mode //0x02
#define GPIO_MODE_ANALOG 0x03 // Analog mode //0x03

#define GPIO_OUTPUT_TYPE_RE 0x1 // Output push-pull (reset state)
#define GPIO_OUTPUT_TYPE_SE 0x1 // Output open-drain

//gpio speed
#define GPIO_OUTPUT_SPEED_LOW   0x00 //Low speed
#define GPIO_OUTPUT_SPEED_MED   0x01 // Medium speed
#define GPIO_OUTPUT_SPEED_HIGH  0x02 //High speed
#define GPIO_OUTPUT_SPEED_VHIGH 0x03 //Very high speed


typedef struct
{
	GPIO_TypeDef *Init;
	uint32_t mode;
	uint32_t pin;
	uint32_t type;
	uint32_t speed;
	
}GPIO_TYPE;

void gpio_Init(GPIO_TYPE port);
static void conf_pin_mode(GPIO_TYPE port, uint32_t pinNum, uint32_t mode);
static void conf_pin_type(GPIO_TYPE port, uint32_t pinNum, uint32_t type);
static void conf_pin_speed(GPIO_TYPE port, uint32_t pinNum, uint32_t speed);

#endif
