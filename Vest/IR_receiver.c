#include <stdint.h>

#define SIZE 33

uint32_t * RCC_AHB2ENR = (uint32_t *)0x4002104c;
uint32_t * RCC_APB1ENR = (uint32_t *)0x40021058;
uint32_t * TIM2_CR1 = (uint32_t *)0x40000000;
uint32_t * GA_MODER = (uint32_t *)0x48000000;
uint32_t * GA_PUPDR = (uint32_t *)0x4800000c;
uint32_t * GA_AFRL = (uint32_t *)0x48000020;
uint32_t * TIM2CCMR = (uint32_t *)0x40000018;
uint32_t * TIM2CCER = (uint32_t *)0x40000020;
uint32_t * TIM2_ARR = (uint32_t *)0x4000002c;
uint32_t * TIM2_PSC = (uint32_t *)0x40000028;
uint32_t * TIM2DIER = (uint32_t *)0x4000000c;
uint32_t * NVICISER = (uint32_t *)0xe000e100;
volatile uint32_t * TIM2_IFR = (uint32_t *)0x40000010;
volatile uint32_t * TIM2CCR1 = (uint32_t *)0x40000034;
volatile uint32_t * GA_OPDR = (uint32_t *)0x48000014;
volatile uint16_t a[SIZE];
volatile uint16_t previous_edge = 0x0000;
volatile uint16_t current_edge = 0x0000;
volatile uint8_t change_flag = 0;
volatile uint16_t time;
volatile uint8_t ind = 0;
volatile uint8_t header = 0;
int main()
{
	*RCC_AHB2ENR |= 0x01;
	*RCC_APB1ENR |= 0x01;
	*GA_MODER &= ~(0x0f<<10);
	*GA_MODER &= ~(0x0f);
	*GA_MODER |= (0x02<<10)|(0x01);
	*GA_PUPDR |= (0x01<<10);
	*GA_AFRL |= (0x01<<20);
	*TIM2CCMR |= (0x01);
	*TIM2CCER |= (0x03);
	*TIM2CCER &= ~(0x08);
	*TIM2_ARR = (0xEA5F);
	*TIM2_PSC = (0x00);
	*TIM2DIER |= (0x02);
	*TIM2_CR1 |= (0x01<<7)|(0x01);
	*NVICISER |= (0x01<<28);
	while(1)
	{
		while(!(change_flag));
		change_flag = 0;
		if(previous_edge<current_edge)
		{
			time = current_edge-previous_edge;
		}
		else
		{
			time = (0xffff-previous_edge)+current_edge;
		}
		if(header==0)
		{
			if(time >= 0xCB20 && time <= 0xDAC0)
			{
				header = 1;
				ind = 0;
			}
			else
			{
				header = 0;
				ind = 0;
			}
		}
		else
		{
			if (time >= 0xFA0 && time <=0x1770)
			{
				a[ind]=0;
			}
			else if(time >= 0X20D0 && time <=0x2580)
			{
				a[ind]=1;
			}
			if(ind==32)
			{
				ind = 0;
				header = 0;
				*GA_OPDR |= (0x01);
				continue;
			}
			ind++;
		}
	}
}

void TIM2_IRQHandler(void)
{
	change_flag = 1;
	previous_edge = current_edge;
	current_edge = *TIM2CCR1;
	*TIM2_IFR &= ~(0x02);
}
