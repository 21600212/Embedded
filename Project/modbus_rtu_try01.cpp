#include "mbed.h"

UnbufferedSerial modbus(PC_6,PC_7);
//Serial modbus(CONSOLE_TX, CONSOLE_RX);       //tx, rx
DigitalOut modbus_transmit(PA_3, 0);

char modbus_buffer_char[20];
uint8_t                modbus_input_buffer[12];
volatile uint8_t    modbus_input_buffer_counter = 0;
volatile bool       modbus_interrupt_complete = false;

uint32_t modbus_result;
char datam[8] = {0x01,0x03,0x00,0x00,0x00,0x02,0xC4,0x0B};
char datam2[8] = {0x01,0x03, 0x80,0x0A, 0x00, 0x01, 0x8D, 0xC8} ;


uint16_t modbus_crc(uint8_t* buf, int len)
{
    uint16_t crc = 0xFFFF;

    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];          // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--) {    // Loop over each bit
            if ((crc & 0x0001) != 0) {      // If the LSB is set
                crc >>= 1;                    // Shift right and XOR 0xA001
                crc ^= 0xA001;
            } else                          // Else LSB is not set
                crc >>= 1;                    // Just shift right
        }
    }
    // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return crc;
}

/*  ISR for MODBUS  */
void modbus_rx_isr()
{
    if (modbus.readable()) {
        modbus.read(&modbus_buffer_char, 1);
        if (modbus_input_buffer_counter == 0 && modbus_buffer_char[0] == 0x00) {
            modbus_input_buffer_counter = 0;
        } else {
            modbus_input_buffer[modbus_input_buffer_counter] = modbus_buffer_char[0];
            modbus_input_buffer_counter++;
        }
    }

    if (modbus_input_buffer_counter > modbus_input_buffer[2] + 4) {
        modbus_interrupt_complete = true;
        modbus_input_buffer_counter = 0;
    }
}

void modbus_read_L1V(uint8_t slave_address)
{
    uint8_t L1V[8] = {slave_address, 0x03, 0x00, 0x02, 0x00, 0x02, 0x65, 0xCB};

    //L1V[6] = modbus_crc(L1V,6) & 0xFF;
    //L1V[7] = (modbus_crc(L1V,6)>>8) & 0xFF;

    modbus_transmit = 1;
    //wait_ms(5);

    for (uint8_t i = 0; i < 8; i++) {
        modbus.write(L1V+i,1);
        //printf("%x", L1V[i]);
    }

    ThisThread::sleep_for(10ms);
    modbus_transmit = 0;
}



int main()
{
    modbus.baud(9600);
    modbus.format(8, SerialBase::None, 2);
    //printf("start\n\r");
    //modbus.attach(&modbus_rx_isr, Serial::RxIrq);
    //modbus_transmit = 1;

    while(1) {

        modbus_read_L1V(0x01);
        //modbus.write(modbus_input_buffer[0], sizeof(modbus_input_buffer[0]), modbus_read_L1V(0x11) , SERIAL_EVENT_TX_COMPLETE);
        /*
        if (modbus_interrupt_complete) {

            modbus_input_buffer_counter = 0;
            modbus_interrupt_complete = false;



            if (modbus_input_buffer[2] == 2) {
                uint16_t l1v1;
                l1v1 = (modbus_input_buffer[3] << 8) | modbus_input_buffer[4];
                memset (modbus_input_buffer, 0, sizeof(modbus_input_buffer));
                //printf("start\n\r");
                //printf("\r\n%.1f\r\n", l1v1*0.001);
                l1v1=0;

            } else if (modbus_input_buffer[2] == 4) {
                uint32_t l1v2;
                l1v2 = (modbus_input_buffer[3] << 24) | (modbus_input_buffer[4] << 16) | (modbus_input_buffer[5] << 8) | modbus_input_buffer[6];
                memset (modbus_input_buffer, 0, sizeof(modbus_input_buffer));
                //printf("start\n\r");
                //printf("%.1f\n\r", l1v2*0.001);
                l1v2=0;
            }
        }
        */
        ThisThread::sleep_for(2000ms);
    }
}
