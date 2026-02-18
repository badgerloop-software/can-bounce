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

// type message in serial monitor to send CAN message to lighting board
void loop() {
  if(Serial.available() > 0){
    msg = Serial.read(); // only take the first char
    while(Serial.available()) {
      Serial.read();
    }
    switch (msg) {
      case '1':
        steering.left_blink = !steering.left_blink;
        break;
      case '2':
        steering.right_blink = !steering.right_blink;
        break;
      case '3':
        steering.headlight = !steering.headlight;
        break;
      case '4':
        pdc.brakeLED = !pdc.brakeLED;
        break;
      case '5':
        bps_fault = (bps_fault == 0)? 1 : 0;
        break;
      case '6':
        // Toggle hazards by setting both blinkers
        if (steering.left_blink && steering.right_blink) {
          // Turn off hazards
          steering.left_blink = 0;
          steering.right_blink = 0;
        } else {
          // Turn on hazards
          steering.left_blink = 1;
          steering.right_blink = 1;
        }
        break;
      default:
        break;
    }
  }
  candecoder.runQueue(1000);
  candecoder.sendSignal();
  counter++;
  if (counter >= 1) {
    counter = 0;
    printf("\033[2J\033[1;1H");
    printf("1) Left Blink: %s\n", steering.left_blink? "on" : "off");
    printf("2) Right Blink: %s\n", steering.right_blink? "on" : "off");
    printf("3) Headlight: %s\n", steering.headlight? "on" : "off");
    printf("4) Brake Light: %s\n", pdc.brakeLED? "on" : "off");
    printf("5) BPS Fault: %s\n", bps_fault? "on" : "off");
    printf("6) Hazards: %s\n", hazards.hazards? "on" : "off");
    printf("sendsuccess: %s\n", sendsuccess? "yes" : "no");
    printf("messages received: %d\n", num_msg_received);
    printf("steering raw: 0x%02X\n", steering.pack());
  }
}