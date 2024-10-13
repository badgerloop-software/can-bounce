#include <Arduino.h>
#include "STM32_CAN.h"
#include "candecoder.h"

CANDecoder candecoder(CAN1, ALT_2);

extern uint8_t int8Received;
extern uint16_t int16Received;
extern float floatReceived;
extern bool boolReceived;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("hey");

  candecoder.sendSignal();
  candecoder.runQueue(1000);

  Serial.println(int8Received);
  Serial.println(int16Received);
  Serial.println(floatReceived);
  Serial.println(boolReceived);
}