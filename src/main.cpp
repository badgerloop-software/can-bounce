#include <Arduino.h>
#include "STM32_CAN.h"
#include "candecoder.h"
#define PWM_PIN PC_8
#define STEP_SIZE 10

CANDecoder candecoder(CAN1, ALT_2, 250000);

// RPM 
HardwareTimer *tim;
uint32_t channel;
uint32_t dutyCycle;
int8_t simRPM;
uint32_t frequencyHz = 0; // uint32 because setPWM uses that for frequency param

char buf;
uint8_t counter = 0;

void setup() {
  Serial.begin(115200);
  // Automatically retrieve TIM instance and channel associated to pin
  // This is used to be compatible with all STM32 series automatically.
  TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(PWM_PIN, PinMap_PWM);
  tim = new HardwareTimer(Instance);

  channel = STM_PIN_CHANNEL(pinmap_function(PWM_PIN, PinMap_PWM));
  // Fixed 10% duty cycle, the speed depends on frequency
  dutyCycle = 10;

  simRPM = 0;
}

void loop() {
  if(Serial.available() > 0){
    buf = Serial.read();
    while(Serial.available()) {
      Serial.read(); // clear buffer
    }

    // switch (buf) {
    //   case 'i':
    //     simRPM += STEP_SIZE;
    //     break;
    //   case 'd':
    //     simRPM -= STEP_SIZE;
    //     break;
    //   default: 
    //     steering_data.direction_switch = !steering_data.direction_switch;
    // }

    // if (simRPM > 0) {
    //     frequencyHz = (48 * simRPM) / 60; // 48 pulses per rotation * RPM, divided by 60 seconds to get frequency
    //     tim->setPWM(channel, PWM_PIN, frequencyHz, dutyCycle);
    // } else {
    //     tim->setPWM(channel, PWM_PIN, 1, 0);
    //     simRPM = 0;
    // }
  }

  candecoder.sendSignal();
  candecoder.runQueue(50);
  // printf("num messages: %d\n", counter_messages);

  if (counter >= 20){
    counter = 0;

    // clear the screen
    printf("\e[1;1H\e[2J");
    printf("Simulating RPM: %d\n", simRPM);
    printf("Press i to increment, d to decrement\n");

    printf("forwardReverse (press 's' to toggle): %s\n", steering_data.direction_switch? "REV": "FWD");
    
    // for (int i = 0; i < 9; i++) {
    //   printf("%d | ", numMessagesReceived[i]);
    // }
    // printf("\n");

    printf("i_12v: %f\n", messageReceived[0]);
    printf("v_12v: %f\n", messageReceived[1]);
    printf("supp_i: %f\n", messageReceived[2]);
    printf("batt_i: %f\n", messageReceived[3]);
    printf("supp_v: %f\n", messageReceived[4]);
    printf("\n");
    printf("estop_mcu: %d\n", digital_data.estop_mcu);
    printf("batt_pos_cont: %d\n", digital_data.batt_pos_cont);
    printf("ppc1_supp_invalid: %d\n", digital_data.ppc1_supp_invalid);
    printf("ppc1_dcdc_invalid: %d\n", digital_data.ppc1_dcdc_invalid);
    printf("mppt_cont_mcu: %d\n", digital_data.mppt_cont_mcu);
    printf("mc_cont_mcu: %d\n", digital_data.mc_cont_mcu);
    printf("mcu_batt_en: %d\n", digital_data.mcu_batt_en);
  }
  counter++;
}