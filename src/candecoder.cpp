#include "candecoder.h"

float    floatReceived;
bool     boolReceived;
uint8_t  counter_messages = 0;

volatile int   numMessagesReceived[10] = {0,0,0,0,0,0,0,0,0,0};
volatile float messageReceived[10]     = {0,0,0,0,0,0,0,0,0,0};

volatile DigitalData   digital_data;
volatile Steering_Data steering_data;

volatile float testAccel     = 0.0f;
volatile float testRegen     = 0.0f;
volatile bool  testEco       = false;
volatile bool  testDirection = false;
volatile bool  testMode      = false;

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency)
    : CANManager(canPort, pins, frequency) {};

void CANDecoder::readHandler(CAN_message_t msg) {
    counter_messages++;
    int offset = msg.id - 0x200;
    if (offset >= 0 && offset <= 10) {
        numMessagesReceived[offset]++;
        switch (offset) {
            case 0:  // acc_out
            case 1:  // regen_brake
            case 2:  // lv_12V_telem
            case 3:  // lv_5V_telem
            case 4:  // lv_5V_current
            case 5:  // current_in_telem
            case 6:  // brake_pressure_telem
            case 8:  // mph
                floatReceived = *((float*)msg.buf);
                messageReceived[offset] = floatReceived;
                break;
            case 7:  // digital_data
                memcpy((void*)&digital_data, msg.buf, sizeof(DigitalData));
                break;
            case 9:  // acc_in
                boolReceived = *((float*)msg.buf);
                messageReceived[offset] = boolReceived;
                break;
            default:
                break;
        }
    }
}

void CANDecoder::sendSignal() {
    this->sendMessage(0x300,               (void*)&steering_data, sizeof(Steering_Data));
    this->sendMessage(TEST_MODE_ENABLE_ID, (void*)&testMode,      sizeof(bool));
    this->sendMessage(TEST_ACCEL_ID,       (void*)&testAccel,     sizeof(float));
    this->sendMessage(TEST_REGEN_ID,       (void*)&testRegen,     sizeof(float));
    this->sendMessage(TEST_ECO_ID,         (void*)&testEco,       sizeof(bool));
    this->sendMessage(TEST_DIRECTION_ID,   (void*)&testDirection, sizeof(bool));
}