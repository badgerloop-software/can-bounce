#include <Arduino.h>
#include "STM32_CAN.h"
#include "candecoder.h"

CANDecoder candecoder(CAN1, ALT_2, 250000);

char msg;
uint8_t counter = 0;

void setup() {
  Serial.begin(115200);
  candecoder.reset();
}

void loop() {
  if (Serial.available() > 0) {
    msg = Serial.read();
    while (Serial.available()) {
      Serial.read();
    }
    switch (msg) {
      case '1':
        if (!hazards) {
          steering.left_blink = !steering.left_blink;
        }
        break;
      case '2':
        if (!hazards) {
          steering.right_blink = !steering.right_blink;
        }
        break;
      case '3':
        steering.headlight = !steering.headlight;
        break;
      case '4':
        pdc.brake_led = !pdc.brake_led;
        break;
      case '5':
        bps_fault = (bps_fault == 0) ? 1 : 0;
        break;
      case '6':
        hazards = !hazards;
        if (hazards) {
          steering.left_blink = false;
          steering.right_blink = false;
        }
        break;
      case '7':
        pdc.direction = !pdc.direction;
        break;
      default:
        break;
    }
  }
  candecoder.runQueue(20);
  candecoder.sendSignal();
  counter++;
  if (counter >= 25) {
    counter = 0;
    printf("\033[2J\033[1;1H");
    printf("1) Left Blink: %s\n", steering.left_blink ? "on" : "off");
    printf("2) Right Blink: %s\n", steering.right_blink ? "on" : "off");
    printf("3) Headlight: %s\n", steering.headlight ? "on" : "off");
    printf("4) Brake Light: %s\n", pdc.brake_led ? "on" : "off");
    printf("5) BPS Fault: %s\n", bps_fault ? "on" : "off");
    printf("6) Hazards: %s\n", hazards ? "on" : "off");
    printf("7) Reverse: %s\n", pdc.direction ? "off" : "on");
    printf("sendsuccess: %s\n", sendsuccess ? "yes" : "no");
    printf("messages received: %d\n", num_msg_received);
  }
}
