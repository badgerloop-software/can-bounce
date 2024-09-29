#include <Arduino.h>
#include "STM32_CAN.h"

STM32_CAN canBus( CAN1, ALT_2 );

CAN_message_t msg;

void setup() {
  msg.buf[0] = 255;
  msg.id = 0x100;
  canBus.begin();
  canBus.setBaudRate(500000);
}

void loop() {
  delay(1000);
  if (canBus.write(msg)) {
    msg.buf[0] = msg.buf[0] - 1;
    printf("Successful send\n");
  } else {
    printf("Unsuccessful message sending\n");
  }
}
