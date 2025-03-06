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

}
bool sendsuccess;

void CANDecoder::sendSignal() {
   sendsuccess = true;
   sendsuccess &= this->sendMessage(0x200, (void*)&acc_out, sizeof(float));
   sendsuccess &= this->sendMessage(0x201, (void*)&regen_brake, sizeof(float));
   sendsuccess &= this->sendMessage(0x202, (void*)&lv_12V_telem, sizeof(float));
   sendsuccess &= this->sendMessage(0x203, (void*)&lv_5V_telem, sizeof(float));
   sendsuccess &= this->sendMessage(0x204, (void*)&lv_5V_current, sizeof(float));
   sendsuccess &= this->sendMessage(0x205, (void*)&current_in_telem, sizeof(float));
   sendsuccess &= this->sendMessage(0x206, (void*)&brake_pressure_telem, sizeof(float));
   sendsuccess &= this->sendMessage(0x207, (void*)&digital_data, sizeof(digital_data));
   sendsuccess &= this->sendMessage(0x20A, (void*)&brakeLED, sizeof(bool));
   if (!sendsuccess) {
      reset();
   }
}