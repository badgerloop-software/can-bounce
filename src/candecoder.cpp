#include "candecoder.h"

#define FIRST_CAN 0x000
#define SECOND_CAN 0x100

#define IS_FIRST_CAN true

uint8_t int8Received;
uint16_t int16Received;
float floatReceived;
boolean boolReceived;


CANDecoder::CANDecoder(PinName rd, PinName td, int frequency = DEFAULT_CAN_FREQ) : CANManager(CAN_TypeDef* canPort, CAN_PINS pins, int frequency){};

#if IS_FIRST_CAN
void CANDecoder::readHandler(CAN_data msg) {
   switch (msg.id) {
      case (FIRST_CAN + 0):
         int8Received = *msg.buf;
         break;
      case (FIRST_CAN + 1):
         int16Received = *msg.buf;
         break;
      case (FIRST_CAN + 2):
         floatReceived = *msg.buf;
         break;
      case (FIRST_CAN + 3):
         boolReceived = *msg.buf;
         break;
   }   
}

int CANDecoder::sendSignal() {
   uint8_t test8int = 496; // random 8-bit integer
   this->sendMessage(SECOND_CAN + 0, (void*)&test8int, sizeof(test8int));

   uint16_t test16int = 30271; // random 16-bit integer
   this->sendMessage(SECOND_CAN + 1, (void*)&test16int, sizeof(test16int));

   float testFloat = 9.8; // random floating point number
   this->sendMessage(SECOND_CAN + 2, (void*)&testFloat, sizeof(testFloat));

   float testBoolean = true;
   this->sendMessage(SECOND_CAN + 3, (void*)&testBoolean, sizeof(testBoolean));
}

#else
void CANDecoder::readHandler(CAN_data msg) {
   switch (msg.id) {
      case (SECOND_CAN + 0):
         int8Received = *msg.buf;
         break;
      case (SECOND_CAN + 1):
         int16Received = *msg.buf;
         break;
      case (SECOND_CAN + 2):
         floatReceived = *msg.buf;
         break;
      case (SECOND_CAN + 3):
         boolReceived = *msg.buf;
         break;
   }   
}

int CANDecoder::sendSignal() {
   uint8_t test8int = 234; // random 8-bit integer
   this->sendMessage(FIRST_CAN + 0, (void*)&test8int, sizeof(test8int));

   uint16_t test16int = 57219; // random 16-bit integer
   this->sendMessage(FIRST_CAN + 1, (void*)&test16int, sizeof(test16int));

   float testFloat = 8.6; // random floating point number
   this->sendMessage(FIRST_CAN + 2, (void*)&testFloat, sizeof(testFloat));

   float testBoolean = false;
   this->sendMessage(FIRST_CAN + 3, (void*)&testBoolean, sizeof(testBoolean));
}
#endif