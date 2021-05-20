#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE 80
char buffer [MAXIMUM_BUFFER_SIZE];

#define HTU21D_TEMP_READ (0xF3) // no hold command
#define HTU21D_HUMI_READ (0xF5)
#define HTU21G_USER_WRITE (0xE6)
#define HTU21D_ADDR (0x40)

//I2C i2c(PB_9_ALT0, PB_10); //D14,D6(PB_10)
I2C i2c(PB_9, PB_8); //D14, D15 (PB_8)
BufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);


float sample_ctemp;
float sample_humid;

int main() {
    while(1) {
        char tx[1];
        char rx[2];

        tx[0] = HTU21D_TEMP_READ; // Triggers a temperature measure by feeding correct opcode.
        i2c.write((HTU21D_ADDR << 1) & 0xFE, tx, 1);
        ThisThread::sleep_for(50); // Per datasheet, wait long enough for device to sample temperature

        // Reads triggered measure
        i2c.read((HTU21D_ADDR << 1) | 0x01, rx, 2);
        ThisThread::sleep_for(1);

        // Algorithm from datasheet to compute temperature.
        unsigned int rawTemperature = ((unsigned int) rx[0] << 8) | (unsigned int) rx[1];
        rawTemperature &= 0xFFFC;

        float tempTemperature = rawTemperature / (float)65536; //2^16 = 65536
        float realTemperature = -46.85 + (175.72 * tempTemperature); //From page 14

        sample_ctemp = realTemperature;


        tx[0] = HTU21D_HUMI_READ; // Triggers a humidity measure by feeding correct opcode.
        i2c.write((HTU21D_ADDR << 1) & 0xFE, tx, 1);
        ThisThread::sleep_for(16); // Per datasheet, wait long enough for device to sample humidity

        // Reads triggered measure
        i2c.read((HTU21D_ADDR << 1) | 0x01, rx, 2);
        ThisThread::sleep_for(1);

        //Algorithm from datasheet.
        unsigned int rawHumidity = ((unsigned int) rx[0] << 8) | (unsigned int) rx[1];

        rawHumidity &= 0xFFFC; //Zero out the status bits but keep them in place

        //Given the raw humidity data, calculate the actual relative humidity
        float tempRH = rawHumidity / (float)65536; //2^16 = 65536
        float rh = -6 + (125 * tempRH); //From page 14

        sample_humid = rh;


        sprintf(buffer, "Temperature: %.2f C\n\r", sample_ctemp);
        pc.write(buffer, strlen(buffer));
        sprintf(buffer, "Humidity: %.2f [%%]\n\r", sample_humid);
        pc.write(buffer, strlen(buffer));
        printf("\n\r");

        ThisThread::sleep_for(3000);
    }
}
