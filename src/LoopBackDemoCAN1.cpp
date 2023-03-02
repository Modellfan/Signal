#include <ACAN_T4.h>
// #include <CANSignal.h>
// #include <CANCounterSignal.h>
// #include <CANCRCSignal.h>
#include <Arduino.h>
#include <ComSignal.h>
#include <Pdu.h>
// #include <E2E_P02.h>
//  #include <CANMessageSignals.h>
// #include <CRC8.h>

void printFrame(CANMessage &frame)
{
  // Print message
  Serial.print("ID: ");
  Serial.print(frame.id, HEX);
  Serial.print(" Ext: ");
  if (frame.ext)
  {
    Serial.print("Y");
  }
  else
  {
    Serial.print("N");
  }
  Serial.print(" Len: ");
  Serial.print(frame.len, DEC);
  Serial.print(" ");
  for (int i = 0; i < frame.len; i++)
  {
    Serial.print(frame.data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}


PduSignal Mod0_Volt0(COM_FLOAT, "Mod0_Volt0", "V", 300, 2000, 3.2, 0, 5, 0, 16, 0.001, 0, COM_LITTLE_ENDIAN, COM_UNSIGNED);
PduSignal Mod0_Volt1(COM_FLOAT, "Mod0_Volt0", "V", 300, 2000, 3.2, 0, 5, 16, 16, 0.001, 0, COM_LITTLE_ENDIAN, COM_UNSIGNED);
PduSignal Mod0_Volt2(COM_FLOAT, "Mod0_Volt0", "V", 300, 2000, 3.2, 0, 5, 32, 16, 0.001, 0, COM_LITTLE_ENDIAN, COM_UNSIGNED);
Pdu BMWi3_Battery_Module_0_x120(0x120, &Mod0_Volt0, &Mod0_Volt1, &Mod0_Volt2);

// CANCounterSignal<u_int8_t> Counter_0x120("Counter_0x120", "", 300, 0, 0, 255, 48, 8, 1, 0, sigEndianess::sigLITTLE_ENDIAN, sigSign::sigUNSIGNED);
// CANCRCSignal<u_int8_t> CRC_0x120("CRC_0x120", "", 300, 0, 0, 255, 56, 8, 1, 0, sigEndianess::sigLITTLE_ENDIAN, sigSign::sigUNSIGNED);


void setup()
{
    
  // crc8.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(500000);
  while (!Serial)
  {
    delay(50);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  Serial.println("CAN1 loopback test");
  ACAN_T4_Settings settings(500 * 1000); // 125 kbit/s
  const uint32_t errorCode = ACAN_T4::can1.begin(settings);
  Serial.print("Bitrate prescaler: ");
  Serial.println(settings.mBitRatePrescaler);
  Serial.print("Propagation Segment: ");
  Serial.println(settings.mPropagationSegment);
  Serial.print("Phase segment 1: ");
  Serial.println(settings.mPhaseSegment1);
  Serial.print("Phase segment 2: ");
  Serial.println(settings.mPhaseSegment2);
  Serial.print("RJW: ");
  Serial.println(settings.mRJW);
  Serial.print("Triple Sampling: ");
  Serial.println(settings.mTripleSampling ? "yes" : "no");
  Serial.print("Actual bitrate: ");
  Serial.print(settings.actualBitRate());
  Serial.println(" bit/s");
  Serial.print("Exact bitrate ? ");
  Serial.println(settings.exactBitRate() ? "yes" : "no");
  Serial.print("Distance from wished bitrate: ");
  Serial.print(settings.ppmFromWishedBitRate());
  Serial.println(" ppm");
  Serial.print("Sample point: ");
  Serial.print(settings.samplePointFromBitStart());
  Serial.println("%");
  if (0 == errorCode)
  {
    Serial.println("can1 ok");
  }
  else
  {
    Serial.print("Error can1: 0x");
    Serial.println(errorCode, HEX);
    while (1)
    {
      delay(100);
      Serial.println("Invalid setting");
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
}

//-----------------------------------------------------------------

static uint32_t gBlinkDate = 0;
static uint32_t gReceivedCount = 0;

//-----------------------------------------------------------------

void loop()
{
  if (gBlinkDate <= millis())
  {
    gBlinkDate += 500;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  CANMessage message;

  if (ACAN_T4::can1.receive(message))
  {
    gReceivedCount += 1;
    BMWi3_Battery_Module_0_x120.unpack(message);
    Mod0_Volt0.print();
    // Serial.print("Received: ");
    // Serial.println(gReceivedCount);
    // msgBMWi3_x120.updateSignals(message);
    // voltageSignal.update(message);
    // if (message.id == 0x120)
    // {
    // if (CRC.unpackAndCheck(message))
    // {
    //   if (Counter.unpackAndCheck(message))
    //   {
    //     // CellVoltage0.update(message);
    //     // CellVoltage0.print();
    //     // CRC.update(message);
    //     // CRC.print();
    //   }
    // }
    // }
    // printFrame(message);
  }
}