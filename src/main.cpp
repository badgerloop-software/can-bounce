#include <stdio.h>
#include <stdint.h>
#include <zephyr/logging/log.h>
#include "candecoder.h"
#include "updatemanager_can.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct device * const canDev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));

static const uint32_t UPDATE_ID = 0xAA;
static const uint32_t CAN_IDS[] = {EXAMPLE_ID};

int main(void) {
    LOG_INF("Hello World DEVELOP----- %s\n", CONFIG_BOARD_TARGET);
    CANDecoder canBus(canDev, CAN_IDS, sizeof(CAN_IDS)/sizeof(CAN_IDS[0]));
    canBus.begin();

    if (canBus.canStatus() != CAN_OK) {
        LOG_ERR("CAN init failed: %d", canBus.canStatus());
        return -1;
    }
    
    while(1) {
        if (canBus.canStatus() != CAN_OK)
        {
            break;
        }

        k_msleep(1000);
    }
    
    return 0;
}

K_THREAD_DEFINE(update_manager_can, 4096, canUpdateTask, (void *)UPDATE_ID, NULL, NULL, 7, 0, 0);
