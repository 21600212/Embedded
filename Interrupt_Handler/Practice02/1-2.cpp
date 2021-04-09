#include "mbed.h"

#define BLINKING_RATE   500ms

DigitalOut redLed(D2);
DigitalOut greenLed(D3);
DigitalOut yellowLed(D4);
DigitalOut led2(LED2);


// main() runs in its own thread in the OS
int main() {
    int yCnt = 0;
    int gCnt = 0;

    while (true) {
        if(yCnt%2==0)
            yellowLed = !yellowLed;
        if(gCnt%4==0)
            greenLed = !greenLed;
        redLed = !redLed;
        led2 = !led2;
        ThisThread::sleep_for(BLINKING_RATE);
        yCnt++;
        gCnt++;
    }
}
