#include "candecoder.h"

uint8_t num_msg_received = 0;
bool sendsuccess = false;

Steering_Data steering = {false, false, false};
bool hazards = false;
PDC_Data pdc = {false, false};
uint8_t bps_fault = 0;

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency)
    : CANManager(canPort, pins, frequency) {
}

void CANDecoder::readHandler(CAN_message_t msg) {
    num_msg_received++;
}

void CANDecoder::sendSignal() {
    bool phase = getBlinkPhase();
    bool left_lamp;
    bool right_lamp;

    if (hazards) {
        left_lamp = phase;
        right_lamp = phase;
    } else {
        left_lamp = steering.left_blink && phase;
        right_lamp = steering.right_blink && phase;
    }

    uint8_t steering_data = 0;
    steering_data |= (steering.headlight ? 1U : 0U) << STEERING_HEADLIGHT_BIT;
    steering_data |= (left_lamp ? 1U : 0U) << STEERING_LEFT_BLINK_BIT;
    steering_data |= (right_lamp ? 1U : 0U) << STEERING_RIGHT_BLINK_BIT;

    uint8_t pdc_data = 0;
    pdc_data |= (pdc.direction ? 1U : 0U) << PDC_DIRECTION_BIT;
    pdc_data |= (pdc.brake_led ? 1U : 0U) << PDC_BRAKE_LED_BIT;

    uint8_t bps_data = (bps_fault << BPS_FAULT_BIT);

    uint8_t hazard_blink = (hazards && phase) ? 1U : 0U;

    sendsuccess = sendMessage(STEERING_ID, &steering_data, 1);
    sendsuccess &= sendMessage(HAZARDS_ID, &hazard_blink, 1);
    sendsuccess &= sendMessage(BPS_ID, &bps_data, 1);
    sendsuccess &= sendMessage(PDC_ID, &pdc_data, 1);

    if (!sendsuccess) {
        reset();
    }
}
