#include "mbed.h"
#include "Adafruit_SSD1306.h"

Adafruit_SSD1306_Spi gOLED(ARDUINO_UNO_D11, ARDUINO_UNO_D13, ARDUINO_UNO_D10, ARDUINO_UNO_D4, ARDUINO_UNO_D7);

int main()
{
  int tmp;

  ThisThread::sleep_for(chrono::milliseconds(5000));
  gOLED.clearDisplay();

  gOLED.printf("Handong Univ. CSEE\r\n\n\n\n");
  gOLED.printf("Temperature:\r\n\n");
  gOLED.printf("Humadity:");

  while(1)
  {
    gOLED.setTextCursor(13*6, 4*8);
    tmp = rand() % 1000;
    gOLED.printf("%4.1f C", tmp / 10.0);

    gOLED.setTextCursor(13*6, 6*8);
    tmp = rand() % 1000;
    gOLED.printf("%4.1f %%", tmp / 10.0);

    gOLED.display();
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}
