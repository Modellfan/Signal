#include <ACAN_T4.h>
// #include <signalsMessage.h>
#include <Arduino.h>


void printFrame(CANMessage &frame)
{
  // Print message
  Serial.print("ID: ");
  Serial.print(frame.id,HEX);
  Serial.print(" Ext: ");
  if(frame.ext) {
    Serial.print("Y");
  } else {
    Serial.print("N");
  }
  Serial.print(" Len: ");
  Serial.print(frame.len,DEC);
  for(int i = 0;i < frame.len; i++) {
    Serial.print(frame.data[i],HEX);
    Serial.print(" ");
  }
  Serial.println();
}

// Signal voltageSignal("Voltage", 0, 16, 0.1, -10.0, Endianess::SIGNAL_BIG_ENDIAN, Sign::SIGNAL_SIGNED, "V", 0.0, 60.0, 0.0, 0.0);
//signalsMessage msgBMWi3_x120(0x120,voltageSignal);

void setup () {

  pinMode (LED_BUILTIN, OUTPUT) ;
  Serial.begin (9600) ;
  while (!Serial) {
    delay (50) ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }
  Serial.println ("CAN1 loopback test") ;
  ACAN_T4_Settings settings (500 * 1000) ; // 125 kbit/s
  const uint32_t errorCode = ACAN_T4::can1.begin (settings) ;
  Serial.print ("Bitrate prescaler: ") ;
  Serial.println (settings.mBitRatePrescaler) ;
  Serial.print ("Propagation Segment: ") ;
  Serial.println (settings.mPropagationSegment) ;
  Serial.print ("Phase segment 1: ") ;
  Serial.println (settings.mPhaseSegment1) ;
  Serial.print ("Phase segment 2: ") ;
  Serial.println (settings.mPhaseSegment2) ;
  Serial.print ("RJW: ") ;
  Serial.println (settings.mRJW) ;
  Serial.print ("Triple Sampling: ") ;
  Serial.println (settings.mTripleSampling ? "yes" : "no") ;
  Serial.print ("Actual bitrate: ") ;
  Serial.print (settings.actualBitRate ()) ;
  Serial.println (" bit/s") ;
  Serial.print ("Exact bitrate ? ") ;
  Serial.println (settings.exactBitRate () ? "yes" : "no") ;
  Serial.print ("Distance from wished bitrate: ") ;
  Serial.print (settings.ppmFromWishedBitRate ()) ;
  Serial.println (" ppm") ;
  Serial.print ("Sample point: ") ;
  Serial.print (settings.samplePointFromBitStart ()) ;
  Serial.println ("%") ;
  if (0 == errorCode) {
    Serial.println ("can1 ok") ;
  }else{
    Serial.print ("Error can1: 0x") ;
    Serial.println (errorCode, HEX) ;
    while (1) {
      delay (100) ;
      Serial.println ("Invalid setting") ;
      digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    }
  }
}

//-----------------------------------------------------------------

static uint32_t gBlinkDate = 0 ;
static uint32_t gReceivedCount = 0 ;

//-----------------------------------------------------------------

void loop () {
  if (gBlinkDate <= millis ()) {
    gBlinkDate += 500 ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }
  CANMessage message ;

  if (ACAN_T4::can1.receive (message)) {
    gReceivedCount += 1 ;
    Serial.print ("Received: ") ;
    Serial.println (gReceivedCount) ;
    //msgBMWi3_x120.updateSignals(message);
    //voltageSignal.update(message);
     printFrame(message);
  }
}