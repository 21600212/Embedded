#include "mbed.h"
  #include <RS485.h>
  Serial pc(USBTX, USBRX);
  RS485 RS485(PC_6,PC_7,PB_3); // Tx, Rx , !RE and DE MAX485 pin

  DigitalOut ho(PB_3); // this pin should be connected to !RE and DE
  typedef uint8_t byte;

  byte regvalue[9];
  byte data[9] = {0x01,0x04,0x00,0x48,0x00,0x02,0xf1,0xdd};//your data
  int main()
  {
    pc.printf("main\n");
    while(1) {
       pc.printf("Starting\n");
       ho = 1;                  // Enable sending on MAX485
       RS485.sendMsg(data,sizeof(data));
       wait_ms(600);            // Must wait for all the data to be sent
       ho = 0;                  // Enable receiving on MAX485
       pc.printf("Getting data\n");
       if(RS485.readable() >0){
           memset(regvalue,0,sizeof(regvalue));
           wait_ms(200);
           RS485.recvMsg(regvalue,sizeof(data),500);
           wait_ms(200);
           for (int count = 0; count < 9; count++) {
               pc.printf("%X - ", regvalue[count]);
           }
       }else printf("No Data\n");
       printf("Done\n");
       wait_ms(1000);
    }
  } 
