#include "mbed.h"

UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 230400);
UnbufferedSerial wifi(ARDUINO_UNO_D8, ARDUINO_UNO_D2, 115200);
Thread sock_thread;
char buffer[80];
char command[20];
int pointer = 0;
int flag = 0;
void check_status (){

    sprintf(buffer, "AT+CWMODE=1\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(3000ms);

    sprintf(buffer, "AT+CWJAP=\"tp_4ahnssu\",\"01051611257a\"\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(10000ms);

    sprintf(buffer, "AT+CIPSTART=\"TCP\",\"192.168.0.200\",502\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(10000ms);

    sprintf(buffer, "AT+CIPSTATUS\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(3000ms);
}

int main(){
//    command[0]=0x00;
//    command[1]=0x00;
//    command[2]=0x00;
//    command[3]=0x00;
//    command[4]=0x00;
//    command[5]=0x06;
//    command[6]=0x01;
//    command[7]=0x05;
//    command[8]=0x00;
//    command[9]=0x11;
//    command[10]=0xFF;
//    command[11]=0x00;

    command[0]=0x00;
    command[1]=0x00;
    command[2]=0x00;
    command[3]=0x00;
    command[4]=0x00;
    command[5]=0x06;
    command[6]=0x01;
    command[7]=0x05;
    command[8]=0x00;
    command[9]=0x11;
    command[10]=0xFF;
    command[11]=0x00;

    char ch;

    sprintf(buffer, "\r\n *****key <---> Wifi *****\r\n");
    pc.write(buffer, strlen(buffer));
    sprintf(buffer, "\r\nTCP and UDP Operation Using AT Commands\r\n");
    pc.write(buffer, strlen(buffer));

    sock_thread.start(&check_status);

    while(true){

        if(pc.readable()){
            pc.read(&ch, 1);
            pc.write(&ch,1);
            if(ch==0x30){
                command[9]=0x12;

                sprintf(buffer, "AT+CIPSEND=%d\r\n",12);
                wifi.write(buffer,strlen(buffer));
                ThisThread::sleep_for(500ms);

                for(int i=0; i<12; ++i){
                    //sprintf(buffer, "%c",command[i]);
                    wifi.write(command+i,1);
                }

                if(command[10]==0xFF)
                    command[10]=0x00;
                else
                    command[10]=0xFF;
            }
            else if(ch==0x31){
                command[9]=0x13;

                sprintf(buffer, "AT+CIPSEND=%d\r\n",12);
                wifi.write(buffer,strlen(buffer));
                ThisThread::sleep_for(500ms);

                for(int i=0; i<12; ++i){
                    //sprintf(buffer, "%c",command[i]);
                    wifi.write(command+i,1);
                }

                if(command[10]==0xFF)
                    command[10]=0x00;
                else
                    command[10]=0xFF;
            }
            else if(ch==0x32){
                command[9]=0x14;

                sprintf(buffer, "AT+CIPSEND=%d\r\n",12);
                wifi.write(buffer,strlen(buffer));
                ThisThread::sleep_for(500ms);

                for(int i=0; i<12; ++i){
                    //sprintf(buffer, "%c",command[i]);
                    wifi.write(command+i,1);
                    pc.write(command+i,1);
                }

                if(command[10]==0xFF)
                    command[10]=0x00;
                else
                    command[10]=0xFF;
            }
            else{
                //wifi.write(&ch,1);
            }
        }
        if(wifi.readable()){
            wifi.read(&ch,1);
            pc.write(&ch,1);
        }
    }
}
