#include <Arduino.h>
#include "STM32_CAN.h"
#include "candecoder.h"

CANDecoder candecoder(CAN1, ALT_2, 250000);
extern bool send_success;
extern uint16_t messages_read;

uint8_t counter = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  candecoder.sendSignal();
  candecoder.runQueue(1000);
  printf("\033[2J\033[1;1H");
  printf("messages_read: %d\n", messages_read);
  printf("send_success: %d\n\n", send_success);
  printf("dir switch: %d\n", steering.direction_switch);
  printf("left_blink: %d\n", steering.left_blink);
  printf("right_blink: %d\n", steering.right_blink);
  printf("crzmode_A: %d\n", steering.crz_mode_a);
  printf("crz-set: %d\n", steering.crz_set);
  printf("crz_reset: %d\n", steering.crz_reset);
  printf("horn: %d\n", steering.horn);
  printf("regen_brake: %f\n", regen_brake);
}