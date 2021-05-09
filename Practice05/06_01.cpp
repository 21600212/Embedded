#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE 80
char buf [MAXIMUM_BUFFER_SIZE];

// Create a DigitalOutput object to toggle an LED whenever data is received.
static DigitalOut led(LED2);

// Create a BufferedSerial object with a default baud rate.
static BufferedSerial pc(USBTX, USBRX, 115200);

int main(void)
{
    sprintf(buf , "Hello New Serial function in mbed os v.%.1f\r\n", 6.0);
    pc.write(buf , strlen(buf));

    sprintf(buf , "Enter characters...\r\n");
    pc.write(buf , strlen(buf));

    while (1) {
        if (pc.readable()){
            led = !led; // toggle LED

            // Echo the input back to the terminal.
            int num = pc.read(buf ,1);
            pc.write(buf, num);
            if (buf[0] == '\r') {
                pc.write("\n", 1);
            }
        }
    }
}
