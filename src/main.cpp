#include "STM32_CAN.h"
#include "candecoder.h"
#include <Arduino.h>

// For random
#include <stdlib.h>
#include <time.h>

// PDC state machine testing
// Three modes: automatic tests, manual input control, or random CAN echo

#define PWM_PIN PC_8
#define TEST_RPM                                                               \
  30 // above the rpm > 2 transition threshold so we can verify direction
     // changes
#define SETTLE_MS 2000 // wait for PDC to process and respond
#define NUM_TESTS 13
// Amount to set simulated RPM up/down per button press in manual mode
#define RPM_STEP 10

CANDecoder candecoder(CAN1, ALT_2, 250000);

HardwareTimer *tim;
uint32_t channel;

enum Mode { MODE_SELECT, MODE_TEST, MODE_MANUAL, MODE_RANDOM };
Mode currentMode = MODE_SELECT;

// test mode state
int testsPassed = 0;
int testsFailed = 0;
int testNum = 0;
bool verifying = false;
bool prompted = false;
bool waitingForAck = false; // wait for user to read results before next test
unsigned long settleStart = 0;

// manual mode state
int16_t simRPM = 0;
uint8_t refreshCounter = 0;

// set simulated wheel speed (0 = stopped)
void setRPM(int rpm) {
  if (rpm > 0) {
    uint32_t freq = (48 * rpm) / 60; // 48 pulses/rev, convert to Hz
    tim->setPWM(channel, PWM_PIN, freq, 10);
  } else {
    tim->setPWM(channel, PWM_PIN, 1, 0);
  }
}

void clearSteering() {
  steering_data.headlight = false;
  steering_data.direction_switch = false;
  steering_data.left_blink = false;
  steering_data.right_blink = false;
  steering_data.crz_mode_a = false;
  steering_data.crz_set = false;
  steering_data.crz_reset = false;
  steering_data.horn = false;
  steering_data.mc_on = false;
  steering_data.park_brake = false;
}

// print what the PDC is sending back over CAN
void printStatus() {
  printf("OUTPUTS FROM PDC\n");
  printf("acc_out:     %.2f\n", messageReceived[0]);
  printf("regen_brake: %.2f\n", messageReceived[1]);
  printf("lv_12V:      %.2f\n", messageReceived[2]);
  printf("lv_5V:       %.2f\n", messageReceived[3]);
  printf("lv_5V_curr:  %.2f\n", messageReceived[4]);
  printf("curr_in:     %.2f\n", messageReceived[5]);
  printf("brake_pres:  %.2f\n", messageReceived[6]);
  printf("mph:         %.2f\n", messageReceived[8]);
  printf("acc_in:      %.2f\n", messageReceived[9]);
  printf("direction:   %d (%s)\n", digital_data.direction,
         digital_data.direction ? "FWD" : "REV");
  printf("eco_mode:    %d\n", digital_data.eco_mode);
  printf("mcu_mc_on:   %d\n", digital_data.mcu_mc_on);
  printf("park_brake:  %d\n", digital_data.park_brake);
  printf("brake_led:   %d\n", digital_data.brake_led);
  printf("\n");
}

// print what we're currently sending to the PDC
void printInputs() {
  printf("INPUTS TO PDC\n");
  printf("direction_switch: %d (%s)\n", steering_data.direction_switch,
         steering_data.direction_switch ? "FWD" : "REV");
  printf("left_blink:       %d\n", steering_data.left_blink);
  printf("right_blink:      %d\n", steering_data.right_blink);
  printf("crz_mode_a:       %d\n", steering_data.crz_mode_a);
  printf("crz_set:          %d\n", steering_data.crz_set);
  printf("crz_reset:        %d\n", steering_data.crz_reset);
  printf("horn:             %d\n", steering_data.horn);
  printf("mc_on:            %d\n", steering_data.mc_on);
  printf("park_brake:       %d\n", steering_data.park_brake);
  printf("simulated RPM:    %d\n", simRPM);
  printf("simAccIn:         %.2f\n", simAccIn);
  printf("\n");
}

