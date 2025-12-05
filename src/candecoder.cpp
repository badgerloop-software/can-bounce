#include "candecoder.h"

float floatReceived;
bool boolReceived;
uint8_t counter_messages = 0; 

volatile int numMessagesReceived[10] = {0,0,0,0,0,0,0,0,0,0};
volatile float messageReceived[10] = {0,0,0,0,0,0,0,0,0,0};

/////////////////////////////////////////////////////////////////
// Example definition of a struct to be sent/received over CAN
/////////////////////////////////////////////////////////////////

// volatile Steering_Data steering_data;

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency){};

void CANDecoder::readHandler(CAN_message_t msg) {
   counter_messages++;

   ////////////////////////////////////////////////////////////////////
   // Assuming the message IDs start from 0x200 for the first message
   ////////////////////////////////////////////////////////////////////

   // int offset = msg.id - 0x200;

   // Serial.printf("Message ID: %d\n", msg.id);
   
   numMessagesReceived[offset]++;

   switch (offset){
      case GIVE_THIS_MACRO_A_PROPER_VALUE_AND_NAME1: // This is how you would receive a float over CAN
         // floatReceived = *((float*)msg.buf);
         // messageReceived[offset] = floatReceived;

         /////////////////////////////////////////////////////////////////
         // Since you are sending a void* you will need to cast it back
         // to the appropriate type. Here we are assuming a float was sent
         /////////////////////////////////////////////////////////////////
         break;
      case GIVE_THIS_MACRO_A_PROPER_VALUE_AND_NAME2: // This is how you would receive a struct over CAN
         // memcpy((void *)&steering_data, msg.buf, sizeof(Steering_Data));

         //////////////////////////////////////////////////////////////
         // Ensure that the order of the fields in the struct matches
         // the order in which they are sent over CAN. Because this is 
         // a direct memory copy, the byte layout must be identical.
         //////////////////////////////////////////////////////////////
         break;
      default:
         // Handle unexpected offsets if necessary
         break;
   }
}

void CANDecoder::sendSignal() {
   // Example of sending a float over CAN
   // float floatToSend = 3.14f;
   // this->sendMessage(APPROPRIATE_MSG_ID, (void*)&floatToSend, sizeof(float));
   
   // Example of sending a struct over CAN
   // this->sendMessage(APPROPRIATE_MSG_ID, (void*)&steering_data, sizeof(Steering_Data));
}