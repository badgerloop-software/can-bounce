#include "candecoder.h"

#include <stdlib.h>
#include <time.h>

#define IS_FIRST_CAN false

uint8_t int8Received;
uint16_t int16Received;
float floatReceived;
bool boolReceived;

volatile int numMessagesReceived[9] = {0,0,0,0,0,0,0,0,0};
volatile float messageReceived[8] = {0,0,0,0,0,0,0,0};

volatile DigitalData digital_data;

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency){};

void CANDecoder::readHandler(CAN_message_t msg) {
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
            //Todo: check if this is correct
            //digital_data = *((DigitalData*)msg.buf);
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
   bool forwardAndReverse = rand() % 2;
   this->sendMessage(0x300, (void*)&forwardAndReverse, sizeof(bool));
}