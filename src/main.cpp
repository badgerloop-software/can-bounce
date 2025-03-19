#include <Arduino.h>
#include "STM32_CAN.h"
#include "candecoder.h"

CANDecoder candecoder(CAN1, DEF, 250000);

const int msgid = 0x300; // CAN ID for lighting board
char msg;

void setup() {
  Serial.begin(115200);
  candecoder.reset();
}

// type message in serial monitor to send CAN message to lighting board
void loop() {
  if(Serial.available() > 0){
    msg = Serial.read();

    //if msg is a '1', turn on led or smth
    u_int8_t onArr[1] = {0};
    u_int8_t offArr[1] = {0};

    bool messageData = (msg == '1');
    if(messageData){
      onArr[0] |= (1 << 0);
      candecoder.sendMessage(msgid, (void*)&onArr, sizeof(bool));
      candecoder.runQueue(1);
    }
    else if(!messageData){
      candecoder.sendMessage(msgid, (void*)&offArr, sizeof(bool));
      candecoder.runQueue(1);
    }
  }
    
}