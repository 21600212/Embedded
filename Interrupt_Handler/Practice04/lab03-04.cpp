#include "mbed.h"

InterruptIn button(USER_BUTTON);
InterruptIn button2(USER_BUTTON);
Timer t;

int start, last;
int end_t;
int PullDown_t;
volatile int bt_pressed, bt2_pressed;

void button_onpressed_cb(void){
    if(start == 0)
        start = t.elapsed_time().count();
    else{
        end_t = t.elapsed_time().count();
        bt_pressed = 1;
    }
}

void button2_onpressed_cb(void){
    PullDown_t = t.elapsed_time().count();
    bt2_pressed = 1;
}

int main() {
    printf("\r\n*** System started ***\r\n");
    button.mode(PullUp);
    button2.mode(PullDown);
    button.fall(&button_onpressed_cb);
    button2.rise(&button2_onpressed_cb);

    int i;
    float sum;

    printf("\r\n*** Basic Timer Example ***\r\n");

    t.start();
    while(1) {
        if(bt_pressed){
            end_t/=1000;
            start/=1000;

            printf("Lab time is %d | %d (s).\r\n", end_t - start, end_t - last);
            printf("Pull down val: %d\r\n", PullDown_t);
            last = end_t;
            bt_pressed = 0;
        }
        if(bt2_pressed){
            printf("Pull down time is %d (s).\r\n",  PullDown_t - last);
            if(PullDown_t - last > 1000){
                start = last = 0;
            }
            bt2_pressed = 0;
        }
    }
}
