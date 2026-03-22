#include "candecoder.h"

// Initialize global variables
uint8_t num_msg_received = 0;
bool sendsuccess = false;

Steering_Data steering = {0, 0, 0, 0, 0, 0, 0, 0};
Hazards_Data hazards = {0, 0};
PDC_Data pdc = {0, 0, 0, 0, 0, 0};
uint8_t bms_fault = 0;

CANDecoder::CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency) 
    : CANManager(canPort, pins, frequency) {
}

void CANDecoder::readHandler(CAN_message_t msg) {
    num_msg_received++;
    
    switch(msg.id) {
        case STEERING_ID:
            if (msg.len >= 1) {
                steering.unpack(msg.buf[0]);
            }
            break;
            
        case HAZARDS_ID:
            if (msg.len >= 1) {
                hazards.unpack(msg.buf[0]);
            }
            break;
            
        case PDC_ID:
            if (msg.len >= 1) {
                pdc.unpack(msg.buf[0]);
            }
            break;
            
        default:
            // Unknown message ID
            break;
    }
}

void CANDecoder::sendSignal() {
    // Update hazards based on blinker state
    // hazards.hazards = (steering.left_blink && steering.right_blink) ? 1 : 0;
    
    // // Prepare data for each message
    // uint8_t steering_data = steering.pack();
    // uint8_t hazards_data = hazards.pack();
    // uint8_t bms_data = (bms_fault << BMS_FAULT_BIT);
    // uint8_t pdc_data = pdc.pack();
    
    // // Send Steering message (0x300)
    // sendsuccess = sendMessage(STEERING_ID, &steering_data, 1);
    
    // // Send Hazards message (0x304)
    // sendsuccess &= sendMessage(HAZARDS_ID, &hazards_data, 1);
    
    // // Send BMS fault message (0x100)
    // sendsuccess &= sendMessage(BMS_ID, &bms_data, 1);
    
    // // Send PDC message (0x207)
    // sendsuccess &= sendMessage(PDC_ID, &pdc_data, 1);
}