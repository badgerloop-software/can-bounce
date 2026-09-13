#ifndef __CAN_DECODER_H__
#define __CAN_DECODER_H__

#include "canmanager.h"

#define CAN_ID_LED  0x600

extern bool g_led_on;

class CANDecoder : public CANManager {
public:
    CANDecoder(CAN_TypeDef* canPort, CAN_PINS pins, int frequency = DEFAULT_CAN_FREQ);

    void readHandler(CAN_message_t msg);
    void sendSignal();
};

#endif
