#include "candecoder.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(can_bounce, LOG_LEVEL_INF);

CANDecoder::CANDecoder(const struct device * canDevice, const uint32_t targetIDList[], size_t targetIDListSize, uint32_t frequency) : 
    ZephyrCAN(canDevice, targetIDList, targetIDListSize, frequency) {}

void CANDecoder::readHandler(struct can_frame * msg) {
    if ((msg->id) == EXAMPLE_ID) {
    }
}

int CANDecoder::sendSignal() {
    return 0;
}
