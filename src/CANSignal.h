#pragma once
#include <ACAN_T4.h>
#include <baseSignal.h>
#include <ACAN_T4.h>

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
            const float offset, const sigEndianess endianess, const sigSign sign);

    void update(const CANMessage &msg);

private:
    const uint8_t _start_bit;      // Starting bit of the signal in the message
    const uint8_t _bit_len;        // Length of the signal in bits
    const float _scale;            // Scale factor for the signal value
    const float _offset;           // Offset applied to the signal value
    const sigEndianess _endianess; // Endianess of the signal value
    const sigSign _sign;           // Sign of the signal value (signed or unsigned)
};