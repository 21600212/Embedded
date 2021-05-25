#include "mbed.h"
#include "Adafruit_SSD1306.h"

I2C i2c(PB_9, PB_8); //D14, D15 (PB_8)

Adafruit_SSD1306_Spi gOLED(ARDUINO_UNO_D11, ARDUINO_UNO_D13, ARDUINO_UNO_D10, ARDUINO_UNO_D4, ARDUINO_UNO_D7);

#define MAXIMUM_BUFFER_SIZE 80
char buffer [MAXIMUM_BUFFER_SIZE];

#define HTU21D_TEMP_READ (0xF3) // no hold command
#define HTU21D_HUMI_READ (0xF5)
#define HTU21G_USER_WRITE (0xE6)
#define HTU21D_ADDR (0x40)

//I2C i2c(PB_9_ALT0, PB_10); //D14,D6(PB_10)

BufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);


float sample_ctemp;
float sample_humid;

unsigned int crc;

int main() {

    // ThisThread::sleep_for(chrono::milliseconds(5000));
    // gOLED.clearDisplay();
    //
    // gOLED.printf("Handong Univ. CSEE\r\n\n\n\n");
    // gOLED.printf("Temperature:\r\n\n");
    // gOLED.printf("Humadity:");

    while(1) {
        char tx[1];
        char rx[3];

        tx[0] = HTU21D_TEMP_READ; // Triggers a temperature measure by feeding correct opcode.
        i2c.write((HTU21D_ADDR << 1) & 0xFE, tx, 1);
        ThisThread::sleep_for(chrono::milliseconds(50)); // Per datasheet, wait long enough for device to sample temperature

        // Reads triggered measure
        i2c.read((HTU21D_ADDR << 1) | 0x01, rx, 2);
        ThisThread::sleep_for(chrono::milliseconds(1));

        // Algorithm from datasheet to compute temperature.
        unsigned int rawTemperature = ((unsigned int) rx[0] << 8) | (unsigned int) rx[1];
        rawTemperature &= 0xFFFC;

        float tempTemperature = rawTemperature / (float)65536; //2^16 = 65536
        float realTemperature = -46.85 + (175.72 * tempTemperature); //From page 14

        sample_ctemp = realTemperature;


        tx[0] = HTU21D_HUMI_READ; // Triggers a humidity measure by feeding correct opcode.
        i2c.write((HTU21D_ADDR << 1) & 0xFE, tx, 1);
        ThisThread::sleep_for(chrono::milliseconds(16)); // Per datasheet, wait long enough for device to sample humidity

        // Reads triggered measure
        i2c.read((HTU21D_ADDR << 1) | 0x01, rx, 3);
        ThisThread::sleep_for(chrono::milliseconds(1));

        //Algorithm from datasheet.
        unsigned int rawHumidity = ((unsigned int) rx[0] << 8) | (unsigned int) rx[1];

        rawHumidity &= 0xFFFC; //Zero out the status bits but keep them in place

        //Given the raw humidity data, calculate the actual relative humidity
        float tempRH = rawHumidity / (float)65536; //2^16 = 65536
        float rh = -6 + (125 * tempRH); //From page 14

        sample_humid = rh;

        crc = (unsigned int) rx[2];


        sprintf(buffer, "Temperature: %.2f C\n\r", sample_ctemp);
        pc.write(buffer, strlen(buffer));
        sprintf(buffer, "Humidity: %.2f [%%]\n\r", sample_humid);
        pc.write(buffer, strlen(buffer));
        sprintf(buffer, "CRC: 0x%lx + 0x%lx / 0x%lx = 0x%lx\n\r", rx[0], rx[1], rx[2], ((rx[0] << 8)|rx[1])%rx[2]);
        pc.write(buffer, strlen(buffer));
        sprintf(buffer, "CRC check: %u / %u = %u  \n\r", rawHumidity, crc, rawHumidity%crc);
        pc.write(buffer, strlen(buffer));
        printf("\n\r");

        // gOLED.setTextCursor(13*6, 4*8);
        //
        // gOLED.printf("%4.1f C", sample_ctemp);
        //
        // gOLED.setTextCursor(13*6, 6*8);
        //
        // gOLED.printf("%4.1f %%", sample_humid);
        //
        // gOLED.display();

        ThisThread::sleep_for(chrono::milliseconds(3000));
    }
}
