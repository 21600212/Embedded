#include "mbed.h"

UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 230400);
UnbufferedSerial wifi(ARDUINO_UNO_D8, ARDUINO_UNO_D2, 115200);
Thread sock_thread;
char buffer[80];
char buffer_wifi[80];

int pointer = 0;
int flag = 0;

char traffic_light[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x05, 0x00, 0x11, 0xFF, 0x00};
char barricade[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x06, 0x00, 0x04, 0x00, 0x01};

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

void turn_on(){
    traffic_light[10]=0xFF;

    sprintf(buffer, "AT+CIPSEND=%d\r\n",12);
    wifi.write(buffer,strlen(buffer));
    ThisThread::sleep_for(500ms);

    for(int i=0; i<12; ++i)
        wifi.write(traffic_light+i,1);

    ThisThread::sleep_for(500ms);
}

void turn_off(){
    traffic_light[10]=0x00;

    sprintf(buffer, "AT+CIPSEND=%d\r\n",12);
    wifi.write(buffer,strlen(buffer));
    ThisThread::sleep_for(500ms);

    for(int i=0; i<12; ++i)
        wifi.write(traffic_light+i,1);

    ThisThread::sleep_for(500ms);
}


void change_light(int led){
    if(led){
        traffic_light[9]=0x14; // red light
        turn_off(); // turn off

        traffic_light[9]=0x12; // green light
        turn_on(); // turn on
    }

    else{
        traffic_light[9]=0x12; // green light
        turn_off(); // turn off

        traffic_light[9]=0x13; // yellow light
        turn_on(); // turn on

        traffic_light[9]=0x13; // yellow light
        turn_off(); // turn off

        traffic_light[9]=0x14; // red light
        turn_on(); // turn on
    }
}

void change_barricade(int block){
    if(block)
        barricade[11]=0x02;
    else
        barricade[11]=0x01;

    sprintf(buffer, "AT+CIPSEND=%d\r\n",12);
    wifi.write(buffer,strlen(buffer));
    ThisThread::sleep_for(500ms);

    for(int i=0; i<12; ++i)
        wifi.write(barricade+i,1);
}

int main(){
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

    int curr_status = 1;

    while(true){

        if(pc.readable()){
            pc.read(&ch_pc, 1);
            pc.write(&ch_pc,1);
            if(ch_pc==0x4D){
                if(curr_status){
                    change_light(1);
                    ThisThread::sleep_for(200ms);
                    change_barricade(0);
                    curr_status = 0;
                }
                else{
                    change_light(0);
                    ThisThread::sleep_for(200ms);
                    change_barricade(1);
                    curr_status = 1;
                }
            }
            else if(ch_pc==0x4C){
                traffic_light[9]=0x12;

                sprintf(buffer, "AT+CIPSEND=%d\r\n",12);
                wifi.write(buffer,strlen(buffer));
                ThisThread::sleep_for(500ms);

                for(int i=0; i<12; ++i){
                    wifi.write(traffic_light+i,1);
                }

                if(traffic_light[10]==0xFF)
                    traffic_light[10]=0x00;
                else
                    traffic_light[10]=0xFF;
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
