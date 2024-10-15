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