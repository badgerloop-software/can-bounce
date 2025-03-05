#include "candecoder.h"

#include <stdlib.h>
#include <time.h>

#define IS_FIRST_CAN false

uint8_t int8Received;
uint16_t int16Received;
float floatReceived;
bool boolReceived;
uint8_t counter_messages = 0; 

volatile int numMessagesReceived[9] = {0,0,1,0,0,0,0,0,0};
volatile float messageReceived[8] = {0,0,0,0,0,5.5,0,0};

volatile DigitalData digital_data;

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency){};

void CANDecoder::readHandler(CAN_message_t msg) {
   counter_messages++;
   int offset = msg.id - 0x200; // Update the base ID to 0x200
   if (offset >= 0 && offset <= 10) { // Adjust the range to match the new IDs
      numMessagesReceived[offset]++;
      switch (offset){
         case 0: // acc_out
            floatReceived = *((float*)msg.buf);
            messageReceived[offset] = floatReceived;
            break;
         case 1: // regen_brake
            floatReceived = *((float*)msg.buf);
            messageReceived[offset] = floatReceived;
            break;
         case 2: // lv_12V_telem
            floatReceived = *((float*)msg.buf);
            messageReceived[offset] = floatReceived;
            break;
         case 3: // lv_5V_telem
            floatReceived = *((float*)msg.buf);
            messageReceived[offset] = floatReceived;
            break;
         case 4: // lv_5V_current
            floatReceived = *((float*)msg.buf);
            messageReceived[offset] = floatReceived;
            break;
         case 5: // current_in_telem
            floatReceived = *((float*)msg.buf);
            messageReceived[offset] = floatReceived;
            break;
         case 6: // brake_pressure_telem
            floatReceived = *((float*)msg.buf);
            messageReceived[offset] = floatReceived;
            break;
         case 7: // digital_data
            memcpy((void *)&digital_data, msg.buf, sizeof(DigitalData));
            break;
         case 10: // brakeLED
            boolReceived = *((bool*)msg.buf);
            messageReceived[offset] = boolReceived;
            break;
         default:
            // Handle unexpected offsets if necessary
            break;
      }
   }
}

void CANDecoder::sendSignal() {
   this->sendMessage(0x200, (void*)&acc_out, sizeof(float));
   this->sendMessage(0x201, (void*)&regen_brake, sizeof(float));
   this->sendMessage(0x202, (void*)&lv_12V_telem, sizeof(float));
   this->sendMessage(0x203, (void*)&lv_5V_telem, sizeof(float));
   this->sendMessage(0x204, (void*)&lv_5V_current, sizeof(float));
   this->sendMessage(0x205, (void*)&current_in_telem, sizeof(float));
   this->sendMessage(0x206, (void*)&brake_pressure_telem, sizeof(float));
   this->sendMessage(0x207, (void*)&digital_data, sizeof(digital_data));
   this->sendMessage(0x20A, (void*)&brakeLED, sizeof(bool));
}