void printManualHelp() {
  printf("Controls | toggles on/off \n");
  printf("f = direction_switch    l = left_blink    k = right_blink\n");
  printf("c = crz_mode_a   z = crz_set   x = crz_reset   h = horn\n");
  printf("m = mc_on         p = park_brake\n");
  printf("i = RPM +%d   d = RPM -%d   0 = stop RPM\n", RPM_STEP, RPM_STEP);
  printf("w = acc +0.1  s = acc -0.1   a = STOP acc\n");
  printf("r = refresh   q = back to menu\n");
}

void report(const char *name, bool pass) {
  if (pass) {
    testsPassed++;
    printf("[PASS] %s\n", name);
  } else {
    testsFailed++;
    printf("[FAIL] %s\n", name);
  }
}

void showModeMenu() {
  printf("\e[1;1H\e[2J");
  printf("PDC Tester\n\n");
  printf("1 = Automated tests\n");
  printf("2 = Manual input control\n");
  printf("3 = Random CAN echo\n\n");
}

// show instructions for the current test step
void showPrompt() {
  printf("\e[1;1H\e[2J");

  switch (testNum) {
  case 0:
    printf("Test 1/%d: OFF State\n", NUM_TESTS);
    printf("Auto: mc_on = OFF, park_brake = OFF, acc_in = 5.0\n");
    printf("Expected: acc_out = 0\n");
    break;
  case 1:
    printf("Test 2/%d: OFF -> Park\n", NUM_TESTS);
    printf("Auto: mc_on = ON, acc_in = 0\n");
    printf("Expected: mc_on = 1, acc_out = 0\n");
    break;
  case 2:
    printf("Test 3/%d: Park -> Idle\n", NUM_TESTS);
    printf("Auto: park_brake = OFF, acc_in = 5.0\n");
    printf("Expected: park_brake = 0, acc_out = 5.0\n");
    break;
  case 3:
    printf("Test 4/%d: Idle direction FWD\n", NUM_TESTS);
    printf("Auto: direction_switch = FWD\n");
    printf("Expected: direction = 1 (FWD)\n");
    break;
  case 4:
    printf("Test 5/%d: Idle -> Forward\n", NUM_TESTS);
    printf("Auto: RPM = %d, direction = FWD\n", TEST_RPM);
    printf("Expected: direction = 1 (FWD)\n");
    break;
  case 5:
    printf("Test 6/%d: Forward acc passthrough\n", NUM_TESTS);
    printf("Press the accelerator pedal partway.\n");
    printf("Expected: acc_out > 0\n");
    break;
  case 6:
    printf("Test 7/%d: Forward -> Idle\n", NUM_TESTS);
    printf("Auto: RPM = 0. Release the accelerator.\n");
    printf("Expected: acc_out = 0\n");
    break;
  case 7:
    printf("Test 8/%d: Idle direction REV\n", NUM_TESTS);
    printf("Auto: direction_switch = REV\n");
    printf("Expected: direction = 0 (REV)\n");
    break;
  case 8:
    printf("Test 9/%d: Idle -> Reverse\n", NUM_TESTS);
    printf("Auto: RPM = %d, direction = REV\n", TEST_RPM);
    printf("Expected: direction = 0 (REV)\n");
    break;
  case 9:
    printf("Test 10/%d: Reverse -> Idle\n", NUM_TESTS);
    printf("Auto: RPM = 0\n");
    printf("Expected: returns to Idle\n");
    break;
  case 10:
    printf("Test 11/%d: Idle -> Park\n", NUM_TESTS);
    printf("Auto: RPM = 0, direction = FWD, park_brake = ON, acc_in = 5.0\n");
    printf("Expected: park_brake = 1, acc_out = 0\n");
    break;
  case 11:
    printf("Test 12/%d: Any -> Park override\n", NUM_TESTS);
    printf("Auto: RPM = %d, direction_switch = FWD, park_brake = ON, acc_in = "
           "5.0\n",
           TEST_RPM);
    printf("Expected: acc_out = 0, park_brake = 1\n");
    break;
  case 12:
    printf("Test 13/%d: Any -> OFF\n", NUM_TESTS);
    printf("Auto: mc_on = OFF\n");
    printf("Expected: acc_out = 0, mc_on = 0\n");
    break;
  }

  printf("\nPress 'n' to apply & verify...\n");
}

