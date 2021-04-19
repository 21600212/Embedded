#include "mbed.h"

DigitalOut led2(LED2);
InterruptIn button(USER_BUTTON);// Used for blinking the LED2
Ticker led2_ticker;
void timeout_cb(void);

float interval = 0.5;   // 500ms

// ISR to handle button pressed event
void button_onpressed_cb(void){
    led2_ticker.detach();  // disable the ticker
    interval = interval * 2;
    if (interval > 4.0) {
        interval = 0.125;
    }
    led2 = 1; // turn on the LED2
    led2_ticker.attach(&timeout_cb, interval);
}

// ISR for time out
void timeout_cb(void){
    led2 = !led2;
}

int main(){
    button.mode(PullUp); // Activate pull-up
    // Attach ISR to handle button-pressed event
    button.fall(&button_onpressed_cb);

    // timer ISRfor blinking the LED2
    led2_ticker.attach(&timeout_cb, interval);
    while(1) {
    }
}
