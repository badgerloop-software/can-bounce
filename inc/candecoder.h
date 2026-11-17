#ifndef __CAN_DECODER_H__
#define __CAN_DECODER_H__

#include "zephyrcan.h"

#define EXAMPLE_ID 0xAB

extern uint8_t counter_messages;
class CANDecoder : public ZephyrCAN {
   public:
   
      CANDecoder(const struct device *canDevice, const uint32_t targetIDList[], size_t targetIDListSize, uint32_t frequency = CAN_FREQUENCY);

      /* Reads input message and does any logic handling needed
       * Intended to be implemented by class extension per board
       */
      void readHandler(struct can_frame * msg);

      /* Send all of this board's message over CAN
       */ 
      int sendSignal();
};

#endif