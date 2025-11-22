#ifndef __CAN_DECODER_H__
#define __CAN_DECODER_H__

#include "canmanager.h"

struct Steering_Data {
    bool headlight : 1;             // input
    bool left_blink : 1;            // input
    bool right_blink : 1;           // input
    bool direction_switch : 1;      // input
    bool horn : 1;                  // input
    bool crz_mode_a : 1;            // input
    bool crz_set : 1;               // input
    bool crz_reset : 1;             // input
};

extern Steering_Data steering;
extern float regen_brake;

class CANDecoder : public CANManager {
public:

   CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency = DEFAULT_CAN_FREQ);

   /* Reads input message and does any logic handling needed
      * Intended to be implemented by class extension per board
      */
   void readHandler(CAN_message_t msg);

   /* Send all of this board's message over CAN
      */ 
   void sendSignal();
};

#endif