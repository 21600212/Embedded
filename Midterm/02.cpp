#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE 80
char buf [MAXIMUM_BUFFER_SIZE];

static BufferedSerial pc(USBTX, USBRX, 230400);

InterruptIn button(USER_BUTTON);

typedef enum state {
    Released,
    PrePressed,
    LongPressed,
    ShortPressed,
    Pressed,
    PreRelease
} buttonState;

volatile buttonState btState;

Timer t;
int pressed_t, curr_t;

void button_onpressed_cb(void){
    pressed_t = chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
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

    t.start();

    while(1) {
      curr_t = chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
      if(btState==PreRelease && curr_t - pressed_t > 1000){
          sprintf(buf , "Long Press\r\n");
          pc.write(buf , strlen(buf));
          btState = LongPressed;
      }
      else{
          sprintf(buf , "Short Press\r\n");
          pc.write(buf , strlen(buf));
          btState = ShortPressed;
      }
      if(btState==Pressed){
          ThisThread::sleep_for(10ms);
          btState = PreRelease;
      }
      if(btState==Released){
          ThisThread::sleep_for(10ms);
          btState = PrePressed;
      }
    }
}
