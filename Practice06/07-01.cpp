#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE 80
#define UPPER_THRESH 180
#define LOWER_THRESH 80

DigitalOut led0(D3);

char buf [MAXIMUM_BUFFER_SIZE];

AnalogIn ain0(A0);
BufferedSerial pc(USBTX, USBRX, 115200);

int main() {
    float meas;

    sprintf(buf, "\r\n Welcome to Analog In Lab.\r\n");
    pc.write(buf, strlen(buf));

    while(1) {
        meas = ain0.read(); // Converts and read the analog input value [0.0]
        meas = meas * 3300; // Change the value to be in the 0 to 3300 mV range
        sprintf(buf , "A0 input voltage = %.2f mV\r\n", meas);
        pc.write(buf, strlen(buf));
        if(meas>UPPER_THRESH)
            led0 = 0;
        else if(meas<LOWER_THRESH)
            led0 = 1;
        ThisThread::sleep_for(chrono::milliseconds(5000));
    }
}
