#ifndef CANDECODER_H
#define CANDECODER_H

#include "canmanager.h"

// CAN Message IDs
#define STEERING_ID 0x300
#define HAZARDS_ID 0x304    
#define BMS_ID 0x100
#define PDC_ID 0x207

// Bit positions for Steering message (0x300)
#define STEERING_HEADLIGHT_BIT      0
#define STEERING_LEFT_BLINK_BIT     1
#define STEERING_RIGHT_BLINK_BIT    2
#define STEERING_DIRECTION_SWITCH_BIT 3
#define STEERING_HORN_BIT          4
#define STEERING_CRZ_MODE_A_BIT    5
#define STEERING_CRZ_SET_BIT       6
#define STEERING_CRZ_RESET_BIT     7

// Bit positions for Hazards message (0x304)
#define HAZARDS_BIT                 0

// Bit positions for BMS message (0x100)
#define BMS_FAULT_BIT               0

// Bit positions for PDC message (0x207)
#define PDC_DIRECTION_BIT           0
#define PDC_MC_SPEED_SIG_BIT       1
#define PDC_ECO_MODE_BIT           2
#define PDC_MC_ON_BIT              3
#define PDC_PARK_BRAKE_BIT         4
#define PDC_BRAKE_LED_BIT          5

// Data structures
struct Steering_Data {
    uint8_t headlight : 1;
    uint8_t left_blink : 1;
    uint8_t right_blink : 1;
    uint8_t direction_switch : 1;
    uint8_t horn: 1;
    uint8_t crz_mode_a: 1;
    uint8_t crz_set: 1;
    uint8_t crz_reset: 1;
    
    // Helper to pack into byte
    uint8_t pack() const {
        return (headlight << STEERING_HEADLIGHT_BIT) |
               (left_blink << STEERING_LEFT_BLINK_BIT) |
               (right_blink << STEERING_RIGHT_BLINK_BIT) |
               (direction_switch << STEERING_DIRECTION_SWITCH_BIT) |
               (horn << STEERING_HORN_BIT) |
               (crz_mode_a << STEERING_CRZ_MODE_A_BIT) |
               (crz_set << STEERING_CRZ_SET_BIT) |
               (crz_reset << STEERING_CRZ_RESET_BIT);
    }
    
    // Helper to unpack from byte
    void unpack(uint8_t data) {
        headlight = (data >> STEERING_HEADLIGHT_BIT) & 0x01;
        left_blink = (data >> STEERING_LEFT_BLINK_BIT) & 0x01;
        right_blink = (data >> STEERING_RIGHT_BLINK_BIT) & 0x01;
        direction_switch = (data >> STEERING_DIRECTION_SWITCH_BIT) & 0x01;
        horn = (data >> STEERING_HORN_BIT) & 0x01;
        crz_mode_a = (data >> STEERING_CRZ_MODE_A_BIT) & 0x01;
        crz_set = (data >> STEERING_CRZ_SET_BIT) & 0x01;
        crz_reset = (data >> STEERING_CRZ_RESET_BIT) & 0x01; 
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
    uint8_t direction : 1;
    uint8_t mc_speed_sig: 1;
    uint8_t eco_mode: 1;
    uint8_t mc_on: 1;
    uint8_t park_brake: 1;
    uint8_t brakeLED: 1;
    uint8_t reserved : 2;
    
    uint8_t pack() const {
        return (brakeLED << PDC_BRAKE_LED_BIT) |
               (park_brake << PDC_PARK_BRAKE_BIT) |
               (mc_on << PDC_MC_ON_BIT) |
               (eco_mode << PDC_ECO_MODE_BIT) |
               (mc_speed_sig << PDC_MC_SPEED_SIG_BIT) |
               (direction << PDC_DIRECTION_BIT);
    }
    
    void unpack(uint8_t data) {
        brakeLED = (data >> PDC_BRAKE_LED_BIT) & 0x01;
        park_brake = (data >> PDC_PARK_BRAKE_BIT) & 0x01;
        mc_on = (data >> PDC_MC_ON_BIT) & 0x01;
        eco_mode = (data >> PDC_ECO_MODE_BIT) & 0x01;
        mc_speed_sig = (data >> PDC_MC_SPEED_SIG_BIT) & 0x01;
        direction = (data >> PDC_DIRECTION_BIT) & 0x01;
    }
};

extern Steering_Data steering;
extern Hazards_Data hazards;
extern PDC_Data pdc;
extern uint8_t bms_fault;
extern uint8_t num_msg_received;
extern bool sendsuccess;

class CANDecoder : public CANManager {
public:
    CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency);
    void readHandler(CAN_message_t msg) override;
    void sendSignal();
};

#endif // CANDECODER_H