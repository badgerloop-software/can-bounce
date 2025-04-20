#ifndef __CAN_DECODER_H__
#define __CAN_DECODER_H__

#include "canmanager.h"

extern uint8_t counter_messages;
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

struct Steering_Data {
   bool direction_switch : 1;      // input
   bool left_blink : 1;            // input
   bool right_blink : 1;           // input
   bool crz_mode_a : 1;            // input
   bool crz_set : 1;               // input
   bool crz_reset : 1;             // input
   bool horn : 1;                  // input
};

extern volatile Steering_Data steering_data;

struct DigitalData {
   bool direction : 1;         // output
   bool eco_mode : 1;          // output
   bool mc_on : 1;             // input
   bool park_brake : 1;        // input
   bool brakeLED : 1;          // derived by state machine
};

 
 extern volatile DigitalData digital_data;
 
 extern volatile int numMessagesReceived[10];
 extern volatile float messageReceived[10];

#endif