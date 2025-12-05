#include <Arduino.h>
#include "STM32_CAN.h"
#include "candecoder.h"

//Define CAN bus object through CANDecoder class
CANDecoder candecoder(CAN1, ALT_2, 250000);

void setup() {
  Serial.begin(115200);
}

void loop() {
  //Send data over CAN
  candecoder.sendSignal();

  //Receive data over CAN every 50ms
  candecoder.runQueue(50);
  // printf("num messages: %d\n", counter_messages);
}