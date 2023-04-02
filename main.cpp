#include "mbed.h"

#define DEVICE_MASTER 0

// Define CAN pins
// Assuming F767ZI will be master
#if DEVICE_MASTER
CAN canBus(PD_0, PD_1);
#else
CAN canBus(PA_11, PA_12);
#endif

// main() runs in its own thread in the OS
int main()
{
    CANMessage msg;
    canBus.frequency(500000);

    while (true) {
        #if DEVICE_MASTER
        char mes = 255;
        wait_us(1000000);
        canBus.write(CANMessage(0, &mes, 1));
        printf("Master has sent first message\n");
        #else
        char mes = 255;
        printf("Slave is waiting for message\n");
        #endif


        while (true) {
            if(canBus.read(msg)) {
                printf("Message received: %d\n", msg.data[0]);
                wait_us(1000000);
                printf("Bouncing!\n");
                if (!canBus.write(CANMessage(0, &mes, 1))) {
                    printf("Unsuccessful sending message!\n");
                }
            }

            if (canBus.tderror()) {
                printf("Transmit error detected, resetting CAN...\n");
                canBus.reset();
                wait_us(1000);
                break;
            }
        }
    }
}

