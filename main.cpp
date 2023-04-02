#include "mbed.h"

// Need to flash every device, numbered 0 to TOTAL - 1
#define TOTAL 2
#define DEVICE 0

// Define CAN pins
// Pick first for big, second for mini
#if !DEVICE
CAN canBus(PD_0, PD_1);
#else
CAN canBus(PA_11, PA_12);
#endif

// main() runs in its own thread in the OS
int main()
{
    CANMessage msg;
    

    while (true) {
        #if !DEVICE
        char mes = 255;
        wait_us(1000000);
        canBus.write(CANMessage(DEVICE + 1, &mes, 1));
        printf("Master has sent first message\n");
        #else
        char mes = 255;
        printf("Slave is waiting for message\n");
        #endif
        canBus.frequency(125000);

        while (true) {
            if(canBus.read(msg)) {
                if (msg.id != DEVICE) continue;

                printf("Message received: %d\n", msg.data[0]);
                wait_us(1000000);
                printf("Bouncing!\n");
                if (!canBus.write(CANMessage((DEVICE + 1) % TOTAL, &mes, 1))) {
                    printf("Unsuccessful sending message!\n");
                } else {
                    mes--;
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

