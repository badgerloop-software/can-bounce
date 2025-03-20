#include "candecoder.h"

#include <stdlib.h>
#include <time.h>

uint8_t num_msg_received = 0;

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency){};

void CANDecoder::readHandler(CAN_message_t msg) {
   num_msg_received++;
}

bool sendsuccess;

bool led1 = false;
bool led2 = false;
Digital_Data led1msg = {0, led1, 0, 0, 0, 0, 0};

void CANDecoder::sendSignal() {
   sendsuccess = true;
   sendsuccess &= this->sendMessage(LED_ID_1, (void*)&led1msg, sizeof(Digital_Data));
   sendsuccess &= this->sendMessage(LED_ID_2, (void*)&led2, sizeof(bool));
   if (!sendsuccess) {
      reset();
   }
}