#include "mbed.h"

PwmOut buzzer(PB_3); //D3

// C4, D4, E4, F4, G4, A4, B4, C5
float freq[]={0, 261.626, 293.665, 329.628, 349.228, 391.995, 440.000, 493.883, 523.251, 587.33, 659.26};
int beat[]={4, 4, 4, 4, 4, 4, 4, 4}; //beat array [ 1, ..., 16], 1 beat = 1/16 sec.

int line1[]={0, 3, 3, 4, 5, 6, 7,   9, 8, 8, 7, 8, 5, 5, 5, 5};
int line2[]={5, 6, 7, 7,  5, 4, 3, 2, 2};
int line3[]={0, 3, 3, 4, 5, 5, 6, 7,  9, 8, 9, 10, 10};
int line4[]={0,  0, 9, 6, 8, 7, 9,  0};

int beat1[]={8, 8, 8, 8, 4, 8, 8,   8, 8, 8, 8, 8, 8, 8, 16, 16};
int beat2[]={4, 8, 2, 8,  8, 8, 8, 8, 2};
int beat3[]={8, 8, 8, 8, 8, 8, 8, 8,  8, 8, 8, 8, 2};
int beat4[]={1,  8, 4, 8, 4, 4,  2,  8};

int main() {
    int period_us;
    int beat_ms;

    while(1){
        for(int i=0; i<16; i++){
            if(freq[line1[i]]==0){
                buzzer.suspend();
                beat_ms = 62.5 * beat1[i];
                ThisThread::sleep_for(chrono::milliseconds(beat_ms)); // hold for beat period
                buzzer.resume();
            }
            else{
                buzzer = 1.0 - 0.1; // set duty cycle (1.0 0.1)
                period_us = 1000000/freq[line1[i]];
                buzzer.period_us(period_us); // set PWM period
                beat_ms = 62.5 * beat1[i];
                ThisThread::sleep_for(chrono::milliseconds(beat_ms)); // hold for beat period
            }

            buzzer.suspend();
            ThisThread::sleep_for(chrono::milliseconds(100));
            buzzer.resume();
        }
        for(int i=0; i<9; i++){
            if(freq[line2[i]]==0){
                buzzer.suspend();
                beat_ms = 62.5 * beat2[i];
                ThisThread::sleep_for(chrono::milliseconds(beat_ms)); // hold for beat period
                buzzer.resume();
            }
            else{
                buzzer = 1.0 - 0.1; // set duty cycle (1.0 0.1)
                period_us = 1000000/freq[line2[i]];
                buzzer.period_us(period_us); // set PWM period
                beat_ms = 62.5 * beat2[i];
                ThisThread::sleep_for(chrono::milliseconds(beat_ms)); // hold for beat period
            }

            buzzer.suspend();
            ThisThread::sleep_for(chrono::milliseconds(100));
            buzzer.resume();
        }
        for(int i=0; i<13; i++){
            if(freq[line3[i]]==0){
                buzzer.suspend();
                beat_ms = 62.5 * beat3[i];
                ThisThread::sleep_for(chrono::milliseconds(beat_ms)); // hold for beat period
                buzzer.resume();
            }
            else{
                buzzer = 1.0 - 0.1; // set duty cycle (1.0 0.1)
                period_us = 1000000/freq[line3[i]];
                buzzer.period_us(period_us); // set PWM period
                beat_ms = 62.5 * beat3[i];
                ThisThread::sleep_for(chrono::milliseconds(beat_ms)); // hold for beat period
            }

            buzzer.suspend();
            ThisThread::sleep_for(chrono::milliseconds(100));
            buzzer.resume();
        }
        for(int i=0; i<8; i++){
            if(freq[line4[i]]==0){
                buzzer.suspend();
                beat_ms = 62.5 * beat4[i];
                ThisThread::sleep_for(chrono::milliseconds(beat_ms)); // hold for beat period
                buzzer.resume();
            }
            else{
                buzzer = 1.0 - 0.1; // set duty cycle (1.0 0.1)
                period_us = 1000000/freq[line4[i]];
                buzzer.period_us(period_us); // set PWM period
                beat_ms = 62.5 * beat4[i];
                ThisThread::sleep_for(chrono::milliseconds(beat_ms)); // hold for beat period
            }

            buzzer.suspend();
            ThisThread::sleep_for(chrono::milliseconds(100));
            buzzer.resume();
        }
    }
}
