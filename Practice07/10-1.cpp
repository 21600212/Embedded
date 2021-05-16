/**
 * RemoteIR library - Test program.
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */

#include <mbed.h>

#include "ReceiverIR.h"
#include "TransmitterIR.h"

#define TEST_LOOP_BACK  0

#define MAXIMUM_BUFFER_SIZE 80

char buffer [MAXIMUM_BUFFER_SIZE];
BufferedSerial pc(USBTX, USBRX, 115200);

ReceiverIR ir_rx(D4);

static DigitalOut led(PA_5);
Ticker ledTicker;

/**
 * Receive.
 *
 * @param format Pointer to a format.
 * @param buf Pointer to a buffer.
 * @param bufsiz Size of the buffer.
 *
 * @return Bit length of the received data.
 */
int receive(RemoteIR::Format *format, uint8_t *buf, int bufsiz, int timeout = 100) {
    int cnt = 0;
    while (ir_rx.getState() != ReceiverIR::Received) {
        cnt++;
        if (timeout < cnt) {
            return -1;
        }
    }
    return ir_rx.getData(format, buf, bufsiz * 8);
}

/**
 * Transmit.
 *
 * @param format Format.
 * @param buf Pointer to a buffer.
 * @param bitlength Bit length of the data.
 *
 * @return Bit length of the received data.
 */
//int transmit(RemoteIR::Format format, uint8_t *buf, int bitlength, int timeout = 100) {
//    int cnt = 0;
//    while (ir_tx.getState() != TransmitterIR::Idle) {
//        cnt++;
//        if (timeout < cnt) {
//            return -1;
//        }
//    }
//    return ir_tx.setData(format, buf, bitlength);
//}

/**
 * Display a current status.
 */
void display_status(char *status, int bitlength) {
    sprintf(buffer, "%-5.5s:%02d\r\n", status, bitlength);
    pc.write(buffer, strlen(buffer));
}

/**
 * Display a format of a data.
 */
void display_format(RemoteIR::Format format) {
    switch (format) {
        case RemoteIR::UNKNOWN:
            sprintf(buffer, "????????\r\n");
            pc.write(buffer, strlen(buffer));
            break;
        case RemoteIR::NEC:
            sprintf(buffer, "NEC     \r\n");
            pc.write(buffer, strlen(buffer));
            break;
        case RemoteIR::NEC_REPEAT:
            sprintf(buffer, "NEC  (R)\r\n");
            pc.write(buffer, strlen(buffer));
            break;
        case RemoteIR::AEHA:
            sprintf(buffer, "AEHA    \r\n");
            pc.write(buffer, strlen(buffer));
            break;
        case RemoteIR::AEHA_REPEAT:
            sprintf(buffer, "AEHA (R)\r\n");
            pc.write(buffer, strlen(buffer));
            break;
        case RemoteIR::SONY:
            sprintf(buffer, "SONY    \r\n");
            pc.write(buffer, strlen(buffer));
            break;
    }
}

/**
 * Display a data.
 *
 * @param buf Pointer to a buffer.
 * @param bitlength Bit length of a data.
 */
void display_data(uint8_t *buf, int bitlength) {
    const int n = bitlength / 8 + (((bitlength % 8) != 0) ? 1 : 0);
    for (int i = 0; i < n; i++) {
        sprintf(buffer, "%02X\r\n", buf[i]);
        pc.write(buffer, strlen(buffer));
    }
    for (int i = 0; i < 8 - n; i++) {
        sprintf(buffer, "--\r\n");
        pc.write(buffer, strlen(buffer));
    }
}

void ledfunc(void) {
    led = led + 1;
}

/**
 * Entry point.
 */
int main(void) {

    ledTicker.attach(&ledfunc, 0.5);

    /*
     * Splash.
     */

    sprintf(buffer, "RemoteIR        \r\n");
    pc.write(buffer, strlen(buffer));
    sprintf(buffer, "Program example.\r\n");
    pc.write(buffer, strlen(buffer));

    ThisThread::sleep_for(chrono::milliseconds(3000));

    /*
     * Initialize.
     */

    sprintf(buffer, "Press a button  \r\n");
    pc.write(buffer, strlen(buffer));
    sprintf(buffer, "on a controller.\r\n");
    pc.write(buffer, strlen(buffer));

    /*
     * Execute.
     */
    while (1) {
        uint8_t buf1[32];
        uint8_t buf2[32];
        int bitlength1;
        int bitlength2;
        RemoteIR::Format format;

        memset(buf1, 0x00, sizeof(buf1));
        memset(buf2, 0x00, sizeof(buf2));

        {
            bitlength1 = receive(&format, buf1, sizeof(buf1));
            if (bitlength1 < 0) {
                continue;
            }
            display_status("RECV", bitlength1);
            display_data(buf1, bitlength1);
            display_format(format);
        }

#if TEST_LOOP_BACK
//        wait_ms(100);
//
//        {
//            bitlength1 = transmit(format, buf1, bitlength1);
//            if (bitlength1 < 0) {
//                continue;
//            }
//            display_status("TRAN", bitlength1);
//            display_data(buf1, bitlength1);
//            display_format(format);
//        }

        ThisThread::sleep_for(chrono::milliseconds(100));

        {
            bitlength2 = receive(&format, buf2, sizeof(buf2));
            if (bitlength2 < 0) {
                continue;
            }
            display_status("RECV", bitlength2);
            display_data(buf2, bitlength2);
            display_format(format);
        }
        \
    \
        ThisThread::sleep_for(chrono::milliseconds(100));

        {
            for (int i = 0; i < sizeof(buf1); i++) {
                if (buf1[i] != buf2[i]) {
                    display_status("CPERR", bitlength2);
                    ThisThread::sleep_for(chrono::milliseconds(1000));
                    continue;
                }
            }
        }
#endif
    }
}
