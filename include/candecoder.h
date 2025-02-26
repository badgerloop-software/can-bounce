#include "canmanager.h"
 
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