#include "mbed.h"
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>

UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 230400);
UnbufferedSerial wifi(ARDUINO_UNO_D8, ARDUINO_UNO_D2, 115200);
Thread sock_thread;
char buffer[80];
char buffer_wifi[80];

char command_out[20];
char command_in[20];
int pointer = 0;
int flag = 0;
void check_status (){


}

void Char2Hex(unsigned char ch, char* szHex)
{
 static unsigned char saucHex[] = "0123456789ABCDEF";
 szHex[0] = saucHex[ch >> 4];
 szHex[1] = saucHex[ch&0xF];
 szHex[2] = 0;
}

void led_on(){
    command_out[9]=0x13;
    command_out[10]=0x00;

    sprintf(buffer, "AT+CIPSEND=%d\r\n",12);
    wifi.write(buffer,strlen(buffer));
    ThisThread::sleep_for(500ms);

    for(int i=0; i<12; ++i)
        wifi.write(command_out+i,1);
}

void led_off(){
    command_out[9]=0x13;
    command_out[10]=0xFF;

    sprintf(buffer, "AT+CIPSEND=%d\r\n",12);
    wifi.write(buffer,strlen(buffer));
    ThisThread::sleep_for(500ms);

    for(int i=0; i<12; ++i)
        wifi.write(command_out+i,1);
}

int main(){
    sprintf(buffer, "AT+CWMODE=1\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(3000ms);

    sprintf(buffer, "AT+CWJAP=\"tp_4ahnssu\",\"01051611257a\"\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(10000ms);

    sprintf(buffer, "AT+CIPMUX=0\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(3000ms);

    sprintf(buffer, "AT+CIPSTART=\"TCP\",\"192.168.0.200\",502\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(10000ms);

    sprintf(buffer, "AT+CIPSTATUS\r\n");
    wifi.write(buffer, strlen(buffer));
    ThisThread::sleep_for(3000ms);


    command_out[0]=0x00;
    command_out[1]=0x00;
    command_out[2]=0x00;
    command_out[3]=0x00;
    command_out[4]=0x00;
    command_out[5]=0x06;
    command_out[6]=0x01;
    command_out[7]=0x05;
    command_out[8]=0x00;
    command_out[9]=0x11;
    command_out[10]=0xFF;
    command_out[11]=0x00;

    command_in[0]=0x00;
    command_in[1]=0x00;
    command_in[2]=0x00;
    command_in[3]=0x00;
    command_in[4]=0x00;
    command_in[5]=0x06;
    command_in[6]=0x01;
    command_in[7]=0x04;
    command_in[8]=0x00;
    command_in[9]=0x00;
    command_in[10]=0x00;
    command_in[11]=0x02;

    char ch_pc;
    char ch_wifi;

    int flag = 0;
    int cnt = 0;

    sprintf(buffer, "\r\n *****key <---> Wifi *****\r\n");
    pc.write(buffer, strlen(buffer));
    sprintf(buffer, "\r\nTCP and UDP Operation Using AT Commands\r\n");
    pc.write(buffer, strlen(buffer));

    sock_thread.start(&check_status);

    while(true){

        if(pc.readable()){
            pc.read(&ch_pc, 1);
            pc.write(&ch_pc,1);
            if(ch_pc==0x49){
                sprintf(buffer, "AT+CIPSEND=%d\r\n",12);
                wifi.write(buffer,strlen(buffer));
                ThisThread::sleep_for(500ms);

                for(int i=0; i<12; ++i){
                    //sprintf(buffer, "%c",command[i]);
                    wifi.write(command_in+i,1);
                }
            }
            else if(ch_pc==0x4F){
                command_out[9]=0x12;

                sprintf(buffer, "AT+CIPSEND=%d\r\n",12);
                wifi.write(buffer,strlen(buffer));
                ThisThread::sleep_for(500ms);

                for(int i=0; i<12; ++i){
                    //sprintf(buffer, "%c",command[i]);
                    wifi.write(command_out+i,1);
                }

                if(command_out[10]==0xFF)
                    command_out[10]=0x00;
                else
                    command_out[10]=0xFF;
            }
            else if(ch_pc==0x32){
                command_out[9]=0x12;

                sprintf(buffer, "AT+CIPSEND=%d\r\n",12);
                wifi.write(buffer,strlen(buffer));
                ThisThread::sleep_for(500ms);

                for(int i=0; i<12; ++i){
                    //sprintf(buffer, "%c",command[i]);
                    wifi.write(command_out+i,1);
                    pc.write(command_out+i,1);
                }

                if(command_out[10]==0xFF)
                    command_out[10]=0x00;
                else
                    command_out[10]=0xFF;
            }
            else{
                //wifi.write(&ch_pc,1);
            }
        }
        if(wifi.readable()){
            wifi.read(&ch_wifi,1);
            if(ch_wifi==0x32)
                flag=1;
            if(flag){
                if(ch_wifi == 0x3A||cnt>0){

                    buffer_wifi[cnt] = ch_wifi;

                    char hex[3];

                    cnt++;
                    if(cnt==14) {
                        flag = 0;
                        cnt=0;
                        for(int i=0; i<14; ++i){
                            Char2Hex(buffer_wifi[i], hex);

                            sprintf(buffer, "*%s\n", hex);
                            pc.write(buffer, 5);
                        }
                        if(buffer_wifi[10] > 0x08)
                            led_on();
                        else
                            led_off();
                    }
                }
                else
                    pc.write(&ch_wifi,1);
            }
            else
                pc.write(&ch_wifi,1);
        }
    }
}
