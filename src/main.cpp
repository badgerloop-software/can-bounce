#include <Arduino.h>
#include "STM32_CAN.h"
#include "candecoder.h"

CANDecoder candecoder(CAN1, ALT_2, 250000);

extern uint8_t int8Received;
extern uint16_t int16Received;
extern float floatReceived;
extern bool boolReceived;

extern int numMessagesReceived[7];

int counter = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  candecoder.sendSignal();
  printf("sent message\n");
  candecoder.runQueue(5000);

  for (int i = 0; i < 7; i++) {
    printf("%d | ", numMessagesReceived[i]);
  }
  printf("\n");
}