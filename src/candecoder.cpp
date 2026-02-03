#include "candecoder.h"

float floatReceived;
bool boolReceived;
uint8_t counter_messages = 0; 

volatile int numMessagesReceived[6] = {0,0,0,0,0,0};
volatile float messageReceived[6] = {0,0,0,0,0,0};

volatile DigitalData digital_data;
volatile Steering_Data steering_data;

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency){};

void CANDecoder::readHandler(CAN_message_t msg) {
   counter_messages++;
   int offset = msg.id - 0x500; // Update the base ID to 0x500
   Serial.printf("Message ID: %d\n", msg.id);
   if (offset >= 0 && offset <= 5) { // Adjust the range to match the new IDs
      numMessagesReceived[offset]++;
      switch (offset){
         case 0: // i_12
            floatReceived = *((float*)msg.buf);
            messageReceived[offset] = floatReceived;
            break;
         case 1: // v_12v
            floatReceived = *((float*)msg.buf);
            messageReceived[offset] = floatReceived;
            break;
         case 2: // supp_i
            floatReceived = *((float*)msg.buf);
            messageReceived[offset] = floatReceived;
            break;
         case 3: // batt_i
            floatReceived = *((float*)msg.buf);
            messageReceived[offset] = floatReceived;
            break;
         case 4: // supp_v
            floatReceived = *((float*)msg.buf);
            messageReceived[offset] = floatReceived;
            break;
         case 5: // digital_data
            memcpy((void *)&digital_data, msg.buf, sizeof(DigitalData));
            break;
         default:
            // Handle unexpected offsets if necessary
            break;
      }
   }
}

void CANDecoder::sendSignal() {
   this->sendMessage(0x300, (void*)&steering_data, sizeof(Steering_Data));
}