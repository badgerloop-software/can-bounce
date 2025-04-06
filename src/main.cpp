#include <Arduino.h>
#include "STM32_CAN.h"
#include "candecoder.h"

CANDecoder candecoder(CAN1, ALT_2, 250000);

extern uint8_t int8Received;
extern uint16_t int16Received;
extern float floatReceived;
extern bool boolReceived;

extern uint32_t msgReceived;
extern uint32_t msgID;

extern int numMessagesReceived[4];

int counter = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  candecoder.sendSignal();
  candecoder.runQueue(50); // 50ms, so 20 sets of messages sent per second
  counter++;

  // print out debug info every second (instead of every 50ms)
  if (counter >= 20) {
    printf("id: %x | msg: %d | numMessageReceived: %d\n", msgID, msgReceived, numMessagesReceived[0]);
    counter = 0;
  }
}