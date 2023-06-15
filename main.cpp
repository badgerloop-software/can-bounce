#include "mbed.h"

CAN canBus(PD_0, PD_1);     // DEFAULT: For use with F767ZI
//CAN canBus(PA_11, PA_12);   // For use with F303K8 or L432KC

// main() runs in its own thread in the OS
int main()
{
    CANMessage msg;
    canBus.frequency(500000);
    while (true) {
        if(canBus.read(msg)) {
            printf("Message received: %d\n", msg.data[0]);
        }

        if (canBus.tderror() | canBus.rderror()) {
            canBus.reset();
            wait_us(1000);
        }
    }
}