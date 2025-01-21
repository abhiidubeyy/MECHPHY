#include <stdint.h>

#define LED_PIN (1 << 0)  
#define DELAY_5_SEC 650500  

uint32_t * RCC_AHB2ENR = (uint32_t *)0x4002104c;
uint32_t * RCC_APB1ENR = (uint32_t *)0x40021058;
uint32_t * TIM2_CR1 = (uint32_t *)0x40000000;

uint32_t * GA_MODER = (uint32_t *)0x48000000;
uint32_t * GA_AFRL = (uint32_t *)0x48000020;
uint32_t * GA_ODR = (uint32_t *)0x48000014; 

uint32_t * TIM2CCMR = (uint32_t *)0x40000018;
uint32_t * TIM2CCER = (uint32_t *)0x40000020;
uint32_t * TIM2DIER = (uint32_t *)0x4000000c;
uint32_t * NVICISER = (uint32_t *)0xe000e100;
volatile uint32_t * TIM2_IFR = (uint32_t *)0x40000010;
volatile uint32_t * TIM2CCR1 = (uint32_t *)0x40000034;

volatile uint32_t led_timer = 0;  

void delay(uint32_t count);

int main()
{
    *RCC_AHB2ENR |= 0x01;
    *RCC_APB1ENR |= 0x01;

    *GA_MODER &= ~(0x03 << 0);
    *GA_MODER |= (0x01 << 0);
    *GA_ODR &= ~LED_PIN;

    *GA_MODER &= ~(0x0F << 10);
    *GA_MODER |= (0x02 << 10);
    *GA_AFRL |= (0x01 << 20);

    *TIM2CCMR |= (0x01 << 0);
    *TIM2CCER |= (0x01);
    *TIM2DIER |= (0x02);
    *TIM2_CR1 |= (0x01);

    *NVICISER |= (0x01 << 28);

    while (1)
    {
        if (led_timer > 0)
        {
            led_timer--;  
        }
        else
        {
            *GA_ODR &= ~LED_PIN; 
        }
    }
}

void TIM2_IRQHandler(void)
{
    *GA_ODR |= LED_PIN;   
    led_timer = DELAY_5_SEC;  
    *TIM2_IFR &= ~(0x02); 
}

void delay(uint32_t count)
{
    for (volatile uint32_t i = 0; i < count; i++);
}
