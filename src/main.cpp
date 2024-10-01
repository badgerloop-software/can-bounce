#include <Arduino.h>
#include "STM32_CAN.h"
#include "canmanager.h"

// STM32_CAN canBus( CAN1, ALT_2 );
CANManager canBus(CAN1, ALT_2);

CAN_message_t msg;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("hey");
  delay(1000);
}
