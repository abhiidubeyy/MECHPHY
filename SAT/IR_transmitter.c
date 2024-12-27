#include <stdint.h>

#define SIZE 8
uint32_t * TIM16CR1 = (uint32_t *)0x40014400;
uint32_t * TIM16ARR = (uint32_t *)0x4001442c;
uint32_t * TIM16PSC = (uint32_t *)0x40014428;
uint32_t * TIM16MR1 = (uint32_t *)0x40014418;
uint32_t * TIM16CER = (uint32_t *)0x40014420;
uint32_t * TIM16CNT = (uint32_t *)0x40014424;
uint32_t * TIM16CCR = (uint32_t *)0x40014434;
uint32_t * TIM16BDT = (uint32_t *)0x40014444;
uint32_t * TIM16EGR = (uint32_t *)0x40014414;
uint32_t * SYSCFG_CFGR1 = (uint32_t *)0x40010004;
uint32_t * RCC_APB2 = (uint32_t *)0x40021060;
uint32_t * RCC_AHB2ENR = (uint32_t *)0x4002104c;
uint32_t * GA_MODER = (uint32_t *)0x48000000;
uint32_t * GA_PUPDR = (uint32_t *)0x4800000c;
uint32_t * GA_AFRH = (uint32_t *)0x48000020;
uint32_t * NVIC_ENR = (uint32_t *)0xe000e100;
uint32_t * NVIC_PR5 = (uint32_t *)0xe000e414;
uint32_t * TIM16IER = (uint32_t *)0x4001440c;
uint32_t * TIM16IFR = (uint32_t *)0x40014410;
uint32_t * TIM16RCR = (uint32_t *)0x40014430;
uint32_t * TIM17CR1 = (uint32_t *)0x40014800;
uint32_t * TIM17ARR = (uint32_t *)0x4001482c;
uint32_t * TIM17PSC = (uint32_t *)0x40014828;
uint32_t * TIM17MR1 = (uint32_t *)0x40014818;
uint32_t * TIM17CER = (uint32_t *)0x40014820;
uint32_t * TIM17CNT = (uint32_t *)0x40014824;
uint32_t * TIM17CCR = (uint32_t *)0x40014834;
uint32_t * TIM17EGR = (uint32_t *)0x40014814;
uint32_t * TIM17BDT = (uint32_t *)0x40014844;
uint32_t * GB_MODER = (uint32_t *)0x48000400;
uint32_t * GB_AFRHR = (uint32_t *)0x48000424;
uint32_t * GB_PUPDR = (uint32_t *)0x4800040c;

int initialization();
int send_byte(int x[8]);
int send_inverse(int x[8]);
int tim_status = 0;
int a[SIZE] = {1,0,1,0,1,1,0,0};
volatile int l_flag = 1;

int main(void)
{
	*NVIC_ENR |= (0x01<<25);
	*NVIC_PR5 |= (0xA<<8);
	*RCC_AHB2ENR |= 0x03;
	*RCC_APB2 |= (1<<17);
	*SYSCFG_CFGR1 |= (0x01<<19);
	*TIM16ARR &= ~(0xFFFF);
	*TIM16ARR |= 0xD2EF;
	*TIM16EGR |= 0x1;
	*TIM16PSC |= 0;
	*TIM16MR1 &= ~(0xffffffff);
	*TIM16MR1 |= (0x6<<4);
	*TIM16MR1 |= (0x01<<3);
	*TIM16CER |= 1;
	*TIM16CCR &= ~(0xFFFF);
	*TIM16CCR |= (0X8CA0);
	*TIM16BDT |= (0x01<<14);
	*GA_MODER &= ~(0x03<<12);
	*GA_MODER |= (0x02<<12);
	*GA_PUPDR &= ~(0x03<<12);
	*GA_PUPDR |= (0x02<<12);
	*GA_AFRH &= ~(0xF<<24);
	*GA_AFRH |= (0xE<<24);
	*TIM16IER |= (0x01);
	*GB_MODER &= ~(0x03<<18);
	*GB_MODER |= (0x02<<18);
	*GB_PUPDR &= ~(0x03<<18);
	*GB_PUPDR |= (0x02<<18);
	*GB_AFRHR |= (0x01<<4);
	*RCC_AHB2ENR |= 0x01;
	*RCC_APB2 |= (1<<18);
	*TIM17ARR = 0x63;
	*TIM17EGR |= 0x1;
	*TIM17PSC = 0;
	*TIM17MR1 &= ~(0xffffffff);
	*TIM17MR1 |= (0x6<<4);
	*TIM17MR1 |= (0x01<<3);
	*TIM17CER |= 1;
	*TIM17CCR &= ~(0xFFFF);
	*TIM17CCR |= (0x32);
	*TIM17BDT |= (0x01<<14);
	*GA_MODER &= ~(0x03<<14);
	*GA_MODER |= (0x02<<14);
	*GA_PUPDR &= ~(0x03<<14);
	*GA_PUPDR |= (0x03<<14);
	*GA_AFRH &= ~(0xF<<28);
	*GA_AFRH |= (0xE<<28);
	*TIM17CR1 |= (1<<7)|(1<<0);
	tim_status = 0;
	while(1)
	{
		initialization();
		while(l_flag == 0);
		send_byte(a);
		while(l_flag == 0);
		send_inverse(a);
	}
}

void TIM1_UP_TIM16_IRQHandler(void)
{
	l_flag++;
	*TIM16CR1 &= ~(0x01);
	*TIM16IFR &= ~(0x01);
}

int initialization()
{
	l_flag--;
	*TIM16ARR = 0xD2EF;
	*TIM16CCR = (0X8CA0);
	*TIM16CR1 |= (1<<7)|(1<<0);
	return 0;
}

int send_byte(int x[8])
{
	for(int i=0;i<8;i++)
	{
		if(x[i]==1)
		{
			l_flag--;
			*TIM16ARR = 0x2327;
			*TIM16CCR = 0x8CA;
			*TIM16CR1 |= 0x01;
		}
		else if(x[i]==0)
		{
			l_flag--;
			*TIM16ARR = 0x1193;
			*TIM16CCR = 0x8CA;
			*TIM16CR1 |= 0x01;
		}
		while(l_flag == 0);
	}
	return 0;
}

int send_inverse(int x[8])
{
	for(int i=0;i<8;i++)
	{
		if(x[i]==0)
		{
			l_flag--;
			*TIM16ARR = 0x2327;
			*TIM16CCR = 0x8CA;
			*TIM16CR1 |= 0x01;
		}
		else if(x[i]==1)
		{
			l_flag--;
			*TIM16ARR = 0x1193;
			*TIM16CCR = 0x8CA;
			*TIM16CR1 |= 0x01;
		}
		while(l_flag == 0);
	}
	return 0;
}
