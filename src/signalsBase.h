#pragma once
#include "Arduino.h"

#define HandleFault(faulttype)

template <typename T>
class signal
{
public:
    /**
     * @brief Construct a new signal object
     *
     * @param name Name of the Signal
     * @param unit Unit of the Signal
     * @param aliveTimeout Time in milliseconds, when the signal should throw a timeout error
     * @param defaultValue The default value of the signal for initialization
     * @param minValue The max value of the signal
     * @param maxValue The min value of the signal
     */
    signal(const char *name, const char *unit, u_int32_t aliveTimeout, T defaultValue, T minValue, T maxValue)
    {
        this->_defaultValue = defaultValue;
        this->_name = name;
        this->_unit = unit;
        this->_minValue = minValue;
        this->_maxValue = maxValue;
        this->_modifiedSinceReset = 0u;
        this->_lastRefresh = 0u;
    }

    // ***
    // *** Allows the variable to be used on the left side of
    // *** the equal sign.
    // ***
    T operator=(T const &value) const
    {
        this->set(value);
        return this->get();
    }

    // ***
    // *** Allows the variable to be used on the right side of
    // *** the equal sign.
    // ***
    operator T()
    {
        return this->get();
    }

    // ***
    // *** ++ postfix
    // ***
    T operator++(int)
    {
        T oldValue = this->get();
        T newValue = oldValue + 1;
        this->set(newValue);
        return oldValue;
    }

    // ***
    // *** ++ prefix
    // ***
    T operator++()
    {
        T newValue = this->get() + 1;
        this->set(newValue);
        return newValue;
    }

    // ***
    // *** -- postifx
    // ***
    T operator--(int)
    {
        T oldValue = this->get();
        T newValue = oldValue - 1;
        this->set(newValue);
        return oldValue;
    }

    // ***
    // *** -- prefix
    // ***
    T operator--()
    {
        T newValue = this->get() - 1;
        this->set(newValue);
        return newValue;
    }

    // ***
    // *** += operator
    // ***
    T operator+=(T const &value) const
    {
        T newValue = this->get() + value;
        this->set(newValue);
        return newValue;
    }

    // ***
    // *** -= operator
    // ***
    T operator-=(T const &value) const
    {
        T newValue = this->get() - value;
        this->set(newValue);
        return newValue;
    }

    // ***
    // *** *= operator
    // ***
    T operator*=(T const &value) const
    {
        T newValue = this->get() * value;
        this->set(newValue);
        return newValue;
    }

    // ***
    // *** /= operator
    // ***
    T operator/=(T const &value) const
    {
        T newValue = this->get() / value;
        this->set(newValue);
        return newValue;
    }

    // ***
    // *** %= operator
    // ***
    T operator%=(T const &value) const
    {
        T newValue = this->get() % value;
        this->set(newValue);
        return newValue;
    }

    // ***
    // *** &= operator
    // ***
    T operator&=(T const &value) const
    {
        T newValue = this->get() & value;
        this->set(newValue);
        return newValue;
    }

    // ***
    // *** |= operator
    // ***
    T operator|=(T const &value) const
    {
        T newValue = this->get() | value;
        this->set(newValue);
        return newValue;
    }

    // ***
    // *** > operator
    // ***
    bool operator>(T const &value) const
    {
        return this->get() > value;
    }

    // ***
    // *** < operator
    // ***
    bool operator<(T const &value) const
    {
        return this->get() < value;
    }

    // ***
    // *** >= operator
    // ***
    bool operator>=(T const &value) const
    {
        return this->get() >= value;
    }

    // ***
    // *** <= operator
    // ***
    bool operator<=(T const &value) const
    {
        return this->get() <= value;
    }

    // ***
    // *** == operator
    // ***
    bool operator==(T const &value) const
    {
        return this->get() == value;
    }

    // ***
    // *** Get the value of the signal
    // ***
    T get()
    {
        T returnValue;
        returnValue = this->_defaultValue;
        return returnValue;
    }

    // ***
    // *** Sets the value of the signal
    // ***
    T set(T const &value)
    {
        if (value > _maxValue)
        {
            HandleFault("Value above limit");
        }
        else if (value < _minValue)
        {
            HandleFault("Value below limit");
        }
        else
        {
            this->_value = value;
            this->_modifiedSinceReset = true;
            this->_lastRefresh = millis();
        }
        return _value;
    }

    // ***
    // *** Sets the value of the signal unprotected. Needed for later failure injection testing
    // ***
    T unprotectedSet(T const &value)
    {
        this->_value = value;
        this->_modifiedSinceReset = true;
        this->_lastRefresh = millis();
        return _value;
    }

    uint16_t length()
    {
        return sizeof(T);
    }

    void checkTimeout()
    {
        if (this._lastRefresh + _aliveTimeout > millis())
        {
            HandleFault("Signal timedout");
        }
    }

    T getDefaultValue()
    {
        return this->_defaultValue;
    }

private:
    T _value;                 // Current value of the signal
    bool _modifiedSinceReset; // Flag indicating if the signal has ever been received
    u_int32_t _lastRefresh;   // Timeout for the signal to be considered alive
    u_int32_t _aliveTimeout;
    T _defaultValue;   // Default value of the signal
    T _minValue;       // Minimum value of the signal
    T _maxValue;       // Maximum value of the signal
    const char *_name; // Name of the signal
    const char *_unit; // Unit of the signal value
};
