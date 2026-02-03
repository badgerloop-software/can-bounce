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
   bool estop_mcu : 1;         // input
   bool batt_pos_cont : 1;          // input
   bool ppc1_supp_invalid : 1;             // input
   bool ppc1_dcdc_invalid : 1;        // input
   bool mppt_cont_mcu : 1;          // input
   bool mc_cont_mcu : 1;          // input
   bool mcu_batt_en : 1;          // input
};

 
 extern volatile DigitalData digital_data;
 
 extern volatile int numMessagesReceived[6];
 extern volatile float messageReceived[6];

#endif