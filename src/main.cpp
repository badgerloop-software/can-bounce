#include <Arduino.h>
#include "STM32_CAN.h"
#include "candecoder.h"

CANDecoder candecoder(CAN1, ALT_2, 500000);

extern uint8_t int8Received;
extern uint16_t int16Received;
extern float floatReceived;
extern bool boolReceived;

extern int numMessagesReceived[4];

int counter = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  candecoder.sendSignal();
  candecoder.runQueue(50);
  counter++;

  if (counter > 20) {
    counter = 0;
    Serial.println(int8Received);
    Serial.println(int16Received);
    Serial.println(floatReceived);
    Serial.println(boolReceived);
    printf("%d | %d | %d | %d\n\n", numMessagesReceived[0], numMessagesReceived[1], numMessagesReceived[2], numMessagesReceived[3]);
  }
}