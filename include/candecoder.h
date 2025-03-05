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

typedef struct{
bool direction : 1;         // output
bool mc_speed_sig : 1;      // input
bool eco_mode : 1;          // output
bool mcu_mc_on : 1;         // output
bool park_brake : 1;        // input
} DigitalData;

extern volatile DigitalData digital_data;

extern volatile int numMessagesReceived[9];
extern volatile float messageReceived[8];

// External volatile declarations for CAN data
extern volatile float acc_out;
extern volatile float regen_brake;
extern volatile float lv_12V_telem;
extern volatile float lv_5V_telem;
extern volatile float lv_5V_current;
extern volatile float current_in_telem;
extern volatile float brake_pressure_telem;
extern volatile bool brakeLED;

#endif