#ifndef CANDECODER_H
#define CANDECODER_H

#include "canmanager.h"

// CAN Message IDs
#define STEERING_ID 0x300
#define HAZARDS_ID 0x304
#define BPS_ID 0x100
#define PDC_ID 0x207

// Bit positions for Steering message (0x300)
#define STEERING_HEADLIGHT_BIT      0
#define STEERING_LEFT_BLINK_BIT     1
#define STEERING_RIGHT_BLINK_BIT    2

// Bit positions for Hazards message (0x304)
#define HAZARDS_BIT                 0

// Bit positions for BPS message (0x100)
#define BPS_FAULT_BIT               0

// Bit positions for PDC message (0x207)
#define PDC_BRAKE_LED_BIT           4

// Data structures
struct Steering_Data {
    uint8_t headlight : 1;
    uint8_t left_blink : 1;
    uint8_t right_blink : 1;
    uint8_t reserved : 5;
    
    // Helper to pack into byte
    uint8_t pack() const {
        return (headlight << STEERING_HEADLIGHT_BIT) |
               (left_blink << STEERING_LEFT_BLINK_BIT) |
               (right_blink << STEERING_RIGHT_BLINK_BIT);
    }
    
    // Helper to unpack from byte
    void unpack(uint8_t data) {
        headlight = (data >> STEERING_HEADLIGHT_BIT) & 0x01;
        left_blink = (data >> STEERING_LEFT_BLINK_BIT) & 0x01;
        right_blink = (data >> STEERING_RIGHT_BLINK_BIT) & 0x01;
    }
};

struct Hazards_Data {
    uint8_t hazards : 1;
    uint8_t reserved : 7;
    
    uint8_t pack() const {
        return (hazards << HAZARDS_BIT);
    }
    
    void unpack(uint8_t data) {
        hazards = (data >> HAZARDS_BIT) & 0x01;
    }
};

struct PDC_Data {
    uint8_t brakeLED : 1;
    uint8_t reserved : 7;
    
    uint8_t pack() const {
        return (brakeLED << PDC_BRAKE_LED_BIT);
    }
    
    void unpack(uint8_t data) {
        brakeLED = (data >> PDC_BRAKE_LED_BIT) & 0x01;
    }
};

extern Steering_Data steering;
extern Hazards_Data hazards;
extern PDC_Data pdc;
extern uint8_t bps_fault;
extern uint8_t num_msg_received;
extern bool sendsuccess;

class CANDecoder : public CANManager {
public:
    CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency);
    void readHandler(CAN_message_t msg) override;
    void sendSignal();
};

#endif // CANDECODER_H