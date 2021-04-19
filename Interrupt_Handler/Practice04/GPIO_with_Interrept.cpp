#include "mbed.h"

volatile int interval;

void led2_init(void);
void led2_toggle(void);
void button_init(void);
void button_Handler(void);  // ISR of EXTI15_10_IRQn

int main() {
    led2_init();
    button_init();

    NVIC_SetVector(EXTI15_10_IRQn, (uint32_t)button_Handler);

    interval = 500;

    while (true) {
        led2_toggle();
        ThisThread::sleep_for(chrono::milliseconds(interval));
    }
}

void led2_init(void){
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIOA->MODER &= ~(0b11 << 10);
    GPIOA->MODER |= (0b01 << 10);
}
void button_init(void){
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIOC->MODER &= ~(0b11 << 26);

    EXTI->IMR = (0b01<<13);
    EXTI->RTSR = 0;
    EXTI->FTSR = (0b01<<13);


}
void button_Handler(void){
    interval = interval << 1;
    if(interval > 2000)
        interval = 500;
}
void led2_toggle(void){
    GPIOA->ODR ^= (0b1 << 5);
}
