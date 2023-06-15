#include "mbed.h"

CAN canBus(PD_0, PD_1);     // DEFAULT: For use with F767ZI
//CAN canBus(PA_11, PA_12);   // For use with F303K8 or L432KC

// main() runs in its own thread in the OS
int main()
{
    CANMessage msg;
    canBus.frequency(500000);
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
            wait_us(1000);
        }
    }
}