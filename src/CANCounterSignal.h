#pragma once
#include <ACAN_T4.h>
#include <CANSignal.h>
#include <Arduino.h>

// https://www.autosar.org/fileadmin/standards/classic/19-11/AUTOSAR_SWS_E2ELibrary.pdf

#define handleError(errortype)

template <typename T>
class CANCounterSignal : public CANSignal<T>
{
public:
    /**
     * @brief Construct a new CANCounterSignal object
     *
     * @param name
     * @param unit
     * @param aliveTimeout
     * @param defaultValue
     * @param minValue
     * @param maxValue
     * @param start_bit
     * @param bit_len
     * @param scale
     * @param offset
     * @param endianess
     * @param sign
     * @param syncCounter How many valid counters do we expect to be synchronized (default=2)
     * @param maxDeltaCounterInit What is the maximum allowed increment in counting (default=1 only a increase by one is valid)
     */
    CANCounterSignal(const char *name, const char *unit, u_int32_t aliveTimeout, T defaultValue, T minValue, T maxValue,
                     const uint8_t start_bit, const uint8_t bit_len, const float scale,
                     const float offset, const sigEndianess endianess, const sigSign sign, const uint8_t syncCounter,
                     const uint8_t maxDeltaCounterInit) : CANSignal<T>(name, unit, aliveTimeout, defaultValue, minValue, maxValue,
                                                                       start_bit, bit_len, scale,
                                                                       offset, endianess, sign),
                                                          _syncCounter(syncCounter),
                                                          _maxDeltaCounterInit(maxDeltaCounterInit)
    {
        _counter = 0;
        _syncCallsCounter = 0;
        _inSync = false;
    }

    boolean updateAndCheck(const CANMessage &msg)
    {
        this->update(msg);

        /*Counter to be used for protecting the next Data. The initial value is 0,
        which means that the first Data will have the counter 0. After the
        protection by the Counter, the Counter is incremented modulo 0xF. The
        value 0xF is skipped (after 0xE the next is 0x0), as 0xF value represents
        the error value. The four high bits are always 0.*/
        u_int8_t currentCounter = this->get();

        if ((currentCounter & 0b00001111) != 0) // Check if the 4 high bits are zero
        {
            handleError("Corrupted message");
            _inSync = false;
            _counter = currentCounter;
            _syncCallsCounter = 0;
            return _inSync;
        }
        currentCounter = currentCounter >> 4; // Shift low bits to right
        if (currentCounter == 0)              // Make it 16 if zero to capture jump back from 15 to 0. Value 16 is never reached
        {
            currentCounter = 16;
        }

        if (_inSync) // When we are in synchronization
        {
            if (currentCounter <= _counter + _maxDeltaCounterInit) // Sync is still ok
            {
                _inSync = true;
            }
            else // We lost sync here
            {
                handleError("Counter lost sync");
                _inSync = false;
                _syncCallsCounter = 0;
            }
        }
        else // When we are not in sync
        {
            if (currentCounter <= _counter + _maxDeltaCounterInit) // Counter is okay
            {
                _syncCallsCounter++;
                if (_syncCallsCounter == _syncCounter) // We got enough good signals to sync
                {
                    _inSync = true;
                }
                else // On our way to sync but not yet
                {
                    _inSync = false;
                }
            }
            else // Signal still skipped messages
            {
                _inSync = false;
                _syncCallsCounter = 0;
            }
        }

        _counter = currentCounter % 16; // Set it to to zero if 16 or to the remainder by 16
        return _inSync;
    }

private:
    /*Counter to be used for protecting the next Data. The initial value is 0,
    which means that the first Data will have the counter 0. After the
    protection by the Counter, the Counter is incremented modulo 0xF. The
    value 0xF is skipped (after 0xE the next is 0x0), as 0xF value represents
    the error value. The four high bits are always 0.*/
    u_int8_t _counter;

    /*Number of Data required for validating the consistency of the counter that
    must be received with a valid counter (i.e. counter within the allowed lockin range) after the detection of an unexpected behavior of a received
    counter.*/
    const u_int8_t _syncCounter;

    // Counts how many of past calls met sync criteria
    u_int8_t _syncCallsCounter;

    /*Initial maximum allowed gap between two counter values of two
    consecutively received valid Data. For example, if the receiver gets Data
    with counter 1 and MaxDeltaCounterInit is 1, then at the next reception
    the receiver can accept Counters with values 2 and 3, but not 4.
    Note that if the receiver does not receive new Data at a consecutive read,
    then the receiver increments the tolerance by 1.*/
    const u_int8_t _maxDeltaCounterInit;

    /*Value to check, if the counter was initialized*/
    boolean _inSync;
};
