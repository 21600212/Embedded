#include "mbed.h"

DigitalOut trigger(ARDUINO_UNO_D3);
DigitalIn echo(ARDUINO_UNO_D2);
Timer timer1;

DigitalOut led1(LED1);
PwmOut buzzer(ARDUINO_UNO_D4); //D3

BufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);
char buffer[80];

void my_wait_us (uint32_t delay)
{
  timer1.reset();
  timer1.start();
  while (timer1.elapsed_time().count() < delay) { }
  timer1.stop();
  timer1.reset();
}

int main()
{
  float distance;

  int period_us;
  int beat_ms;

  sprintf(buffer, "\r\nWelcome to Utrasonic Sensor Lab.!\r\n");
  pc.write(buffer, strlen(buffer));

  // initialization
  trigger = 0;

  while(1) {
    // trigger sonar to send a ping
    timer1.reset();
    trigger = 1;
    my_wait_us(15);
    trigger = 0;
    led1 = 1;

    while (echo == 0) {} //wait for echo high
    timer1.start(); //when echo high, start timer
    while (echo == 1) {} //wait for echo low
    timer1.stop(); //stop timer
    led1 = 0;

    distance = timer1.elapsed_time().count() * 0.017;
    sprintf(buffer, " The distance is %.2f [cm] \n\r", distance);
    pc.write(buffer, strlen(buffer));

    if(distance < 100){
        buzzer = 1.0 - 0.1; // set duty cycle (1.0 0.1)
        period_us = 1000000/261.626;
        beat_ms = 62.5 * (distance+7)/7;
        buzzer.period_us(period_us);
        ThisThread::sleep_for(chrono::milliseconds(100)); // hold for beat period
        buzzer.suspend();
        ThisThread::sleep_for (chrono::milliseconds(beat_ms));
        buzzer.resume();
    }
    else
        buzzer.suspend();

    ThisThread::sleep_for(chrono::milliseconds(10)); // 5sec
  }
}
