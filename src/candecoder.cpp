#include "candecoder.h"

uint8_t num_msg_received = 0;
bool sendsuccess;

Steering_Data steering;
PDC_Data pdc;
uint8_t bps_fault = 0;

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency){};

void CANDecoder::readHandler(CAN_message_t msg) {
   num_msg_received++;
}

void CANDecoder::sendSignal() {
   sendsuccess = true;
   sendsuccess &= this->sendMessage(STEERING_ID, (void*)&steering, sizeof(Steering_Data));
   sendsuccess &= this->sendMessage(PDC_ID, (void*)&pdc, sizeof(PDC_Data));
   sendsuccess &= this->sendMessage(BPS_ID, (void*)&bps_fault, sizeof(uint8_t));
   if (!sendsuccess) {
      reset();
   }
}