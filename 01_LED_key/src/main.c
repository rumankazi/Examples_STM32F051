#include "stm32f0xx_conf.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#define TIMER_TICK_HZ 1000u

volatile unsigned long timer_tick = 0;

void SysTick_Handler(void)
{
  if(timer_tick > 0)
	  --timer_tick;
}

void delay_ms(unsigned int time_ms)
{
	timer_tick = time_ms;
	while(timer_tick);
}

int main(void)
{

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 	// enable the clock to GPIOC
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 	// enable the clock to GPIOA

	// Put PORTC.8 in output mode where the LED is connected
	GPIOC->MODER |= (1 << 16);

	// Put PORTC.9 in output mode
	GPIOC->MODER |= (1 << 18);

	// Put PORTA.0 in input mode where the SW is connected
	GPIOA->MODER &= ~(3 << 0);

	// This configures interrupt such that SysTick_Handler is called
	// at ever TIMER_TICK_HZ i.e. 1/1000 = 1ms
	SysTick_Config(SystemCoreClock / TIMER_TICK_HZ);

	while(1)
	{
		// Read SW of PORTA.0	
		if ((GPIOA->IDR & 0x0001) !=0x0000)
		{
			 // Set PORTC.8
		   GPIOC->BSRR = (1 << 8);
		 }
		else
		{
		   // Reset PORTC.8
		   GPIOC->BRR = (1 << 8);
		   //delay_ms(1000);
		}
		
		
	}

}
