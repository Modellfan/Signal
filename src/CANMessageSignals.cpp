#include "CANMessageSignals.h"

CANMesSig::CANMesSig(uint32_t id)
{
    for (int i = 0; i < CFG_MAX_SIGNALS; i++)
        _signals[i] = nullptr;
    _id = id;
}

void CANMesSig::updateSignals(const CANMessage &msg)
{
    if (msg.id == _id)
    {
        for (int i = 0; i < CFG_MAX_SIGNALS; i++)
        {
            if (_signals[i])
            {
                _signals[i]->update(msg);
            }
        }
    }
}

void CANMesSig::addSignal(Signal *signal)
{
    if (findSignal(signal) == -1)
    {
        int8_t i = findSignal(NULL);
        if (i != -1)
        {
            _signals[i] = signal;
        }
        else
        {
            // Logger::error("unable to add signal, max number of signals reached.");
        }
    }
}

void CANMesSig::removeSignal(Signal *signal)
{
    int8_t i = findSignal(signal);
    if (i != -1)
    {
        _signals[i] = nullptr;
    }
}

int8_t CANMesSig::findSignal(Signal *signal)
{
    for (int i = 0; i < CFG_MAX_SIGNALS; i++)
    {
        if (signal == _signals[i])
        {
            return i;
        }
    }
    return -1;
}
