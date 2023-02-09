#pragma once
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

class Signal
{
public:
    Signal(const char *name, const uint8_t start_bit, const uint8_t len, const float scale,
           const float offset, const sigEndianess endianess, const sigSign sign, const char *unit,
           float aliveTimeout, float defaultValue, float minValue, float maxValue);
    void update(const CANMessage &msg);
    void print();

private:
    const char *_name;             // Name of the signal
    const uint8_t _start_bit;      // Starting bit of the signal in the message
    const uint8_t _bit_len;        // Length of the signal in bits
    const float _scale;            // Scale factor for the signal value
    const float _offset;           // Offset applied to the signal value
    const sigEndianess _endianess; // Endianess of the signal value
    const sigSign _sign;           // Sign of the signal value (signed or unsigned)
    const char *_unit;             // Unit of the signal value

    float _value; // Current value of the signal
    bool _wasEverReceived; // Flag indicating if the signal has ever been received
    float _aliveTimeout;   // Timeout for the signal to be considered alive
    float _defaultValue;   // Default value of the signal
    float _minValue;       // Minimum value of the signal
    float _maxValue;       // Maximum value of the signal
};