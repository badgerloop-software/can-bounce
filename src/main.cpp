#include <Arduino.h>
#include "STM32_CAN.h"

STM32_CAN canBus( CAN1, ALT_2 );

CAN_message_t msg;

void setup() {
  canBus.begin();
  canBus.setBaudRate(500000);
}

void loop() {
  delay(1000);
  if (canBus.read(msg)) {
    printf("Message received: %x | %d\n", msg.id, msg.buf[0]);
  } else {
    printf("no messages in read queue\n");
  }
}
