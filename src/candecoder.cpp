#include "candecoder.h"

#define IS_FIRST_CAN false

#if IS_FIRST_CAN
#define RECEIVE_BASE_ID 0x000
#define SEND_BASE_ID 0x100
#define INT8_VAL 16
#define INT16_VAL 30271
#define FLOAT_VAL 9.8
#define BOOL_VAL true
#else
#define RECEIVE_BASE_ID 0x100
#define SEND_BASE_ID 0x000
#define INT8_VAL 234
#define INT16_VAL 57219
#define FLOAT_VAL 8.6
#define BOOL_VAL false
#endif

uint8_t int8Received;
uint16_t int16Received;
float floatReceived;
bool boolReceived;

int numMessagesReceived[4] = {0,0,0,0};

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency){};

void CANDecoder::readHandler(CAN_message_t msg) {
   switch (msg.id) {
      case (RECEIVE_BASE_ID + 0):
         int8Received = *(int8_t*)msg.buf;
         numMessagesReceived[0]++;
         break;
      case (RECEIVE_BASE_ID + 1):
         int16Received = *(int16_t*)msg.buf;
         numMessagesReceived[1]++;
         break;
      case (RECEIVE_BASE_ID + 2):
         floatReceived = *(float*)msg.buf;
         numMessagesReceived[2]++;
         break;
      case (RECEIVE_BASE_ID + 3):
         boolReceived = *(bool*)msg.buf;
         numMessagesReceived[3]++;
         break;
   }   
}

void CANDecoder::sendSignal() {
   uint8_t test8int = INT8_VAL; 
   this->sendMessage(SEND_BASE_ID + 0, (void*)&test8int, sizeof(test8int));

   uint16_t test16int = INT16_VAL;
   this->sendMessage(SEND_BASE_ID + 1, (void*)&test16int, sizeof(test16int));

   float testFloat = FLOAT_VAL;
   this->sendMessage(SEND_BASE_ID + 2, (void*)&testFloat, sizeof(testFloat));

   bool testBoolean = BOOL_VAL;
   this->sendMessage(SEND_BASE_ID + 3, (void*)&testBoolean, sizeof(testBoolean));
}