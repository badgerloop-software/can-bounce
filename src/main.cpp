#include <Arduino.h>
#include "STM32_CAN.h"
#include "candecoder.h"

CANDecoder candecoder(CAN1, ALT_2, 250000);

// External volatile declarations for CAN data
volatile float acc_out;
volatile float regen_brake;
volatile float lv_12V_telem;
volatile float lv_5V_telem;
volatile float lv_5V_current;
volatile float current_in_telem;
volatile float brake_pressure_telem;
volatile bool brakeLED;

int counter = 0;

void initRandomAAAValues(){
  // Initialize variables with constant values
  acc_out = 3.5f;                // Accelerator output
  regen_brake = 1.2f;            // Regenerative braking level
  lv_12V_telem = 12.6f;          // 12V system voltage
  lv_5V_telem = 5.1f;            // 5V system voltage
  lv_5V_current = 0.8f;          // Current on 5V rail in amps
  current_in_telem = 24.3f;      // System current in amps
  brake_pressure_telem = 6.7f;   // Brake pressure in bar
  
  // Initialize boolean value
  brakeLED = false;              // Brake light status
  
  // Initialize digital_data struct with bit values
  digital_data.direction = true;     // Forward direction
  digital_data.mc_speed_sig = false; // No speed signal
  digital_data.eco_mode = true;      // Eco mode enabled
  digital_data.mcu_mc_on = true;     // Motor controller on
  digital_data.park_brake = false;   // Park brake disengaged
  
}

void sanityCheck(){
  Serial.println("Values initialized:");
  Serial.print("acc_out: "); Serial.println(acc_out);
  Serial.print("regen_brake: "); Serial.println(regen_brake);
  Serial.print("lv_12V_telem: "); Serial.println(lv_12V_telem);
  Serial.print("lv_5V_telem: "); Serial.println(lv_5V_telem);
  Serial.print("lv_5V_current: "); Serial.println(lv_5V_current);
  Serial.print("current_in_telem: "); Serial.println(current_in_telem);
  Serial.print("brake_pressure_telem: "); Serial.println(brake_pressure_telem);
  Serial.print("brakeLED: "); Serial.println(brakeLED);
  Serial.print("digital_data: dir="); Serial.print(digital_data.direction);
  Serial.print(", speed_sig="); Serial.print(digital_data.mc_speed_sig);
  Serial.print(", eco="); Serial.print(digital_data.eco_mode);
  Serial.print(", mc_on="); Serial.print(digital_data.mcu_mc_on);
  Serial.print(", brake="); Serial.println(digital_data.park_brake);
}

void setup() {
  Serial.begin(115200);
  initRandomAAAValues();
}

void loop() {
  if (((int)acc_out)%20 == 0) {
    printf("counter acc_out: %f\n", acc_out);
    printf("sendsuccess: %d\n", sendsuccess);
  }
  acc_out += 1;
  candecoder.sendSignal();
  candecoder.runQueue(50);
}