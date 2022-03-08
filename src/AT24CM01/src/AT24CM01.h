/*************************************************** 
  Arduino library for the AT24CM01
  
  Author: Jonathan Dempsey JDWifWaf@gmail.com
  
  Version: 1.0.1
  License: Apache 2.0
 ****************************************************/

#ifndef _AT24CM01_H
#define _AT24CM01_H

#include <Arduino.h>

#define ATWDelay 5                   // Delay to allow write to be completed (datasheet shows max 5ms)
#define AT2CSPEED 400000             // Clock speed for I2C
#define ATMAXADR 131072              // Size of EEPROM in words

#ifndef JOURNAL_EEPROM_I2C_ADDRESS
    #define ATDEVADR 0x52                // Lowest page address
#else
    #define ATDEVADR JOURNAL_EEPROM_I2C_ADDRESS
#endif

class AT24CM01
{
    public:
        void begin(TwoWire *inWire);
        uint32_t read(uint32_t address, uint8_t size = 1);
        void read(uint32_t address, uint8_t data[], uint8_t indexCount);
        
        void write(uint32_t address, uint32_t data);
        void write(uint32_t address, uint32_t data, uint8_t size);
        void write(uint32_t address, uint8_t data[], uint8_t indexCount);
        
        /* Variable Specific Functions conversions ................................................ */

        /* ------------------------ Write --------------------- */
        inline void writeBool(uint32_t a, bool b) {
            write((uint32_t)a, (uint32_t)b, sizeof(bool));
        }
        inline void writeCharArray(uint32_t a, char c[], uint8_t i) {
            write((uint32_t)a, (uint8_t*)c, (uint8_t)i);
        }
        inline void writeByteArray(uint32_t a, byte y[], uint8_t i) {
            write((uint32_t)a, (uint8_t*)y, (uint8_t)i);
        }
        inline void writeByte(uint32_t a, byte y) {
            write((uint32_t)a, (uint32_t)y, sizeof(byte)); 
        }
        inline void writeChar(uint32_t a, char c) { 
            write((uint32_t)a, (uint32_t)c, sizeof(char));
        }         
        inline void writeUnsignedChar(uint32_t a, unsigned char uc) { 
            write((uint32_t)a, (uint32_t)uc, sizeof(unsigned char));
        } 
        inline void writeShort(uint32_t a, short l) { 
            write((uint32_t)a, (uint32_t)l, sizeof(short));
        }
        inline void writeLong(uint32_t a, long l) { 
            write((uint32_t)a, (uint32_t)l, sizeof(long));
        }
        inline void writeUnsignedLong(uint32_t a, unsigned long ul) { 
            write((uint32_t)a, (uint32_t)ul, sizeof(unsigned long)); 
        }
        inline void writeInt(uint32_t a, unsigned int i) {
            write((uint32_t)a, (uint32_t)i, sizeof(unsigned int));
        }
        inline void writeUnsignedInt(uint32_t a, unsigned int ui) {
            return write((uint32_t)a, (uint32_t)ui, sizeof(unsigned int));
        }
        inline void writeWord(uint32_t a, word ui) {
            return write((uint32_t)a, (uint32_t)ui, sizeof(word));
        }
        inline void writeDouble(uint32_t a, double d) {
            mkconv.dval = d;
            write((uint32_t)a, (uint8_t*)mkconv.bval, sizeof(double));
        }
        inline void writeFloat(uint32_t a, float f) {
            mkconv.fval = f;
            write((uint32_t)a, (uint8_t*)mkconv.bval, sizeof(float));
        }
       /* ------------------------ Read --------------------- */

        inline bool readBool(uint32_t a) {
            return (bool)read((uint32_t)a, sizeof(bool));
        }
        inline void readCharArray(uint32_t a, char c[], uint8_t i) {
            read((uint32_t)a, (uint8_t*)c, (uint8_t)i); 
        }
        inline void readByteArray(uint32_t a, uint8_t y[], uint8_t i) {
            read((uint32_t)a, (uint8_t*)y, (uint8_t)i);
        }
        inline byte readByte(uint32_t a) {
            return (byte)read((uint32_t)a, sizeof(byte)); 
        }
        inline char readChar(uint32_t a) {
            return (char)read((uint32_t)a, sizeof(char));
        }
        inline unsigned char readUnsignedChar(uint32_t a) { 
            return (unsigned char)read((uint32_t)a, sizeof(unsigned char));
        } 
        inline short readShort(uint32_t a) { 
            return (short)read((uint32_t)a, sizeof(short));
        }
        inline long readLong(uint32_t a) { 
            return (long)read((uint32_t)a, sizeof(long));
        }
        inline unsigned long readUnsignedLong(uint32_t a) { 
            return (unsigned long)read((uint32_t)a, sizeof(unsigned long)); 
        }
        inline int readInt(uint32_t a) {
            return (int)read((uint32_t)a, sizeof(unsigned int));
        }
        inline unsigned int readUnsignedInt(uint32_t a) {
            return (unsigned int)read((uint32_t)a, sizeof(unsigned int));
        }
        inline word readword(uint32_t a) {
            return (word)read((uint32_t)a, sizeof(word));
        }
        inline double readDouble(uint32_t a) {
            read((uint32_t)a, (uint8_t*)mkconv.bval, sizeof(double));
            return mkconv.dval;
        }
        inline float readFloat(uint32_t a) {
            read((uint32_t)a, (uint8_t*)mkconv.bval, sizeof(float));
            return mkconv.fval;
        }

    protected:
    private:
        uint8_t _eepromAddr;
        TwoWire* ATWire;             // TwoWire library instance
        union conv {
                float fval;
                double dval;
                uint8_t bval[sizeof(float)];
        } mkconv;
};

#endif
