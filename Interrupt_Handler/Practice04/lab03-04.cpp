#include "mbed.h"

InterruptIn button(USER_BUTTON);
// InterruptIn button2(USER_BUTTON);
Timer t;

int start_t, last;
int end_t;
int PullDown_t;
int bt_mode;
volatile int is_bt_pressed, is_bt_pressed_off;

void button_onpressed_cb(void);
void button_offpressed_cb(void);

void button_onpressed_cb(void){
    if(start_t == 0)
        last = start_t = chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
    else if(start_t == -1)
        start_t = 0;
    else{
        end_t = chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
        is_bt_pressed = 1;
    }
}

void button_offpressed_cb(void){
    PullDown_t = chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
    is_bt_pressed_off = 1;
}

int main() {
    printf("\n*** Timer started ***\r\n\n");
    button.mode(PullUp);
    button.fall(&button_onpressed_cb);

    t.start();
    while(1) {
        if(is_bt_pressed && start_t != 0){
            printf("Lab time: %d / %d (ms).\r\n", end_t - start_t, end_t - last);
            last = end_t;
            is_bt_pressed = 0;
            button.mode(PullDown);
            button.rise(&button_offpressed_cb);
        }
        if(is_bt_pressed_off){
            if(PullDown_t - last > 1000){
                start_t = -1;
                last = PullDown_t = 0;
                t.reset();
                printf("\r\n@@@ Reset Timer @@@\r\n\n");
            }
            is_bt_pressed_off = 0;
            button.mode(PullUp);
            button.rise(&button_onpressed_cb);
        }
    }
}