// set CAN + PWM inputs for the current test
void applyInputs() {
  switch (testNum) {
  case 0: // OFF state: mc_on off, send acc to prove it's ignored
    steering_data.mc_on = false;
    steering_data.park_brake = false;
    simAccIn = 5.0f;
    break;
  case 1: // OFF -> Park: turn mc_on on
    steering_data.mc_on = true;
    simAccIn = 0.0f;
    break;
  case 2: // Park -> Idle: release park brake
    steering_data.park_brake = false;
    steering_data.direction_switch = true; // prep FWD for next test
    simAccIn = 5.0f;
    break;
  case 3: // idle direction FWD
    steering_data.direction_switch = true;
    simAccIn = 5.0f;
    break;
  case 4: // idle -> forward
    steering_data.direction_switch = true;
    simAccIn = 5.0f;
    setRPM(TEST_RPM);
    break;
  case 5:
    break; // user presses pedal
  case 6:  // forward -> idle
    setRPM(0);
    simAccIn = 0.0f;
    break;
  case 7: // idle direction REV
    steering_data.direction_switch = false;
    simAccIn = 0.0f;
    break;
  case 8: // idle -> reverse
    steering_data.direction_switch = false;
    simAccIn = 5.0f;
    setRPM(TEST_RPM);
    break;
  case 9: // reverse -> idle
    setRPM(0);
    simAccIn = 0.0f;
    break;
  case 10: // idle -> park: stopped, engage park brake
    steering_data.park_brake = true;
    setRPM(0);
    steering_data.direction_switch = true;
    simAccIn = 5.0f;
    break;
  case 11: // Any -> Park override: moving with park brake (should zero acc)
    steering_data.park_brake = true;
    setRPM(TEST_RPM);
    steering_data.direction_switch = true;
    simAccIn = 5.0f;
    break;
  case 12: // any -> off: turn mc_on off
    steering_data.mc_on = false;
    setRPM(0);
    simAccIn = 0.0f;
    break;
  }
}

// check PDC outputs against expected values
void verifyOutputs() {
  printf("\e[1;1H\e[2J");
  printf("Test %d/%d Results\n\n", testNum + 1, NUM_TESTS);
  printStatus();

  switch (testNum) {
  case 0:
    report("OFF: acc_out = 0", messageReceived[0] == 0.0f);
    break;
  case 1:
    report("Park: mc_on = 1", digital_data.mcu_mc_on == true);
    report("Park: acc_out = 0", messageReceived[0] == 0.0f);
    break;
  case 2:
    report("Idle: park_brake = 0", digital_data.park_brake == false);
    // acc_in passthrough works, acc_out should NOT be zero if pedal is pressed
    report("Idle: acc_out > 0 (pedal pressed)", messageReceived[0] > 0.0f);
    break;
  case 3:
    report("Idle: direction = 1 (FWD)", digital_data.direction == 1);
    break;
  case 4:
    report("Forward: direction = 1 (FWD)", digital_data.direction == 1);
    break;
  case 5:
    report("Forward: acc_out > 0 (pedal pressed)", messageReceived[0] > 0.0f);
    break;
  case 6:
    report("Fwd->Idle: acc_out = 0 (pedal released)",
           messageReceived[0] == 0.0f);
    break;
  case 7:
    report("Idle: direction = 0 (REV)", digital_data.direction == 0);
    break;
  case 8:
    report("Reverse: direction = 0 (REV)", digital_data.direction == 0);
    break;
  case 9:
    report("Rev->Idle: acc_out = 0 (no throttle)", messageReceived[0] == 0.0f);
    break;
  case 10:
    report("Idle->Park: park_brake = 1", digital_data.park_brake == true);
    report("Idle->Park: acc_out = 0", messageReceived[0] == 0.0f);
    break;
  case 11:
    report("Any->Park Override: park_brake = 1",
           digital_data.park_brake == true);
    report("Any->Park Override: acc_out = 0", messageReceived[0] == 0.0f);
    break;
  case 12:
    report("OFF: mc_on = 0", digital_data.mcu_mc_on == false);
    report("OFF: acc_out = 0", messageReceived[0] == 0.0f);
    break;
  }
}

