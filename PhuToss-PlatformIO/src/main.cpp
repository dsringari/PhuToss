#include <Arduino.h>
#include <Servo.h>
#include "NewPing.h"

#include "main.h"
#include "servoCenterTest.h"
#include "pins.h"

// Ultrasonic
NewPing clawSonic(clawSonicTrig, clawSonicEcho, 100);
NewPing wallSonic(wallSonicTrig, wallSonicEcho, 100);

// Claw / Arm
Servo leftArmServo;
Servo rightArmServo;
Servo clawServo;

// IR
enum CandleDirection { candle_none, candle_left, candle_center, candle_right };
CandleDirection candleDirection = candle_none;
int IR1 = 0;
int IR2 = 0;
int IR3 = 0;
int IR4 = 0;
int IR5 = 0;
// For any pin, when should we detect that a candle is within our sight?
const int CANDLE_DETECT_THRESHOLD = 500;


// -- MARK: FUNCTION IMPLEMENTATION

void setup() {
  attachServos();
}

void loop() {
  servoCenterTest();
}

void attachServos() {
  leftArmServo.attach(leftArmServoPin);
  rightArmServo.attach(rightArmServoPin);
  clawServo.attach(clawServoPin);
  armUp();
  clawOpen();
}

// Arm

void armDown() {
  leftArmServo.write(155); // TODO: Test whether test1Servo refers to left or right
  rightArmServo.write(10);
}

void armUp() {
  leftArmServo.write(85); 
  rightArmServo.write(95);
}

// Claw

void clawOpen() {
  // Do something with the claw servo
}

void clawClose() {
  // Do something with the claw servo
}

// IR 

void scanIR() {
    IR1 = analogRead(IRA1_PIN);
    IR2 = analogRead(IRA2_PIN);
    IR3 = analogRead(IRA3_PIN);
    IR4 = analogRead(IRA4_PIN);
    IR5 = analogRead(IRA5_PIN);
}

/*
       IR3
   IR2     IR4
IR1           IR5
*/

void scanCandleDirection() {
  scanIR();
  int leftSum = IR1 + IR2 + IR3;
  int centerSum = IR2 + IR3 + IR4;
  int rightSum = IR3 + IR4 + IR5;

  if (leftSum < CANDLE_DETECT_THRESHOLD && centerSum < CANDLE_DETECT_THRESHOLD && rightSum < CANDLE_DETECT_THRESHOLD) {
    candleDirection = candle_none;
    return;
  }

  if (IR1 < CANDLE_DETECT_THRESHOLD &&
      IR2 < CANDLE_DETECT_THRESHOLD &&
      IR3 < CANDLE_DETECT_THRESHOLD && 
      IR4 < CANDLE_DETECT_THRESHOLD &&
      IR5 < CANDLE_DETECT_THRESHOLD) {
        candleDirection = candle_none;
        return;
      }

  // IR1 is the greatest so left
  if (IR1 > IR2 && IR1 > IR3 && IR1 > IR4 && IR1 > IR5) {
    candleDirection = candle_left;
  } else if (IR2 > IR1 && IR2 > IR3 && IR2 > IR4 && IR2 > IR5) {
    // IR2 is the greatest so left
    candleDirection = candle_left;
  } else if (IR3 > IR1 && IR3 > IR2 && IR3 > IR4 && IR3 > IR5) {
    // IR3 is the greatest so center
    candleDirection = candle_center;
  } else if (IR4 > IR1 && IR4 > IR2 && IR4 > IR3 && IR4 > IR5) {
    // IR4 is the greatest so right
    candleDirection = candle_right;
  } else if (IR5 > IR1 && IR5 > IR2 && IR5 > IR3 && IR5 > IR4) {
    // IR5 is the greatest so right
    candleDirection = candle_right;
  }
}