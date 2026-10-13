#include <Arduino.h>
#include "STM32_CAN.h"
#include "candecoder.h" 

CANDecoder candecoder(CAN1, ALT_2, 250000);

static void redraw() {
    Serial.printf("\033[2J\033[1;1H");
    Serial.printf("sc2.5-tutorial can-bounce\n");
    Serial.printf("1 = toggle LED cmd\n");
    Serial.printf("LED cmd: %s\n", g_led_on ? "ON" : "off");
}

void setup() {
    Serial.begin(115200);
    redraw();
}

void loop() {
    if (Serial.available() > 0) {
        char key = Serial.read();
        while (Serial.available()) {
            Serial.read();
        }

        if (key == '1') {
            g_led_on = !g_led_on;
            redraw();
        }
    }

    // TODO: send the LED command over CAN
    candecoder.sendSignal();

    candecoder.runQueue(50);
}
