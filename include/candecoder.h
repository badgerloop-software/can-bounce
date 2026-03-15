#ifndef __CAN_DECODER_H__
#define __CAN_DECODER_H__

#include "canmanager.h"

extern uint8_t counter_messages;
class CANDecoder : public CANManager {
public:
  CANDecoder(CAN_TypeDef *canPort, CAN_PINS pins,
             int frequency = DEFAULT_CAN_FREQ);

  /* Reads input message and does any logic handling needed
   * Intended to be implemented by class extension per board
   */
  void readHandler(CAN_message_t msg);

  /* Send all of this board's message over CAN
   */
  void sendSignal();
};

struct Steering_Data {
  // Byte 0 — matches 0x300 CAN signal bit layout:
  bool headlight : 1;        // bit 0  (HEADL_TELEM)
  bool left_blink : 1;       // bit 1  (LEFT_BLINK)
  bool right_blink : 1;      // bit 2  (RIGHT_BLINK)
  bool direction_switch : 1; // bit 3  (DIR_SW_TELEM)
  bool horn : 1;             // bit 4  (HORN_TELEM)
  bool crz_mode_a : 1;       // bit 5  (CRZ_MODE_A)
  bool crz_set : 1;          // bit 6  (CRZ_SET_TELEM)
  bool crz_reset : 1;        // bit 7  (CRZ_RESET_TELEM)
  // Byte 1 — testing extensions, not in car CAN spec:
  bool park_brake : 1; // byte 1 bit 0
  bool mc_on : 1;      // byte 1 bit 1
};

extern volatile Steering_Data steering_data;

struct DigitalData {
  bool direction : 1;    // output
  bool mc_speed_sig : 1; // input
  bool eco_mode : 1;     // output
  bool mcu_mc_on : 1;    // output/state (was mc_on — renamed to match PDC's
                         // Digital_Data)
  bool park_brake : 1;   // input
  bool brake_led : 1;    // output (derived from brake sensor)
};

extern volatile DigitalData digital_data;

extern volatile float simAccIn;

extern volatile int numMessagesReceived[10];
extern volatile float messageReceived[10];

#endif