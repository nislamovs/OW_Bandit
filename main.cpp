#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "Configuration/commons.h"

#include "SPI.h"
#include "OneWire.h"
#include "Wire.h"
#include "SoftwareSerial.h"

SoftwareSerial serial(10, 11); // RX, TX
int main(void) {

    Serial.begin(57600);
    while (!Serial) {;} // wait for serial port to connect. Needed for native USB port only
    serial.begin(9600); // set the data rate for the SoftwareSerial port
    OneWire  ds(12);  // digital pin 12

    while(1){
        while(1) {
            byte addr[8] = {0};

            if (!ds.search(addr)) {
//                serial.println("No more addresses.");
                ds.reset_search();
                break;
            }

            for (int i = 0; i < 8; i++) {
                char buffer[2];
                sprintf(buffer, "%02X", addr[i]);
                serial.print(buffer);
            }
            serial.println("");

            if (OneWire::crc8(addr, 7) != addr[7]) {
                serial.println("CRC is not valid!");
                break;
            }

            if (addr[0] != 0x01) {
                serial.println("Device is not a DS1990A family device.");
                break;
            }
            ds.reset();

        }
        _delay_ms(300);
    }

    return 0;
}
