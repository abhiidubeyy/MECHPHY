#include <stdint.h>

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
volatile uint32_t edge_u = 0x00000000;
volatile uint32_t edge_d = 0x00000000;
volatile uint32_t time_gap = 0x00000000;
volatile int pin_state = 1;
volatile uint32_t * GA_OPDR = (uint32_t *)0x48000014;
volatile int header_;
int check_header();
int check_bits();
int check_integrity();
volatile int a[8] = {0,0,0,0,0,0,0,0};
volatile int i_a[8] = {1,1,1,1,1,1,1,1};
int main()
{
	*RCC_AHB2ENR |= 0x01;
	*RCC_APB1ENR |= 0x01;
	*GA_MODER &= ~(0x0f<<10);
	*GA_MODER |= (0x02<<10)|(0x01<<12);
	*GA_PUPDR |= (0x01<<10);
	*GA_AFRL |= (0x01<<20);
	*TIM2CCMR |= (0x01);
	*TIM2CCER |= (0x03);
	*TIM2CCER |= (0x08);
	*TIM2_ARR = (0x9C3F);
	*TIM2_PSC = (0x00);
	*TIM2DIER |= (0x02);
	*TIM2_CR1 |= (0x01<<7)|(0x01);
	*NVICISER |= (0x01<<28);
	while(1)
	{
		while(pin_state){};
		while(!pin_state){};
		int x = check_header();
		if(x==0)
		{
			int y = check_bits();
			if(y == 1)
			{
				int  z = check_integrity();
				if(z == 1)
				{
					*GA_OPDR |= (0x01<<6);
					a[0]=0;
					a[1]=0;
					a[2]=0;
					a[3]=0;
					a[4]=0;
					a[5]=0;
					a[6]=0;
					a[7]=0;
				}
			}
			else
			{
				*GA_OPDR &= ~(0x01<<6);
				continue;
			}
		}
		else
		{
			*GA_OPDR &= ~(0x01<<6);
			continue;
		}
	}
}

void TIM2_IRQHandler(void)
{
	if(pin_state == 1)
	{
		pin_state = 0;
		edge_d  = *TIM2CCR1;
		*TIM2CCER &= ~(0x02);
	}
	else
	{
		pin_state = 1;
		edge_u = *TIM2CCR1;
		*TIM2CCER |= (0x02);
	}
	*TIM2_IFR &= ~(0x02);
}

int check_header()
{
	if(edge_u>edge_d)
	{
		time_gap = edge_u - edge_d;
	}
	else
	{
		time_gap = (0x9C3F - edge_d)+edge_u;
	}
	if(time_gap >= 0x84D0 && time_gap <= 0x8CA0)
	{
		while(pin_state);
		if(edge_u<edge_d)
		{
			time_gap = edge_d - edge_u;
		}
		else
		{
			time_gap = (0x9C3F - edge_u)+edge_d;
		}
		if(time_gap >= 0x3B60 && time_gap <= 0x47E0)
		{
			header_ = 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
	return 0;
}

int check_bits()
{
	for(int j = 0;j<8;j++)
	{
		while(!pin_state);
		if(edge_u>edge_d)
		{
			time_gap = edge_u - edge_d;
		}
		else
		{
			time_gap = (0x9C3F - edge_d)+edge_u;
		}
		if(time_gap >= 0x708 && time_gap <= 0xA28)
		{
			while(pin_state);
			if(edge_u<edge_d)
			{
				time_gap = edge_d - edge_u;
			}
			else
			{
				time_gap = (0x9C3F - edge_u)+edge_d;
			}
			if(time_gap >= 0x708 && time_gap <= 0xA28)
			{
				a[j] = 0;
				i_a[j] = 1;
			}
			else if(time_gap >= 0x1388 && time_gap <= 0x1B58)
			{
				a[j] = 1;
				i_a[j] = 0;
			}
		}
	}
	return 1;
}

int check_integrity()
{
	for(int j = 0;j<8;j++)
	{
		if(a[j]==i_a[j])
		{
			return -1;
		}
	}
	return 1;
}
