#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE 80
char buf [MAXIMUM_BUFFER_SIZE];

static BufferedSerial pc(USBTX, USBRX, 230400);

InterruptIn button(USER_BUTTON);

typedef enum state {
    Released,
    PrePressed,
    Pressed,
    PreRelease
} buttonState;

volatile buttonState btState;

void button_onpressed_cb(void){
    btState = Pressed;
}

void button_offpressed_cb(void){
    btState = Released;
}

int main() {
    sprintf(buf , "\r\nProgram Start\r\n");
    pc.write(buf , strlen(buf));

    button.mode(PullUp);
    button.fall(&button_onpressed_cb);
    button.rise(&button_offpressed_cb);

    btState = PrePressed;

    while(1) {
        if(btState==Pressed){
            pc.write("P", 1);
            ThisThread::sleep_for(10ms);
            btState = PreRelease;
        }
        if(btState==Released){
            pc.write("R", 1);
            ThisThread::sleep_for(10ms);
            btState = PrePressed;
        }
    }
}
