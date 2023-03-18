#pragma once
#include "Arduino.h"

// Usage
// Signal<float> test("Temp1_Mod1", "°C", 500, 25, -40, 100);

#define DET_REPORTERROR(Error_Sender, Error_ID) ;

typedef enum
{
    COM_BOOLEAN,
    COM_UINT8,
    COM_UINT16,
    COM_UINT32,
    // COM_UINT64,
    // COM_UINT8_N,
    // COM_UINT8_DYN,
    COM_SINT8,
    COM_SINT16,
    COM_SINT32,
    // COM_SINT64,
    COM_FLOAT
} Com_SignalType;


#define COM_SIGNALTYPE_UNSIGNED FALSE
#define COM_SIGNALTYPE_SIGNED TRUE

#define SignalTypeToSize(type)                                                       \
    ((type == COM_UINT8) ? sizeof(uint8_t) : (type == COM_UINT16) ? sizeof(uint16_t) \
                                         : (type == COM_UINT32)   ? sizeof(uint32_t) \
                                         : (type == COM_SINT8)    ? sizeof(int8_t)   \
                                         : (type == COM_SINT16)   ? sizeof(int16_t)  \
                                         : (type == COM_SINT32)   ? sizeof(int32_t)  \
                                         : (type == COM_FLOAT)    ? sizeof(float)    \
                                                                  : sizeof(boolean))

class ComSignal
{
public:
    ComSignal(Com_SignalType signalType, const char *name, const char *unit, const u_int32_t aliveTimeout, const u_int32_t firstAliveTimeout, const float defaultValue, const float minValue, const float maxValue)
    {

        this->_name = name;
        this->_unit = unit;
        this->_type = signalType;

        this->_aliveTimeout = aliveTimeout;
        this->_firstAliveTimeout = firstAliveTimeout;
        this->_validSignal = false;
        this->_lastRefresh = millis();

        this->_defaultValue = defaultValue;
        this->_minValue = minValue;
        this->_maxValue = maxValue;
    }

    void checkTimeout()
    {
        if (_validSignal)
        {
            if (millis() > this->_lastRefresh + _aliveTimeout)
            {
                DET_REPORTERROR(_name, "Signal timedout in operation");
            }
        }
        else
        {
            if (millis() > this->_lastRefresh + _firstAliveTimeout)
            {
                DET_REPORTERROR(_name, "Signal timedout after reset");
            }
        }
    }

    /**
     * @brief get the content of the variable
     *
     * @return u_int32_t
     */
    float get()
    {
        return _value;
    }

    /**
     * @brief sets a new value to the variable. Checks before, if the value is in range. Resets the deadtime monitor.
     *
     * @param value
     * @return u_int32_t
     */
    float set(float value)
    {
        bool isBelow = false;
        bool isAbove = false;
        switch (_type)
        {
        case COM_SINT8:
            if (value.sint8 < _minValue.sint8)
                isBelow = true;
            else if (value.sint8 > (int8_t)_maxValue.sint8)
                isAbove = true;
            break;
        // case COM_UINT8:
        //     if ((uint8_t)value < (uint8_t)_minValue)
        //         isBelow = true;
        //     else if ((uint8_t)value > (uint8_t)_maxValue)
        //         isAbove = true;
        //     break;
        // case COM_SINT16:
        //     if ((int16_t)value < (int16_t)_minValue)
        //         isBelow = true;
        //     else if ((int16_t)value > (int16_t)_maxValue)
        //         isAbove = true;
        //     break;
        // case COM_UINT16:
        //     if ((uint16_t)value < (uint16_t)_minValue)
        //         isBelow = true;
        //     else if ((uint16_t)value > (uint16_t)_maxValue)
        //         isAbove = true;
        //     break;
        // case COM_SINT32:
        //     if ((int32_t)value < (int32_t)_minValue)
        //         isBelow = true;
        //     else if ((int32_t)value > (int32_t)_maxValue)
        //         isAbove = true;
        //     break;
        // case COM_UINT32:
        //     if ((uint32_t)value < (uint32_t)_minValue)
        //         isBelow = true;
        //     else if ((uint32_t)value > (uint32_t)_maxValue)
        //         isAbove = true;
        //     break;
        case COM_FLOAT:
            if (value.floatValue < _minValue.floatValue)
                isBelow = true;
            else if (value.floatValue > _maxValue.floatValue)
                isAbove = true;
            break;
        case COM_BOOLEAN:
            break;
        }

        if (isAbove)
        {
            DET_REPORTERROR(_name, "Value above max value");
            return _defaultValue;
        }

        if (isBelow)
        {
            DET_REPORTERROR(_name, "Value below min value");
            return _defaultValue;
        }

        this->_value = value;
        this->_validSignal = true;
        this->_lastRefresh = millis();

        return _value;
    }