void setup() {
  Serial.begin(115200);

  TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(PWM_PIN, PinMap_PWM);
  tim = new HardwareTimer(Instance);
  channel = STM_PIN_CHANNEL(pinmap_function(PWM_PIN, PinMap_PWM));

  srand(time(NULL));
  setRPM(0);
  clearSteering();
  delay(2000);
  showModeMenu();
}

// test mode
void loopTestMode(char input) {
  if (input == 'q' || input == 'Q') {
    testNum = NUM_TESTS;
    verifying = false;
    prompted = false;
    waitingForAck = false;
  }

  if (input == 'r' || input == 'R') {
    printf("\e[1;1H\e[2J");
    printStatus();
    printf("Press 'n' to continue...\n");
    return;
  }

  // all tests done
  if (testNum >= NUM_TESTS) {
    if (!prompted) {
      prompted = true;
      printf("\e[1;1H\e[2J");
      printf("TEST SUMMARY\n\n");
      printf("Total:  %d\n", testsPassed + testsFailed);
      printf("Passed: %d\n", testsPassed);
      printf("Failed: %d\n", testsFailed);
      printf("Result: %s\n\n",
             testsFailed == 0 ? "ALL PASS" : "FAILURES DETECTED");
      printf("'n' to restart, 'q' to go back to menu\n");
    }
    if (input == 'n') {
      testsPassed = 0;
      testsFailed = 0;
      testNum = 0;
      verifying = false;
      prompted = false;
      waitingForAck = false;
      setRPM(0);
      simAccIn = 0.0f;
      clearSteering();
      for (int i = 0; i < 10; i++) {
        messageReceived[i] = 0;
        numMessagesReceived[i] = 0;
      }
    }
    if (input == 'q' || input == 'Q') {
      currentMode = MODE_SELECT;
      prompted = false;
      waitingForAck = false;
      showModeMenu();
    }
    return;
  }

  // wait for user to acknowledge test results before showing next prompt
  if (waitingForAck) {
    if (input == 'n') {
      waitingForAck = false;
      prompted = false; // now allow showPrompt to run for the next test
    }
    return;
  }

  // prompt then verify cycle
  if (!verifying) {
    if (!prompted) {
      showPrompt();
      prompted = true;
    }
    if (input == 'n') {
      applyInputs();
      settleStart = millis();
      verifying = true;
    }
  } else {
    if (millis() - settleStart >= SETTLE_MS) {
      verifyOutputs();
      testNum++;
      verifying = false;
      waitingForAck = true; // hold on results screen until user presses 'n'
      if (testNum < NUM_TESTS)
        printf("\nPress 'n' for next test...\n");
      else
        printf("\nPress 'n' to see summary...\n");
    }
  }
}

// manual mode
void loopManualMode(char input) {
  bool changed = false;

  switch (input) {
  case 'f':
    steering_data.direction_switch = !steering_data.direction_switch;
    changed = true;
    break;
  case 'l':
    steering_data.left_blink = !steering_data.left_blink;
    changed = true;
    break;
  case 'k':
    steering_data.right_blink = !steering_data.right_blink;
    changed = true;
    break;
  case 'c':
    steering_data.crz_mode_a = !steering_data.crz_mode_a;
    changed = true;
    break;
  case 'z':
    steering_data.crz_set = !steering_data.crz_set;
    changed = true;
    break;
  case 'x':
    steering_data.crz_reset = !steering_data.crz_reset;
    changed = true;
    break;
  case 'h':
    steering_data.horn = !steering_data.horn;
    changed = true;
    break;
  case 'm':
    steering_data.mc_on = !steering_data.mc_on;
    changed = true;
    break;
  case 'p':
    steering_data.park_brake = !steering_data.park_brake;
    changed = true;
    break;
  case 'i':
    simRPM += RPM_STEP;
    setRPM(simRPM);
    changed = true;
    break;
  case 'd':
    simRPM -= RPM_STEP;
    if (simRPM < 0)
      simRPM = 0;
    setRPM(simRPM);
    changed = true;
    break;
  case '0':
    simRPM = 0;
    setRPM(0);
    changed = true;
    break;
  case 'w':
    simAccIn += 0.1f;
    changed = true;
    break;
  case 's':
    simAccIn -= 0.1f;
    if (simAccIn < 0)
      simAccIn = 0;
    changed = true;
    break;
  case 'a':
    simAccIn = 0;
    changed = true;
    break;
  case 'r':
  case 'R':
    changed = true;
    break;
  case 'q':
  case 'Q':
    currentMode = MODE_SELECT;
    simRPM = 0;
    simAccIn = 0;
    setRPM(0);
    clearSteering();
    showModeMenu();
    return;
  }

  // refresh on change or periodically
  refreshCounter++;
  if (changed || refreshCounter >= 20) {
    refreshCounter = 0;
    printf("\e[1;1H\e[2J");
    printf("MANUAL MODE\n\n");
    printInputs();
    printStatus();
    printManualHelp();
  }
}

