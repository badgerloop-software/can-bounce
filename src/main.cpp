#include <Arduino.h>
#include "STM32_CAN.h"
#include "candecoder.h"

CANDecoder candecoder(CAN1, ALT_2, 250000);

uint8_t int8Received;
uint16_t int16Received;
float floatReceived;
bool boolReceived;

int numMessagesReceived[9] = {0,0,0,0,0,0,0,0,0};
float messageReceived[9] = {0,0,0,0,0,0,0,0,0};

void setup() {
  Serial.begin(115200);
}

void loop() {
  candecoder.sendSignal();
  Serial.printf("sent message\n");
  candecoder.runQueue(5000);

  for (int i = 0; i < 9; i++) {
    printf("%d | ", numMessagesReceived[i]);
  }
  Serial.printf("\n");

  Serial.printf("acc_out | regen_brake | lv_12V_telem | lv_5V_telem | lv_5V_current | current_in_telem | brake_pressure_telem | brakeLED\n");

  for (int i = 0; i < 8; i++) {
    Serial.printf("%f | ", messageReceived[i]);
  }
  Serial.printf("\n");
  Serial.printf("Digital Data: \n");
  Serial.printf("Direction: %d\n", digital_data.direction);
  Serial.printf("MC Speed Signal: %d\n", digital_data.mc_speed_sig);
  Serial.printf("Eco Mode: %d\n", digital_data.eco_mode);
  Serial.printf("MCU MC On: %d\n", digital_data.mcu_mc_on);
  Serial.printf("Park Brake: %d\n", digital_data.park_brake);
}