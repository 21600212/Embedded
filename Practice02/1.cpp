#include "mbed.h"

#define BLINKING_RATE   500

DigitalOut led2(LED2);
DigitalIn button(USER_BUTTON);

// main() runs in its own thread in the OS
int main() {
    int interval = 500;
    int i = 0;
    while (true) {
        if(button == 0){
            interval = interval << 1;
            if(interval > 4000)
                interval = 125;
        }
        if(i==interval)
            led2 = !led2;
        i++;
        ThisThread::sleep_for(1);
    }
}
