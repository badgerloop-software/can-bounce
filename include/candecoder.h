#ifndef __CAN_DECODER_H__
#define __CAN_DECODER_H__

#include <Arduino.h>
#include "canmanager.h"

#define STEERING_ID 0x300
#define HAZARDS_ID 0x304
#define PDC_ID 0x207
#define BPS_ID 0x100

#define BLINK_DELAY_MS 400

#define STEERING_HEADLIGHT_BIT 0
#define STEERING_LEFT_BLINK_BIT 1
#define STEERING_RIGHT_BLINK_BIT 2

#define PDC_DIRECTION_BIT 0
#define PDC_BRAKE_LED_BIT 5

#define BPS_FAULT_BIT 0

inline bool getBlinkPhase() {
    return ((millis() / BLINK_DELAY_MS) % 2) != 0;
}

struct Steering_Data {
    bool headlight;
    bool left_blink;
    bool right_blink;
};

struct PDC_Data {
    bool direction;
    bool brake_led;
};

extern uint8_t num_msg_received;
extern bool sendsuccess;

extern Steering_Data steering;
extern bool hazards;
extern PDC_Data pdc;
extern uint8_t bps_fault;

class CANDecoder : public CANManager {
public:
    CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency = DEFAULT_CAN_FREQ);
    void readHandler(CAN_message_t msg);
    void sendSignal();
};

#endif
