#include "mbed.h"

CAN canBus(PD_0, PD_1);

// main() runs in its own thread in the OS
int main()
{
    CANMessage msg;
    canBus.frequency(115200);
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