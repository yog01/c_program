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
#define GPIO_MODE_INPUT  (uint32_t)0x00 //Input (reset state)
#define GPIO_MODE_OUTPUT (uint32_t)0x01 // General purpose output mode
#define GPIO_MODE_AFIO   (uint32_t)0x02 //Alternate function mode
#define GPIO_MODE_ANALOG (uint32_t)0x03 // Analog mode

#define GPIO_OUTPUT_TYPE_PP_RE 0x1 // Output push-pull (reset state)
#define GPIO_OUTPUT_TYPE_OD_SE 0x1 // Output open-drain

//gpio speed
#define GPIO_OUTPUT_SPEED_LOW 0x00 //Low speed
#define GPIO_OUTPUT_SPEED_MED 0x01 // Medium speed
#define GPIO_OUTPUT_SPEED_HIGH 0x02 //High speed
#define GPIO_OUTPUT_SPEED_VHIGH 0x03 //Very high speed

//external interrupt configuration register 1
#define	 EXTI_PORTA_PINS  0x000F// PA[x] pin // define
#define  EXTI_PORTB_PINS  0x0001 // PB[x] pin
#define  EXTI_PORTC_PINS  0x0002 // PC[x] pin
#define  EXTI_PORTD_PINS  0x0003 //PD[x] pin

//rising interupt 
#define RISING_EDGE_DISABLE 0x0 // Rising trigger disabled (for Event and Interrupt) for input line
#define RISING_EDGE_ENABLE  0x1 //Rising trigger enabled (for Event and Interrupt) for input line

//Falling trigger event configuration bit of line x
#define FALLING_EDGE_DISABLE 0x0 //Falling trigger disabled (for Event and Interrupt) for input line
#define FALLING_EDGE_ENABLE 0x1 //Falling trigger enabled (for Event and Interrupt) for input line.
//Pullup/pulldown configuration
#define GPIO_NO_PULL_UPDOWN (uint32_t)0x11 //No pull-up, pull-down
#define GPIO_PULL_UP        (uint32_t)0x01 //Pull-up
#define GPIO_PULL_DOWN      (uint32_t)0x02 //Pull-down
//System configuration controller clock enable APB2
#define RCC_CLK_SYSCFGR_DE (uint32_t)0x0 //System configuration controller clock disabled
#define RCC_CLK_SYSCFGR_EN (uint32_t)0x1 //System configuration controller clock enabled
//software interupt unable
#define EXTI_SW_EN (uint32_t)0x1 //Software Interrupt on line x
typedef enum
{
	RISING,
	FALLING,
	RISING_FALLING
}EDGE_TYPE;

typedef struct
{
	GPIO_TypeDef *Init;
	uint32_t mode;
	uint32_t pin;
	uint32_t type;
	uint32_t speed;
	uint32_t pull;
}GPIO_TYPE;

typedef struct
{
	uint32_t exti_val;
}EXTI_TYPE;

void gpio_Init(GPIO_TYPE port);
static void conf_pin_mode(GPIO_TYPE port, uint32_t pinNum, uint32_t mode);
static void conf_pin_type(GPIO_TYPE port, uint32_t pinNum, uint32_t type);
static void conf_pin_speed(GPIO_TYPE port, uint32_t pinNum, uint32_t speed);
static void conf_pin_pull(GPIO_TYPE port, uint32_t pinNum, uint32_t pull);

void conf_external_interupt(GPIO_TYPE port, uint32_t exti_no, uint32_t pinNum, EXTI_TYPE exti);
void conf_edge(uint32_t pinNum, EDGE_TYPE edge);
void enable_IRQ(uint32_t pinNum, IRQn_Type irq);
void clear_interupt_irq(uint32_t pinNum);
void conf_external_sw_interupt(uint32_t pinNum);

void set_gpio_pin(uint32_t pinNum);
void reset_gpio_pin(uint32_t pinNum);

void EXTI0_IRQHandler(void);
#endif
