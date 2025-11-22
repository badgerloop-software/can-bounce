#include "candecoder.h"

#define IS_FIRST_CAN false

Steering_Data steering;
float regen_brake;
bool send_success;
uint16_t messages_read;

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) : CANManager(canPort, pins, frequency){};

void CANDecoder::readHandler(CAN_message_t msg) {
   messages_read++;
   switch (msg.id) {
      case 0x300:
         steering = *(Steering_Data*)msg.buf;
         break;
      case 0x301:
         regen_brake = *(float*)msg.buf;
         break;
      default:
         break;
   }
}

void CANDecoder::sendSignal() {
   float r = ((float)rand() / RAND_MAX) * 100.0f;
   send_success = this->sendMessage(0x201, (void*)&r, sizeof(float));
   printf("Random float: %f\n", r);
}