    /**
     * @brief
     *
     * @param value Sets the value of the signal unprotected. Needed for later failure injection testing
     * @return u_int32_t
     */
    float unprotectedSet(float const &value)
    {
        this->_value = value;
        this->_validSignal = true;
        this->_lastRefresh = millis();
        return _value;
    }

    /**
     * @brief Returns true, if the signal is valid
     *
     * @return boolean
     */
    boolean valid()
    {
        return _validSignal;
    }

    Com_SignalValueType getDefaultValue()
    {
        return this->_defaultValue;
    }

    Com_SignalType getType()
    {
        return _type;
    }

    uint16_t length()
    {
        return SignalTypeToSize(_type);
    }

    // int8_t set_COM_SINT8(int8_t value)
    // {
    //     return (int8_t)set((uint32_t)value);
    // }
    // int8_t get_COM_SINT8()
    // {
    //     return (int8_t)get();
    // }

    // uint8_t set_COM_UINT8(uint8_t value)
    // {
    //     return (uint8_t)set((uint32_t)value);
    // }
    // uint8_t get_COM_UINT8()
    // {
    //     return (uint8_t)get();
    // }

    // int16_t set_COM_SINT16(int16_t value)
    // {
    //     return (int16_t)set((uint32_t)value);
    // }
    // int16_t get_COM_SINT16()
    // {
    //     return (int16_t)get();
    // }

    // uint16_t set_COM_UINT16(uint16_t value)
    // {
    //     return (uint16_t)set((uint32_t)value);
    // }
    // uint16_t get_COM_UINT16()
    // {
    //     return (uint16_t)get();
    // }

    // int32_t set_COM_SINT32(int32_t value)
    // {
    //     return (int32_t)set((uint32_t)value);
    // }
    // int32_t get_COM_SINT32()
    // {
    //     return (int32_t)get();
    // }

    // uint32_t set_COM_UINT32(uint32_t value)
    // {
    //     return (uint32_t)set((uint32_t)value);
    // }
    // uint32_t get_COM_UINT32()
    // {
    //     return (uint32_t)get();
    // }

    void print()
    {
        Serial.print(_name);
        Serial.print(": ");

        switch (_type)
        {
        // case COM_SINT8:
        //     Serial.print((int8_t)_value);
        //     break;
        // case COM_UINT8:
        //     Serial.print((uint8_t)_value);
        //     break;
        // case COM_SINT16:
        //     Serial.print((int16_t)_value);
        //     break;
        // case COM_UINT16:
        //     Serial.print((uint16_t)_value);
        //     break;
        // case COM_SINT32:
        //     Serial.print((int32_t)_value);
        //     break;
        // case COM_UINT32:
        //     Serial.print((uint32_t)_value);
        //     break;
        case COM_FLOAT:
            Serial.print(_value.floatValue);
            break;
        // case COM_BOOLEAN:
        //     Serial.print(((boolean)_value) ? "yes" : "no");
        }

        Serial.print(" ");
        Serial.print(_unit);
        Serial.print(" (valid: ");
        Serial.print(_validSignal ? "yes" : "no");
        Serial.println(")");
    }

private:
    float _value;        // Current value of the signal
    float _defaultValue; // Default value of the signal
    float _minValue;     // Minimum value of the signal
    float _maxValue;     // Maximum value of the signal

    Com_SignalType _type;

    bool _validSignal;      // Flag indicating if the signal has ever been received
    u_int32_t _lastRefresh; // Timeout for the signal to be considered alive
    u_int32_t _aliveTimeout;
    u_int32_t _firstAliveTimeout;

    const char *_name; // Name of the signal
    const char *_unit; // Unit of the signal value
};
