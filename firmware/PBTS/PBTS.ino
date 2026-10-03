/*
  Priority-Based Task Execution Using Arduino
  Team: Kalagatori Vaishnav Aditya (CH.SC.U4CYS25012), Tejpratap Kumar Sah (CH.SC.U4CYS25052)

  Simple priority scheduling: each loop, run the highest-priority task that is
  currently flagged "ready". 0 = highest priority.
    0 Fire alarm (flame sensor, interrupt)
    1 Motion (motion sensor, interrupt)
    2 Button
    3 LCD refresh (runs only when nothing else is pending)
*/

#include <LiquidCrystal.h>

#define PIN_FLAME      2   // interrupt pin
#define PIN_MOTION     3   // interrupt pin
#define PIN_BUTTON     4
#define PIN_BUZZER     8
#define PIN_LED_FIRE   9
#define PIN_LED_MOTION 10

LiquidCrystal lcd(12, 11, 5, 6, 7, 13);

volatile bool fireReady   = false;
volatile bool motionReady = false;
unsigned long lastLcdUpdate = 0;

void onFlame(void)  { fireReady = true; }
void onMotion(void) { motionReady = true; }

void runFireTask(void) {
  digitalWrite(PIN_LED_FIRE, HIGH);
  tone(PIN_BUZZER, 2000, 300);
  lcd.setCursor(0, 0);
  lcd.print("!! FIRE ALARM !!");
  Serial.println("[P0] Fire alarm");
  delay(50);
  digitalWrite(PIN_LED_FIRE, LOW);
  fireReady = false;
}

void runMotionTask(void) {
  digitalWrite(PIN_LED_MOTION, HIGH);
  Serial.println("[P1] Motion");
  delay(20);
  digitalWrite(PIN_LED_MOTION, LOW);
  motionReady = false;
}

void runButtonTask(void) {
  Serial.println("[P2] Button");
  delay(10);
}

void runLcdTask(void) {
  lcd.setCursor(0, 1);
  lcd.print("System OK      ");
  Serial.println("[P3] LCD refresh");
  lastLcdUpdate = millis();
}

void setup(void) {
  Serial.begin(9600);
  pinMode(PIN_FLAME, INPUT);
  pinMode(PIN_MOTION, INPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED_FIRE, OUTPUT);
  pinMode(PIN_LED_MOTION, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("PBTS Scheduler");

  attachInterrupt(digitalPinToInterrupt(PIN_FLAME), onFlame, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_MOTION), onMotion, RISING);
}

void loop(void) {
  // Highest priority first: check and handle fire, then motion, then button.
  // Lower-priority tasks only run when nothing more urgent is pending.
  if (fireReady) {
    runFireTask();
  } else if (motionReady) {
    runMotionTask();
  } else if (digitalRead(PIN_BUTTON) == LOW) {
    runButtonTask();
  } else if (millis() - lastLcdUpdate >= 1000) {
    runLcdTask();
  }
}
