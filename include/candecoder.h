#ifndef __CAN_DECODER_H__
#define __CAN_DECODER_H__

#include "canmanager.h"

// Test mode CAN IDs (must match PDC)
#define TEST_ACCEL_ID           0x310
#define TEST_REGEN_ID           0x311
#define TEST_ECO_ID             0x312
#define TEST_DIRECTION_ID       0x313
#define TEST_MODE_ENABLE_ID     0x314

extern uint8_t counter_messages;

class CANDecoder : public CANManager {
   public:
      CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency = DEFAULT_CAN_FREQ);
      void readHandler(CAN_message_t msg);
      void sendSignal();
};

struct Steering_Data {
   bool direction_switch : 1;
   bool left_blink : 1;
   bool right_blink : 1;
   bool crz_mode_a : 1;
   bool crz_set : 1;
   bool crz_reset : 1;
   bool horn : 1;
};

extern volatile Steering_Data steering_data;

struct DigitalData {
   bool direction : 1;
   bool mc_speed_sig : 1;  // add this
   bool eco_mode : 1;
   bool mc_on : 1;     // rename to match
   bool park_brake : 1;
   bool brakeLED : 1;      // this is sent separately via 0x20A so you can drop it
};

extern volatile DigitalData digital_data;

extern volatile int   numMessagesReceived[10];
extern volatile float messageReceived[10];

// Test control state
extern volatile float testAccel;
extern volatile float testRegen;
extern volatile bool  testEco;
extern volatile bool  testDirection;
extern volatile bool  testMode;

#endif