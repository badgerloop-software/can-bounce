#include "candecoder.h"

#define IS_FIRST_CAN false

uint8_t int8Received;
uint16_t int16Received;
float floatReceived;
bool boolReceived;

int numMessagesReceived[7] = {0,0,0,0,0,0,0};
float messageReceived[7] = {0,0,0,0,0,0,0};

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency){};

void CANDecoder::readHandler(CAN_message_t msg) {
   int offset = msg.id - 0x400;
   if (offset >= 0 && offset <= 6) {
      numMessagesReceived[offset]++;
      switch (offset){
         case 0: //Bool
            boolReceived = *((bool*)msg.buf);
            messageReceived[offset] = boolReceived;
            break;
         case 1: //bool with enums
            boolReceived = *((bool*)msg.buf); 
            messageReceived[offset] = boolReceived;
            break;
         case 5: //Int8
            int8Received = *((int8_t*)msg.buf);
            messageReceived[offset] = int8Received;
            break;
         default: //Float
            floatReceived = *((float*)msg.buf);
            messageReceived[offset] = floatReceived;
            break;
      }
   }
}

void CANDecoder::sendSignal() {
   uint8_t ov_high = 1;
   this->sendMessage(0x051, (void*)&ov_high, sizeof(ov_high));
}