// Random CAN echo mode
// Passive listener — just displays whatever the PDC sends over CAN.
// Use with PDC DEBUG_TECHNIQUE == 1 (PDC randomizes its own outputs).
// NOTE: the display refresh can be noisy (thanks to fast updates), so we
// refresh at a slower rate to make it readable.
#define RANDOM_REFRESH_INTERVAL 2000 // ms between display refreshes
unsigned long lastRandomTime = 0;
// Only print the random-mode banner on the first refresh to avoid repeating it.
bool randomModeFirstDisplay = true;

void loopRandomMode(char input) {
  if (input == 'q' || input == 'Q') {
    currentMode = MODE_SELECT;
    showModeMenu();
    return;
  }

  unsigned long now = millis();
  bool shouldRefresh = randomModeFirstDisplay || input == 'r' ||
                       now - lastRandomTime >= RANDOM_REFRESH_INTERVAL;
  if (shouldRefresh) {
    lastRandomTime = now;
    bool printHeader = randomModeFirstDisplay || input == 'r';
    randomModeFirstDisplay = false;

    printf("\e[1;1H\e[2J");
    if (printHeader) {
      printf("RANDOM CAN ECHO MODE  (r = refresh, q = quit)\n\n");
    }
    printStatus();
  }
}

void loop() {
  candecoder.sendSignal();
  candecoder.runQueue(50);

  char input = 0;
  if (Serial.available() > 0) {
    input = Serial.read();
    while (Serial.available())
      Serial.read();

    // Some tools send single-byte commands (0x01/0x02/0x03).
    // Normalize them to their ASCII digit equivalents so the menu works.
    if (input >= 1 && input <= 3) {
      input = '0' + input;
    }
  }

  switch (currentMode) {
  case MODE_SELECT:
    if (input == '1') {
      currentMode = MODE_TEST;
      prompted = false;
      testsPassed = 0;
      testsFailed = 0;
      testNum = 0;
      verifying = false;
      simRPM = 0;
      simAccIn = 0.0f;
      setRPM(0);
      clearSteering();
      for (int i = 0; i < 10; i++) {
        messageReceived[i] = 0;
        numMessagesReceived[i] = 0;
      }
      printf("\e[1;1H\e[2J");
      printf("PDC State Machine Tests\n\n");
      printf("n = next step   r = read outputs   q = skip to results\n\n");
      printf("Press 'n' to begin...\n");
    } else if (input == '2') {
      currentMode = MODE_MANUAL;
      refreshCounter = 20; // immediate display
      simRPM = 0;
      simAccIn = 0.0f;
      setRPM(0);
      clearSteering();
    } else if (input == '3') {
      currentMode = MODE_RANDOM;
      simRPM = 0;
      simAccIn = 0.0f;
      setRPM(0);
      clearSteering();
      lastRandomTime = 0; // trigger immediate send
      randomModeFirstDisplay = true;
      printf("\e[1;1H\e[2J");
      printf("Starting random CAN echo mode...\n");
    }
    break;
  case MODE_TEST:
    loopTestMode(input);
    break;
  case MODE_MANUAL:
    loopManualMode(input);
    break;
  case MODE_RANDOM:
    loopRandomMode(input);
    break;
  }
}
