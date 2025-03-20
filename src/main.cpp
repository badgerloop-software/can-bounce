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
    msg = Serial.read();

    if (msg == '1') {
      led1 = !led1; // toggle
      led1msg.left_blink = led1;
      
    } else if (msg == '2') {
      led2 = !led2; // toggle
    }
  }
  candecoder.runQueue(50);
  candecoder.sendSignal();
  counter++;
  if (counter >=20) {
    printf("\033[2J\033[1;1H");
    printf("LED 1: %s\n", led1? "on" : "off");
    printf("LED 2: %s\n", led2? "on" : "off");
    printf("sendsuccess: %s\n", sendsuccess? "yes" : "no");
    printf("messages received: %d\n", num_msg_received);
    counter = 0;
  }

}