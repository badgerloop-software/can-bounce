#include <Arduino.h>
#include "STM32_CAN.h"

STM32_CAN canBus( CAN1, ALT_2 );

CAN_message_t msg;

void setup() {
  canBus.setBaudRate(500000);
  msg.buf[0] = 255;
  msg.id = 0x100;
}

void loop() {
  delay(1000);
  if (canBus.write(msg)) {
    printf("Unsuccessful message sending\n");
  } else {
    msg.buf[0]--;
    printf("Successful send\n");
  }
}
