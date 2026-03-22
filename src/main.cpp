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
  }
  candecoder.runQueue(1000);
  candecoder.sendSignal();
  counter++;
  if (counter >= 1) {
    counter = 0;
    printf("\033[2J\033[1;1H");
    printf("PDC");
    printf("Direction: %s\n", pdc.direction? "reverse" : "forward");
    printf("MC Speed Sig: %s\n", pdc.mc_speed_sig? "on" : "off");
    printf("Eco Mode: %s\n", pdc.eco_mode? "on" : "off");
    printf("MC On: %s\n", pdc.mc_on? "on" : "off");
    printf("Park Brake: %s\n", pdc.park_brake? "engaged" : "released");
    printf("Brake LED: %s\n", pdc.brakeLED? "on" : "off");

    printf("\n\nSteering\n");
    printf("Direction Switch: %s\n", steering.direction_switch? "on" : "off");
    printf("Horn: %s\n", steering.horn? "on" : "off");
    printf("Cruise Mode A: %s\n", steering.crz_mode_a? "on" : "off");
    printf("Cruise Set: %s\n", steering.crz_set? "on" : "off");
    printf("Cruise Reset: %s\n", steering.crz_reset? "on" : "off");
    printf("Headlight: %s\n", steering.headlight? "on" : "off");
    printf("Left Blinker: %s\n", steering.left_blink? "on" : "off");
    printf("Right Blinker: %s\n", steering.right_blink? "on" : "off");
    
    printf("\n\nHazards: %s\n", hazards.hazards? "on" : "off");

    printf("\n\nBMS Fault: %s\n", bms_fault? "present" : "none");
  }
}