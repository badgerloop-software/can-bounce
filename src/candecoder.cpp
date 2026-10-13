#include "candecoder.h"

bool g_led_on = false;

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency)
    : CANManager(canPort, pins, frequency) {}

void CANDecoder::readHandler(CAN_message_t msg) {
    (void)msg;
}

void CANDecoder::sendSignal() {
    // TODO: send g_led_on on CAN_ID_LED
    uint8_t data = g_led_on ? 1 : 0;
    sendMessage(CAN_ID_LED, &data, sizeof(data));
}
