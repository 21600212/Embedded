#include "mbed.h"

#define BLINKING_RATE   500ms

int R = 1;
int Y = 2;
int G = 4;
int rCnt = 0;
int yCnt = 0;
int gCnt = 0;

PortOut motorCtl(PortB, 0x038);

InterruptIn button(PC_13);

void button_pressed(void){
    R = (R*2)%7;
    Y = (Y*2)%7;
    G = (G*2)%7;
}

// main() runs in its own thread in the OS
int main() {
    button.fall(&button_pressed);

    int LED_MASK = 0x0038;

    int rVal = 1;
    int yVal = 1;
    int gVal = 1;

    while (true) {
        if(rCnt%R==0){
            if(rVal){
                LED_MASK -= 32;
                rVal = 0;
            }
            else{
                LED_MASK += 32;
                rVal = 1;
            }
        }
        if(yCnt%Y==0){
            if(yVal){
                LED_MASK -= 16;
                yVal = 0;
            }
            else{
                LED_MASK += 16;
                yVal = 1;
            }
        }
        if(gCnt%G==0){
            if(gVal){
                LED_MASK -= 8;
                gVal = 0;
            }
            else{
                LED_MASK += 8;
                gVal = 1;
            }
        }

        motorCtl = LED_MASK;

        rCnt++;
        yCnt++;
        gCnt++;

        ThisThread::sleep_for(BLINKING_RATE);
    }
}
