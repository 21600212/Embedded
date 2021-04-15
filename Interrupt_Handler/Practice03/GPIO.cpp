#include "mbed.h"

void led2_init(void);
void led2_toggle(void);
void button_init(void);
uint32_t button_input(void);

int main() {
    int interval = 500;
    uint32_t val;

    led2_init();
    button_init();

    while (true) {
        val = button_input();
        if(val==0){
            interval = interval << 1;
            if(interval > 2000)
                interval = 500;
        }

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
}

uint32_t button_input(void){
    uint32_t inputData = GPIOC->IDR;
    inputData &= (0b1 << 13);

    return inputData;
}

void led2_toggle(void){
    GPIOA->ODR = ~GPIOA->ODR;
}
