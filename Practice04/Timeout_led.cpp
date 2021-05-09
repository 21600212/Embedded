#include "mbed.h"

Ticker time1;
Timeout timeout;

DigitalOut led1(D3);
DigitalOut led2(D4);

void blink_led1(){
    led1 = !led1;
}

void off_led2() {
    led2 = 0;
}

int main() {
    led2 = 1;

    time1.attach(&blink_led1, 0.5);
    timeout.attach(&off_led2, 10.0);

    while(1){
    }
}
