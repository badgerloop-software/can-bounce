#include "canmanager.h"
 
class CANDecoder : public CANManager {
   public:
   
      CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency = DEFAULT_CAN_FREQ);

      /* Reads input message and does any logic handling needed
       * Intended to be implemented by class extension per board
       */
      void readHandler(CAN_data msg);

      /* Send a message over CAN
       *
       * messageID: CAN ID to use to identify the signal
       * data: Payload array
       * length: Size of data in bytes
       * timeout: in milliseconds
       */ 
      void sendSignal();
};