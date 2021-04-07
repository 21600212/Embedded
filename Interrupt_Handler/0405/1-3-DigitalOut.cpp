#include "mbed.h"

#define BLINKING_RATE   500ms

DigitalOut redLed(D2);
DigitalOut greenLed(D3);
DigitalOut yellowLed(D4);
DigitalOut led2(LED2);

InterruptIn button(PC_13);

int R = 1;
int Y = 2;
int G = 4;

void button_pressed(void){
    R = (R*2)%7;
    Y = (Y*2)%7;
    G = (G*2)%7;
}

// main() runs in its own thread in the OS
int main() {
    button.fall(&button_pressed);

    int rCnt = 0;
    int yCnt = 0;
    int gCnt = 0;

    while (true) {
        if(rCnt%R==0)
            redLed = !redLed;
        if(yCnt%Y==0)
            yellowLed = !yellowLed;
        if(gCnt%G==0)
            greenLed = !greenLed;

        rCnt++;
        yCnt++;
        gCnt++;

        ThisThread::sleep_for(BLINKING_RATE);
    }
}
