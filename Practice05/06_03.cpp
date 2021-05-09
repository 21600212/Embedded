#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE 80
char buf[MAXIMUM_BUFFER_SIZE];

static DigitalOut led(LED2);
static UnbufferedSerial pc(USBTX, USBRX, 115200);

void rx_handler()
{
    char c;

    led = !led;

    // Read the data to clear the receive interrupt
    if (pc.read(&c, 1)){
        pc.write(&c, 1);
        if (c=='\r'){
            pc.write("\n", 1);
        }
    }
}

int main(void)
{
    pc.format(8,SerialBase::None, 1);

    // Regist a callback to process a Rx (receive) interrupt.
    pc.attach(rx_handler, SerialBase::RxIrq);

    sprintf(buf, "Hello New Serial function in mbed-os v.%.1f\r\n", 6.0);
    pc.write(buf, strlen(buf));

    sprintf(buf, "Enter characters...\n\r");
    pc.write(buf, strlen(buf));

    while(1){
    }
}
