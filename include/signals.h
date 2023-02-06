
//SG_ CellVoltage0 : 0|16@1+ (0.001,0) [0|5] "V" BMS

//@0 = Big-Endian

//@1 = Little Endian

//+ signed value

//- unsigned value

#include <ACAN_T4.h>


enum class Endianess {
    SIGNAL_BIG_ENDIAN,
    SIGNAL_LITTLE_ENDIAN
};

enum class Sign {
    SIGNAL_SIGNED,
    SIGNAL_UNSIGNED
};

class Signal {
public:
  Signal(const char* name, const uint8_t start_bit, const uint8_t len, const float scale, 
         const float offset, const Endianess endianess, const Sign sign, const char* unit, 
         float aliveTimeout, float defaultValue, float minValue, float maxValue)
      : _name(name), _start_bit(start_bit), _len(len), _scale(scale), _offset(offset), 
        _endianess(endianess), _sign(sign), _unit(unit), _aliveTimeout(aliveTimeout),
        _defaultValue(defaultValue), _minValue(minValue), _maxValue(maxValue) {
    _value = _defaultValue;
    _wasEverReceived = false;
  }

    void update(const CANMessage& msg) {
        uint32_t signal_bits = 0;
        // Extract the relevant bits from the message
        for (int i = 0; i < _len; ++i) {
            signal_bits |= ((msg.data[(_start_bit + i) / 8] >> ((_start_bit + i) % 8)) & 1) << i;
        }

        if (_endianess == BIG_ENDIAN) {
            signal_bits = __builtin_bswap32(signal_bits);
        }

        // Apply the scaling and offset to the signal's bits
        if (_sign == SIGNED) {
            int32_t signed_value = (int32_t)signal_bits;
            _value = _scale * signed_value + _offset;
        } else {
            _value = _scale * signal_bits + _offset;
        }
    }


private:
  const char* _name;
  const uint8_t _start_bit;
  const uint8_t _len;
  const float _scale;
  const float _offset;
  const Endianess _endianess;
  const Sign _sign;
  const char* _unit;

  float _value;
  uint32_t _value_uint32;
  bool _wasEverReceived;
  float _aliveTimeout;
  float _defaultValue;
  float _minValue;
  float _maxValue;
};


// Signal voltageSignal("Voltage", 0, 16, 0.1, -10.0, Endianess::SIGNAL_BIG_ENDIAN, Sign::SIGNAL_SIGNED, "V", 0.0, 60.0, 0.0, 0.0);

