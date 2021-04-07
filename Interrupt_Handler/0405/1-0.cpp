#include "mbed.h"

#define BLINKING_RATE   500ms

DigitalOut redLed(D2);
DigitalOut greenLed(D3);
DigitalOut yellowLed(D4);
DigitalOut led2(LED2);

InterruptIn button(PC_13);

void button_pressed(void){
    redLed = !redLed;
}

// main() runs in its own thread in the OS
int main() {
    button.fall(&button_pressed);

    int yCnt = 0;
    int gCnt = 0;

    while (true) {
        if(yCnt++/2==0) yellowLed = !yellowLed;
        if(yCnt++/2==0) greenLed = !greenLed;
        redLed = !redLed;
        led2 = !led2;
        ThisThread::sleep_for(BLINKING_RATE);
    }
}
