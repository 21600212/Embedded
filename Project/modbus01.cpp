#include "mbed.h"

UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 230400);
UnbufferedSerial wifi(ARDUINO_UNO_D8, ARDUINO_UNO_D2, 115200);
Thread sock_thread;
char buffer[80];
char command[20];
int pointer = 0;
int flag = 0;


int main(){
    command[0]=0x00;
    command[1]=0x00;
    command[2]=0x00;
    command[3]=0x00;
    command[4]=0x00;
    command[5]=0x08;
    command[6]=0x01;
    command[7]=0x0F;
    command[8]=0x00;
    command[9]=0x10;
    command[10]=0x00;
    command[11]=0x08;
    command[12]=0x01;
    command[13]=0x02;

    char ch;

    sprintf(buffer, "\r\n *****key <---> Wifi *****\r\n");
    pc.write(buffer, strlen(buffer));
    sprintf(buffer, "\r\nTCP and UDP Operation Using AT Commands\r\n");
    pc.write(buffer, strlen(buffer));

    sprintf(buffer, "AT+CWMODE=1\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(2000ms);

    sprintf(buffer, "AT+CWJAP=\"turtlebot01\",\"turtlebot\"\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(2000ms);

    sprintf(buffer, "AT+CIPMUX=1\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(2000ms);

    sprintf(buffer, "AT+CIPSTART=\"TCP\",\"192.168.0.200\",502\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(2000ms);



    sprintf(buffer, "*********client build complete*********\r\n");
    pc.write(buffer, strlen(buffer));


    while(true){
        if(pc.readable()){
            pc.read(&ch, 1);
            pc.write(&ch,1);
            if(ch==0x2b){
                sprintf(buffer, "Command=%d\r\n",strlen(command));
                pc.write(buffer,strlen(buffer));
                sprintf(buffer, "AT+CIPSEND=%d\r\n",strlen(command));
                wifi.write(buffer,strlen(buffer));
                ThisThread::sleep_for(500ms);
                sprintf(buffer, "%s\r\n",command);
                wifi.write(buffer,strlen(buffer));
            }
            else if(ch==0x2d){
                    sprintf(buffer, "AT+CIPSTATUS\r\n");
                    wifi.write(buffer, strlen(buffer));
                    ThisThread::sleep_for(2000ms);
            }
            else{
                sprintf(buffer,"else!\r\n");
                pc.write(buffer,strlen(buffer));
                wifi.write(&ch,1);
            }
        }
        if(wifi.readable()){
            wifi.read(&ch,1);
            pc.write(&ch,1);
        }
    }
}
