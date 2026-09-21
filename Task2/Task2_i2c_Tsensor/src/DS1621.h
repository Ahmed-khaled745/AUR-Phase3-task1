#ifndef DS1621_H
#define DS1621_H

#include <Arduino.h>
#include <Wire.h>

#define DS1621_CMD_READ_TEMP 0xAA
#define DS1621_CMD_ACCESS_CFG 0xAC
#define DS1621_CMD_START_CONV 0xEE
#define DS1621_CMD_STOP_CONV 0x22

#define DS1621_CFG_1SHOT 0x01
#define DS1621_CFG_DONE 0x80

enum DS1621_Status
{
    DS1621_OK = 0,
    DS1621_ERR_DATA_TOO_LONG = 1,
    DS1621_ERR_ADDR_NACK = 2,
    DS1621_ERR_DATA_NACK = 3,
    DS1621_ERR_OTHER = 4,
    DS1621_ERR_TIMEOUT = 5
};

class DS1621
{
public:
    explicit DS1621(uint8_t address = 0x48);

    // Req 1: init I2C + wrapper functions returning error codes
    void begin();
    DS1621_Status writeCommand(uint8_t cmd);
    DS1621_Status writeRegister(uint8_t reg, uint8_t value);
    // Req 4 + 5: register read using Repeated Start, multi-byte
    DS1621_Status readRegister(uint8_t reg, uint8_t *buf, uint8_t len);

    // Req 2: configuration
    DS1621_Status configure(bool oneShot);

    // Req 3: start conversion
    DS1621_Status startConversion();
    // DS1621_Status waitForConversion(uint16_t timeoutMs = 1000);

    // Req 5 + 6: raw payload and parsed value
    DS1621_Status readRawTemperature(uint8_t &msb, uint8_t &lsb);
    DS1621_Status readTemperature(float &celsius);
    static float parseTemperature(uint8_t msb, uint8_t lsb);

    static const char *statusToString(DS1621_Status s);

private:
    uint8_t _addr;
};

#endif