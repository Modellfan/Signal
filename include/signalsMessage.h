#pragma once
#include <ACAN_T4.h>
#include <signals.h>

#define CFG_MAX_SIGNALS 8

class signalsMessage {
 public:
  signalsMessage(uint32_t id);
  void updateSignals(const CANMessage& msg);
  void addSignal(Signal *signal);
  void removeSignal(Signal *signal);
  int8_t findSignal(Signal *signal);
  // Device *getDeviceByType(DeviceType);
  // void printDeviceList();

 private:
  uint32_t _id;
  Signal *_signals[CFG_MAX_SIGNALS];
};





    

