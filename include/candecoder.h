#include "canmanager.h"
 
class CANDecoder : public CANManager {
   public:

      CANDecoder(PinName rd, PinName td, int frequency = DEFAULT_CAN_FREQ);

      /* Reads input message and does any logic handling needed
       * Intended to be implemented by class extension per board
       */
      virtual void readHandler(CAN_data msg) = 0;

      /* Send a message over CAN
       *
       * messageID: CAN ID to use to identify the signal
       * data: Payload array
       * length: Size of data in bytes
       * timeout: in milliseconds
       */ 
      int sendSignal();
};