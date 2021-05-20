#include "mbed.h"

#define HTU21D_TEMP_READ (0xF3) // no hold command
#define HTU21D_HUMI_READ (0xF5)
#define HTU21G_USER_WRITE (0xE6)
#define HTU21D_ADDR (0x40)

//I2C i2c(PB_9_ALT0, PB_10); //D14,D6(PB_10)
I2C i2c(PB_9, PB_8); //D14, D15 (PB_8)
BufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);

int main(){
  pc.printf("I2C scanner starting\n");
  i2c.frequency(100e3);
  int ack;
  unsigned char address;
  for (address = 1; address < 127; address++) {
      ack = i2c.write(address << 1, data, 1); // address is shifted to the left, because mbed uses 8bits
      if (ack == 0) {
          pc.printf("Sent 0x%X; 0x%X ACK \r\n", address << 1, address); // Send command string
      }
      wait_ms(2);
  }

  pc.printf("I2C scanner done\n");

}
