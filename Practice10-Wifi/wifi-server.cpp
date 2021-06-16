#include "mbed.h"

UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 230400);
UnbufferedSerial wifi(ARDUINO_UNO_D8, ARDUINO_UNO_D2, 115200);
Thread sock_thread;
char buffer[80];
char message[2048];
int pointer = 0;
int flag = 0;

int main(){
    char ch;

    sprintf(buffer, "\r\n *****key <---> Wifi *****\r\n");
    pc.write(buffer, strlen(buffer));
    sprintf(buffer, "\r\nTCP and UDP Operation Using AT Commands\r\n");
    pc.write(buffer, strlen(buffer));

    sprintf(buffer, "AT+CWMODE=1\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(2000ms);

    sprintf(buffer, "AT+CWJAP=\"DL-Lab 2G\",\"!nth0319\"\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(2000ms);

    sprintf(buffer, "AT+CIPMUX=1\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(2000ms);

    sprintf(buffer, "AT+CIPSERVER=1,50000\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(2000ms);

    sprintf(buffer, "AT+CIFSR\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(2000ms);

    sprintf(buffer, "AT+CIPRECVMODE=0\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(2000ms);

    sprintf(buffer, "*********server build complete*********\r\n");
    pc.write(buffer, strlen(buffer));


    while(true){
        if(wifi.readable()){
            wifi.read(&ch,1);
            pc.write(&ch,1);
            if(ch == '+'){
                flag = 1;
            }
            if(flag){
                if(ch == '\r'){
                    sprintf(buffer, "AT+CIPSEND=0,%d\r\n",strlen(message));
                    wifi.write(buffer,strlen(buffer));
                    ThisThread::sleep_for(500ms);
                    sprintf(buffer, "%s\r\n",message);
                    wifi.write(buffer,strlen(buffer));
                    pointer = 0;
                    flag = 0;
                }
                else{
                    message[pointer] = ch;
                    pointer ++;
                }
            }
        }
    }
}
