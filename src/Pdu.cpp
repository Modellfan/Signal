#include <Pdu.h>

Pdu::Pdu(uint32_t id, PduSignal *signal1)
{
    _id = id;
    _signals[0] = signal1;
    _signalCount = 1;
}

Pdu::Pdu(uint32_t id, PduSignal *signal1, PduSignal *signal2)
{
    _id = id;
    _signals[0] = signal1;
    _signals[1] = signal2;
    _signalCount = 2;
}

Pdu::Pdu(uint32_t id, PduSignal *signal1, PduSignal *signal2, PduSignal *signal3)
{
    _id = id;
    _signals[0] = signal1;
    _signals[1] = signal2;
    _signals[2] = signal3;
    _signalCount = 3;
}

Pdu::Pdu(uint32_t id, PduSignal *signal1, PduSignal *signal2, PduSignal *signal3, PduSignal *signal4)
{
    _id = id;
    _signals[0] = signal1;
    _signals[1] = signal2;
    _signals[2] = signal3;
    _signals[3] = signal4;
    _signalCount = 4;
}

Pdu::Pdu(uint32_t id, PduSignal *signal1, PduSignal *signal2, PduSignal *signal3, PduSignal *signal4, PduSignal *signal5)
{
    _id = id;
    _signals[0] = signal1;
    _signals[1] = signal2;
    _signals[2] = signal3;
    _signals[3] = signal4;
    _signals[4] = signal5;
    _signalCount = 5;
}

Pdu::Pdu(uint32_t id, PduSignal *signal1, PduSignal *signal2, PduSignal *signal3, PduSignal *signal4, PduSignal *signal5, PduSignal *signal6)
{
    _id = id;
    _signals[0] = signal1;
    _signals[1] = signal2;
    _signals[2] = signal3;
    _signals[3] = signal4;
    _signals[4] = signal5;
    _signals[5] = signal6;
    _signalCount = 6;
}

void Pdu::unpack(const CANMessage &msg)
{
    if (msg.id == _id)
    {
        for (int i = 0; i < _signalCount; i++)
        {
            if (_signals[i])
            {
                _signals[i]->unpack(msg);
            }
        }
    }
}


