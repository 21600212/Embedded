#include "mbed.h"
#include<stdlib.h>
#include<stdio.h>


#define PWM_MIN 550
#define PWM_MID 1350
#define PWM_MAX 2150

PwmOut servoMotor(D6); //PB_10, D6
UnbufferedSerial pc(PA_2, PA_3, 115200);

char rx_buffer[10];
char tx_buffer[50];
int index = 0;
volatile int flag;

void rx_ISR(void)
{
  char ch;
  pc.read(&ch, 1);
  pc.write(&ch, 1);
  rx_buffer[index++] = ch;
  if(ch == '\r'){
    pc.write("\n", 1);
    rx_buffer[--index] = '\0';
    index = 0;
    flag = 1;
  }
}

void control_servo(){
  sprintf(tx_buffer , "Degree Change Start\r\n");
  pc.write(tx_buffer , strlen(tx_buffer));

  int degree = atoi(rx_buffer);

  degree = 550 + degree * 8;

  sprintf(tx_buffer , "%d\r\n", degree);
  pc.write(tx_buffer , strlen(tx_buffer));

  servoMotor.pulsewidth_us(degree);
}

int main()
{
  sprintf(tx_buffer , "PWM Test Program (Servo Motor)!\r\n");
  pc.write(tx_buffer , strlen(tx_buffer));
  pc.attach(rx_ISR);
  servoMotor.period_ms(20); // PWM period = 20ms
  servoMotor.pulsewidth_us(PWM_MIN); //for 0 degree

  while(1) {
    flag = 0;
    sprintf(tx_buffer , "Enter the rotation degree [0 - 180]: ");
    pc.write(tx_buffer, strlen(tx_buffer));

    while(flag != 1);

    control_servo();
  }
}
