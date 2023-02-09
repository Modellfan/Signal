#include "signals.h"
#include <Arduino.h>
#include <ACAN_T4.h>

// https://github.com/FrankBoesing/FastCRC

Signal::Signal(const char *name, const uint8_t start_bit, const uint8_t len, const float scale,
               const float offset, const sigEndianess endianess, const sigSign sign, const char *unit,
               float aliveTimeout, float defaultValue, float minValue, float maxValue)
    : _name(name),
      _start_bit(start_bit),
      _bit_len(len),
      _scale(scale),
      _offset(offset),
      _endianess(endianess),
      _sign(sign),
      _unit(unit),
      _aliveTimeout(aliveTimeout),
      _defaultValue(defaultValue),
      _minValue(minValue),
      _maxValue(maxValue)
{
    _value = _defaultValue;
    _wasEverReceived = false;
}

void Signal::update(const CANMessage &msg)
{
    uint32_t bits = 0;
    if (_endianess == sigEndianess::sigBIG_ENDIAN)
    {
        // for (unsigned int i = 0; i < _bit_len / 8; i++)
        // {
        //     bits |= (uint32_t)(msg.data[(_start_bit + i) / 8] << ((_start_bit + i) % 8));
        // }
    }
    else
    {
        //Little endian. Only full bytes supported yet.
        for (unsigned int i = 0; i < _bit_len / 8; i++)
        {
            bits |= (uint32_t)(msg.data[(_start_bit  / 8 + i)] << (i * 8));
        }
    }

    // if (_bit_len % 8)
    //     bits |= (uint32_t)((msg.data[(_start_bit + _bit_len / 8)] & ((1 << (_bit_len % 8)) - 1)) << (_bit_len - _bit_len % 8));

    if (_sign == sigSign::sigUNSIGNED)
    {
        _value = (bits * _scale + _offset);
  
    }
    else
    {
        int32_t signed_bits = *((int32_t *)&bits);
        _value = (signed_bits * _scale + _offset);
    
    }
}

void Signal::print()
{
    Serial.print(_name);
    Serial.print(": ");
    Serial.print(_value);
    Serial.print(" ");
    Serial.print(_unit);
    Serial.print(" | was ever received: ");
    Serial.println(_wasEverReceived ? "yes" : "no");
}
