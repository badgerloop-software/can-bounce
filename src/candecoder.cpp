#include "candecoder.h"

// TODO: PDC firmware must be updated to read mc_on and park_brake from
//       this CAN instead of from physical switch inputs,
//       so the test board can control them over CAN, mc_on will not be on can

float floatReceived;
bool boolReceived;
uint8_t counter_messages = 0;

volatile int numMessagesReceived[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile float messageReceived[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

volatile float simAccIn = 0.0f;
volatile DigitalData digital_data;
volatile Steering_Data steering_data;

CANDecoder::CANDecoder(CAN_TypeDef *canPort, CAN_PINS pins, int frequency)
    : CANManager(canPort, pins, frequency) {};

void CANDecoder::readHandler(CAN_message_t msg) {
  counter_messages++;
  int offset = msg.id - 0x200; // Update the base ID to 0x200
                               // Serial.printf("Message ID: %d\n", msg.id);
  if (offset >= 0 && offset < 10) { // Adjust the range to match the new IDs
    numMessagesReceived[offset]++;
    switch (offset) {
    case 0: // acc_out
      memcpy(&floatReceived, msg.buf, sizeof(float));
      messageReceived[offset] = floatReceived;
      break;
    case 1: // regen_brake
      memcpy(&floatReceived, msg.buf, sizeof(float));
      messageReceived[offset] = floatReceived;
      break;
    case 2: // lv_12V_telem
      memcpy(&floatReceived, msg.buf, sizeof(float));
      messageReceived[offset] = floatReceived;
      break;
    case 3: // lv_5V_telem
      memcpy(&floatReceived, msg.buf, sizeof(float));
      messageReceived[offset] = floatReceived;
      break;
    case 4: // lv_5V_current
      memcpy(&floatReceived, msg.buf, sizeof(float));
      messageReceived[offset] = floatReceived;
      break;
    case 5: // current_in_telem
      memcpy(&floatReceived, msg.buf, sizeof(float));
      messageReceived[offset] = floatReceived;
      break;
    case 6: // brake_pressure_telem
      memcpy(&floatReceived, msg.buf, sizeof(float));
      messageReceived[offset] = floatReceived;
      break;
    case 7: // digital_data
      memcpy((void *)&digital_data, msg.buf,
             sizeof(DigitalData) < 8 ? sizeof(DigitalData) : 8);
      break;
    case 8: // mph
      memcpy(&floatReceived, msg.buf, sizeof(float));
      messageReceived[offset] = floatReceived;
      break;
    case 9: // acc_in
      memcpy(&floatReceived, msg.buf, sizeof(float));
      messageReceived[offset] = floatReceived;
      break;
    default:
      // Handle unexpected offsets if necessary
      break;
    }
  }
}

void CANDecoder::sendSignal() {
  this->sendMessage(0x300, (void *)&steering_data, sizeof(Steering_Data));
  this->sendMessage(0x209, (void *)&simAccIn,
                    sizeof(float)); // simulate acceleration pedal
}