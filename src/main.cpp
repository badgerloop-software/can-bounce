#include <Arduino.h>
#include "STM32_CAN.h"
#include "candecoder.h"

CANDecoder candecoder(CAN1, ALT_2, 250000);

uint8_t counter = 0;

void printMenu() {
    printf("\e[1;1H\e[2J");
    printf("=== PDC OUTPUT TEST  [mode: %s] ===\n", testMode ? "ON" : "OFF");
    printf("  T      : toggle test mode on/off\n");
    printf("  a / z  : accel   +1%% / -1%%\n");
    printf("  s / x  : regen   +1%% / -1%%\n");
    printf("  e      : toggle eco\n");
    printf("  d      : toggle direction (FWD/REV)\n");
    printf("-----------------------------------\n");
    printf("Accel:      %5.1f%%\n", (double)testAccel);
    printf("Regen:      %5.1f%%\n", (double)testRegen);
    printf("Eco:        %s\n",       testEco       ? "ON"  : "OFF");
    printf("Direction:  %s\n",       testDirection ? "REV" : "FWD");
    printf("-----------------------------------\n");
    printf("PDC feedback:\n");
    printf("  acc_out:         %f\n", messageReceived[0]);
    printf("  regen_brake:     %f\n", messageReceived[1]);
    printf("  lv_12V:          %f\n", messageReceived[2]);
    printf("  lv_5V:           %f\n", messageReceived[3]);
    printf("  lv_5V_current:   %f\n", messageReceived[4]);
    printf("  current_in:      %f\n", messageReceived[5]);
    printf("  brake_pressure:  %f\n", messageReceived[6]);
    printf("  Direction out:   %d\n", digital_data.direction);
    printf("  Eco out:         %d\n", digital_data.eco_mode);
    printf("  MC on:           %d\n", digital_data.mc_on);
    printf("  Park brake:      %d\n", digital_data.park_brake);
    printf("  Brake LED:       %d\n", digital_data.brakeLED);
}

void setup() {
    Serial.begin(115200);
}

void loop() {
    if (Serial.available() > 0) {
        char buf = Serial.read();
        while (Serial.available()) Serial.read();

        switch (buf) {
            case 'T': case 't':
                testMode = !testMode;
                if (!testMode) { testAccel = 0.0f; testRegen = 0.0f; }
                break;
            case 'a':
                if (testAccel < 100.0f) testAccel += 1.0f;
                break;
            case 'z':
                if (testAccel > 0.0f) testAccel -= 1.0f;
                break;
            case 's':
                if (testRegen < 100.0f) testRegen += 1.0f;
                break;
            case 'x':
                if (testRegen > 0.0f) testRegen -= 1.0f;
                break;
            case 'e': case 'E':
                testEco = !testEco;
                break;
            case 'd': case 'D':
                testDirection = !testDirection;
                break;
        }
    }

    candecoder.sendSignal();
    candecoder.runQueue(50);

    if (counter >= 20) {
        counter = 0;
        printMenu();
    }
    counter++;
}