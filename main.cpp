#include "mbed.h"

CAN canBus(PA_11, PA_12);

// main() runs in its own thread in the OS
int main()
{
    CANMessage msg;
    canBus.frequency(115200);
    char mes = 255;
    wait_us(1000000);
    while (true) {
        if (!canBus.write(CANMessage(0, &mes, 1))) {
            printf("Unsuccessful sending message!\n");
        } else {
            mes--;
            printf("Sent successfully\n");
        }
        wait_us(1000000);

        if (canBus.tderror()) {
            printf("Transmit error detected, resetting CAN...\n");
            canBus.reset();
        }
    }
}