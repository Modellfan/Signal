#pragma once
#include <ACAN_T4.h>
#include <PduSignal.h>

#define CFG_MAX_SIGNALS 8

class Pdu
{
public:
  Pdu(uint32_t id, PduSignal *signal1);
  Pdu(uint32_t id, PduSignal *signal1, PduSignal *signal2);
  Pdu(uint32_t id, PduSignal *signal1, PduSignal *signal2, PduSignal *signal3);
  Pdu(uint32_t id, PduSignal *signal1, PduSignal *signal2, PduSignal *signal3, PduSignal *signal4);
  Pdu(uint32_t id, PduSignal *signal1, PduSignal *signal2, PduSignal *signal3, PduSignal *signal4, PduSignal *signal5);
  Pdu(uint32_t id, PduSignal *signal1, PduSignal *signal2, PduSignal *signal3, PduSignal *signal4, PduSignal *signal5, PduSignal *signal6);

  void unpack(const CANMessage &msg);
  void pack(CANMessage &msg);

private:
  uint32_t _id;
  uint8_t _signalCount;
  PduSignal *_signals[CFG_MAX_SIGNALS];
};
