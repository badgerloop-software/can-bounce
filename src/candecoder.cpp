#include "candecoder.h"
#include "canmanager.h"

bool g_led_on = false;

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency)
    : CANManager(canPort, pins, frequency) {}

void CANDecoder::readHandler(CAN_message_t msg) {
    (void)msg;
}

void CANDecoder::sendSignal() {
    sendMessage(CAN_ID_LED, g_led_on, sizeof(g_led_on), 1);
}
