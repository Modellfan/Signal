#pragma once
#include <ACAN_T4.h>
#include <baseSignal.h>
#include <Arduino.h>

// SG_ CellVoltage0 : 0|16@1+ (0.001,0) [0|5] "V" BMS
//@0 = Big-Endian
//@1 = Little Endian
//+ signed value
//- unsigned value

enum class sigEndianess
{
    sigBIG_ENDIAN,
    sigLITTLE_ENDIAN
};
enum class sigSign
{
    sigSIGNED,
    sigUNSIGNED
};

template <typename T>
class CANSignal : public Signal<T>
{
public:
    CANSignal(const char *name, const char *unit, u_int32_t aliveTimeout, T defaultValue, T minValue, T maxValue,
              const uint8_t start_bit, const uint8_t bit_len, const float scale,
              const float offset, const sigEndianess endianess, const sigSign sign) : Signal<T>(name, unit, aliveTimeout, defaultValue, minValue, maxValue),
                                                                                      _start_bit(start_bit),
                                                                                      _bit_len(bit_len),
                                                                                      _scale(scale),
                                                                                      _offset(offset),
                                                                                      _endianess(endianess),
                                                                                      _sign(sign)
    {
    }

    void update(const CANMessage &msg)
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
            // Little endian. Only full bytes supported yet.
            for (unsigned int i = 0; i < _bit_len / 8; i++)
            {
                bits |= (uint32_t)(msg.data[(_start_bit / 8 + i)] << (i * 8));
            }
        }

        // if (_bit_len % 8)
        //     bits |= (uint32_t)((msg.data[(_start_bit + _bit_len / 8)] & ((1 << (_bit_len % 8)) - 1)) << (_bit_len - _bit_len % 8));

        if (_sign == sigSign::sigUNSIGNED)
        {
            this->set(bits * _scale + _offset);
        }
        else
        {
            int32_t signed_bits = *((int32_t *)&bits);
            this->set(signed_bits * _scale + _offset);
        }
    }

    u_int8_t getBitLen()
    {
        return _bit_len;
    }

private:
    const uint8_t _start_bit;      // Starting bit of the signal in the message
    const uint8_t _bit_len;        // Length of the signal in bits
    const float _scale;            // Scale factor for the signal value
    const float _offset;           // Offset applied to the signal value
    const sigEndianess _endianess; // Endianess of the signal value
    const sigSign _sign;           // Sign of the signal value (signed or unsigned)
};
