#include "mbed.h"
#include <cstring>

#define MAXIMUM_BUFFER_SIZE 80
char buf [MAXIMUM_BUFFER_SIZE];
char tmp [MAXIMUM_BUFFER_SIZE];

// Create a DigitalOutput object to toggle an LED whenever data is received.
static DigitalOut led(LED2);
static DigitalOut redLed(D3);
static DigitalOut greenLed(D4);

// Create a BufferedSerial object with a default baud rate.
static BufferedSerial blt(D8, D2, 9600);
static BufferedSerial pc(USBTX, USBRX, 115200);


int main(void)
{
    sprintf(buf , "Hello New Serial function in mbed os v.%.1f\r\n", 6.0);
    pc.write(buf , strlen(buf));

    sprintf(buf , "Enter characters...\r\n");
    pc.write(buf , strlen(buf));

    int cnt = 0;
    int flg = 0;

    while (1) {
        if (pc.readable()){
            led = !led; // toggle LED

            int num = pc.read(buf ,1);
            pc.write(buf, num);
            blt.write(buf, num);
        }
        if(blt.readable()){
            int num = blt.read(buf ,strlen(buf));
            pc.write(buf, num);
            if (buf[0] == '\n'){
                flg = 1;
            }
            else {
                tmp[cnt++] = buf[0];
            }
        }
        else if(flg){
            flg = 0;
            if(!strncmp(tmp, "rLed On", 7)){
                redLed = 1;
                sprintf(buf , "Rled status: on\r\n");
            }
            else if(!strncmp(tmp, "rLed Off", 8)){
                redLed = 0;
                sprintf(buf , "Rled status: off\r\n");
            }
            else if(!strncmp(tmp, "gLed On", 7)){
                greenLed = 1;
                sprintf(buf , "Gled status: on\r\n");
            }
            else if(!strncmp(tmp, "gLed Off", 8)){
                greenLed = 0;
                sprintf(buf , "Gled status: off\r\n");
            }
            else if(!strncmp(tmp, "rLed Status", 11)){
                if(redLed)
                    sprintf(buf , "Rled status: on\r\n");
                else
                    sprintf(buf , "Rled status: off\r\n");
            }
            else if(!strncmp(tmp, "gLed Status", 11)){
                if(greenLed)
                    sprintf(buf , "Gled status: on\r\n");
                else
                    sprintf(buf , "Gled status: off\r\n");
            }
            else{
                sprintf(buf , "Undefined command\r\n");
            }
            blt.write(buf, strlen(buf));
            pc.write(buf , strlen(buf));
            cnt = 0;
        }
    }
}
