#include "mbed.h"

PwmOut buzzer(PB_3); //D3

// C4, D4, E4, F4, G4, A4, B4, C5
float freq []={261.626, 293.665, 329.628, 349.228, 391.995, 440.000, 493.883, 523.251};

int beat[]={4, 4, 4, 4, 4, 4, 4, 4}; //beat array [ 1, ..., 16], 1 beat = 1/16 sec.

int main() {
    int period_us;
    int beat_ms;

    while (true) {
        for(int i=0; i<8; i++){
            buzzer = 1.0 - 0.1; // set duty cycle (1.0 0.1)
            period_us = 1000000/freq[i];
            beat_ms = 62.5 * beat[i];
            buzzer.period_us(period_us); // set PWM period
            ThisThread::sleep_for(chrono::milliseconds(beat_ms)); // hold for beat period
            buzzer.suspend();
            ThisThread::sleep_for(chrono::milliseconds(100));
            buzzer.resume();
        }
        for(int i=0; i<8; i++) {
            buzzer = 1.0 - 0.05; // set duty cycle
            period_us = 1000000/freq[7 - i];
            beat_ms = 62.5 * beat[7 - i];
            buzzer.period_us(period_us); // set PWM period
            ThisThread::sleep_for(chrono::milliseconds(beat_ms)); // hold for beat period
            buzzer.suspend();
            ThisThread::sleep_for (chrono::milliseconds(100));
            buzzer.resume();
        }
    }
}
