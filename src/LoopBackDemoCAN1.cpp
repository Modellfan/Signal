#include <ACAN_T4.h>
#include <CANSignal.h>
#include <CANCounterSignal.h>
#include <Arduino.h>

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

Signal<float> test("Temp1_Mod1", "°C", 500, 25, -40, 100);
// CANSignal<int> mySignal("Speed", "km/h", 5000, 0, 0, 250, 0, 8, 1, 0, sigEndianess::sigLITTLE_ENDIAN, sigSign::sigUNSIGNED);
//  signalsMessage msgBMWi3_x120(0x120);
CANSignal<float> CellVoltage0("CellVoltage0", "V", 300, 3.2, 0, 5, 0, 16, 0.001, 0, sigEndianess::sigLITTLE_ENDIAN, sigSign::sigUNSIGNED);
CANCounterSignal<u_int8_t> Counter("Counter", "", 300, 0, 0, 255, 48, 8, 1, 0, sigEndianess::sigLITTLE_ENDIAN, sigSign::sigUNSIGNED, 2, 1);

// Signal CellVoltage1("CellVoltage1", 16, 16, 0.001, 0, sigEndianess::sigLITTLE_ENDIAN, sigSign::sigUNSIGNED, "V", 200, 20, 0, 5);
// Signal CellVoltage2("CellVoltage2", 32, 16, 0.001, 0, sigEndianess::sigLITTLE_ENDIAN, sigSign::sigUNSIGNED, "V", 200, 20, 0, 5);
// Signal Counter("Counter", 48, 8, 1, 0, sigEndianess::sigLITTLE_ENDIAN, sigSign::sigUNSIGNED, "", 200, 0, 0, 1);
// Signal CRC("CRC", 56, 8, 1, 0, sigEndianess::sigLITTLE_ENDIAN, sigSign::sigUNSIGNED, "", 200, 0, 0, 1);
// msgBMWi3_x120.addSignal(CellVoltage0);

// Signal voltageSignal("Voltage", 0, 16, 0.1, -10.0, Endianess::SIGNAL_BIG_ENDIAN, Sign::SIGNAL_SIGNED, "V", 0.0, 60.0, 0.0, 0.0);
// signalsMessage msgBMWi3_x120(0x120,voltageSignal);

void setup()
{

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
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
    Serial.print("Received: ");
    Serial.println(gReceivedCount);
    // msgBMWi3_x120.updateSignals(message);
    // voltageSignal.update(message);
    if (message.id == 0x120)
    {
      if (Counter.updateAndCheck(message))
      {
        Counter.print();
        CellVoltage0.update(message);
        CellVoltage0.print();
        CRC.update(message);
        CRC.print();
      }
    }
    printFrame(message);
  }
}