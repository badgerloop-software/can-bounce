#include "mbed.h"

#define DEVICE_MASTER 0

// Define CAN pins
CAN canBus(PA_11, PA_12);


// main() runs in its own thread in the OS
int main()
{
    CANMessage msg;
    canBus.frequency(115200);


    while (true) {
        #if DEVICE_MASTER
        wait_us(1000000);
        char mes = 255;
        canBus.write(CANMessage(0, &mes, 1));
        printf("Master has sent first message\n");
        #else
        char mes = 0;
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
                break;
            }
        }
    }
}

