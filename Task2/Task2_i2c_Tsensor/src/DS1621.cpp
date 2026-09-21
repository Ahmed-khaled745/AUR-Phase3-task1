#include "DS1621.h"

DS1621::DS1621(uint8_t address) : _addr(address)
{
}

void DS1621::begin()
{
    Wire.begin();
    Wire.setClock(100000);
}

DS1621_Status DS1621::writeCommand(uint8_t cmd) // start convert T
{
    Wire.beginTransmission(_addr);
    Wire.write(cmd);
    return (DS1621_Status)(Wire.endTransmission());
}

DS1621_Status DS1621::writeRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.write(value);
    return (DS1621_Status)(Wire.endTransmission());
}

DS1621_Status DS1621::readRegister(uint8_t reg, uint8_t *buf, uint8_t len)
{
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    DS1621_Status status = (DS1621_Status)(Wire.endTransmission(false));
    if (status != DS1621_OK)
        return status;

    Wire.requestFrom(_addr, len);
    for (uint8_t i = 0; i < len; i++)
    {
        if (Wire.available())
            buf[i] = Wire.read();
        else
            return DS1621_ERR_TIMEOUT;
    }
    return DS1621_OK;
}

DS1621_Status DS1621::configure(bool oneShot)
{
    uint8_t cfg = oneShot ? DS1621_CFG_1SHOT : 0;
    DS1621_Status s = writeRegister(DS1621_CMD_ACCESS_CFG, cfg);
    if (s != DS1621_OK)
        return s;
    return DS1621_OK;
}

DS1621_Status DS1621::startConversion()
{
    return writeCommand(DS1621_CMD_START_CONV);
}

DS1621_Status DS1621::readRawTemperature(uint8_t &msb, uint8_t &lsb)
{
    uint8_t buf[2];
    DS1621_Status s = readRegister(DS1621_CMD_READ_TEMP, buf, 2);
    if (s != DS1621_OK)
        return s;
    msb = buf[0];
    lsb = buf[1];
    return DS1621_OK;
}

float DS1621::parseTemperature(uint8_t msb, uint8_t lsb)
{
    int16_t raw = (msb << 8) | lsb;
    return raw / 256.0;
}
DS1621_Status DS1621::readTemperature(float &celsius)
{
    uint8_t msb, lsb;
    DS1621_Status s = readRawTemperature(msb, lsb);
    if (s != DS1621_OK)
        return s;
    celsius = parseTemperature(msb, lsb);
    return DS1621_OK;
}

const char *DS1621::statusToString(DS1621_Status s)
{
    switch (s)
    {
    case DS1621_OK:
        return "OK";
    case DS1621_ERR_DATA_TOO_LONG:
        return "Data too long";
    case DS1621_ERR_ADDR_NACK:
        return "Address NACK";
    case DS1621_ERR_DATA_NACK:
        return "Data NACK";
    case DS1621_ERR_OTHER:
        return "Other error";
    case DS1621_ERR_TIMEOUT:
        return "Timeout";
    default:
        return "Unknown error";
    }
}
