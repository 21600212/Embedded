#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE 80
char buffer [MAXIMUM_BUFFER_SIZE];

#define HTU21D_TEMP_READ (0xF3) // no hold command
#define HTU21D_HUMI_READ (0xF5)
#define HTU21G_USER_WRITE (0xE6)
#define HTU21D_ADDR (0x40)

//I2C i2c(PB_9_ALT0, PB_10); //D14,D6(PB_10)
I2C i2c(PB_9, PB_8); //D14, D15 (PB_8)
BufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);

const int addr7bit = 0x40 << 1;      // 7 bit I2C address
const int addr8bit = 0xF3; // 8bit I2C address, 0x90

int main()
{
    char cmd[2];
    int ack = -1;
    while (1) {
        cmd[0] = 0x01;
        cmd[1] = 0x00;


        i2c.write(addr7bit, cmd, 2);

        ThisThread::sleep_for(500);

        cmd[0] = 0x00;

        ack = i2c.write(HTU21D_TEMP_READ, cmd, 1);

        if(ack == 1){
            i2c.read(addr7bit, cmd, 2);

            float tmp = (float((cmd[0] << 8) | cmd[1]) / 256.0);
            sprintf(buffer, "Temp = %.2f\n", tmp);
            //sprintf(buffer, "Temp = %s\r\n", cmd);

            pc.write(buffer, strlen(buffer));
        }
        else{
            sprintf(buffer, "ACK = %d\n", ack);
            pc.write(buffer, strlen(buffer));
        }
    }

  sprintf(buffer, "I2C scanner done\n");
  pc.write(buffer, strlen(buffer));

}
