
#include "mbed.h"

UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 230400);
UnbufferedSerial wifi(ARDUINO_UNO_D8, ARDUINO_UNO_D2, 115200);
Thread sock_thread;
char buffer[80];
char buffer_wifi[80];

char command[20];
char command_pwm_out[20];
int pointer = 0;
int flag = 0;

void Char2Hex(unsigned char ch, char* szHex)
{
 static unsigned char saucHex[] = "0123456789ABCDEF";
 szHex[0] = saucHex[ch >> 4];
 szHex[1] = saucHex[ch&0xF];
 szHex[2] = 0;
}


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
    command_pwm_out[0]=0x00;
    command_pwm_out[1]=0x00;
    command_pwm_out[2]=0x00;
    command_pwm_out[3]=0x00;
    command_pwm_out[4]=0x00;
    command_pwm_out[5]=0x06;
    command_pwm_out[6]=0x01;
    command_pwm_out[7]=0x06;
    command_pwm_out[8]=0x00;
    command_pwm_out[9]=0x04;
    command_pwm_out[10]=0x00;
    command_pwm_out[11]=0x01;

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

    char ch_pc;
    char ch_wifi;

    int flag = 0;
    int cnt = 0;

    while(true){

        if(pc.readable()){
            pc.read(&ch_pc, 1);
            pc.write(&ch_pc,1);
            if(ch_pc==0x4D){
                sprintf(buffer, "AT+CIPSEND=%d\r\n",12);
                wifi.write(buffer,strlen(buffer));
                ThisThread::sleep_for(500ms);

                for(int i=0; i<12; ++i){
                    //sprintf(buffer, "%c",command[i]);
                    wifi.write(command_pwm_out+i,1);
                }

                if(command_pwm_out[11]==0x01)
                    command_pwm_out[11]=0x02;
                else if(command_pwm_out[11]==0x02)
                    command_pwm_out[11]=0x00;
                else
                    command_pwm_out[11]=0x01;
            }
            else if(ch_pc==0x4C){
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
                    if(cnt==13) {
                        flag = 0;
                        cnt=0;
                        for(int i=0; i<13; ++i){
                            Char2Hex(buffer_wifi[i], hex);

                            sprintf(buffer, "*%s\n", hex);
                            pc.write(buffer, 5);
                        }
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
