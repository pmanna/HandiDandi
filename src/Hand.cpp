#include "Hand.h"
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 50

int limits[2][5][2] = {
  {
    {70, 160},{20,120}, {155,10}, {175,10}, {160,50}
  },
  {
    {70,0}, {0,100}, {160,10}, {10,150}, {160,50}
  }
};

void setupHands()
{
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
}

int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  
  //Serial.println(analog_value);
  return analog_value;
}

int pwmValue(int hand, int finger, int percent)
{
  int val = map(percent, 0, 100, limits[hand][finger][0], limits[hand][finger][1]);
  
  return pulseWidth(val);
}

int servoPin(int hand, int finger)
{
  return 8 * hand + finger;
}

void moveFinger(int finger, int hand, int percent)
{
  pwm.setPWM(servoPin(hand,finger),0,pwmValue(hand,finger,percent));
}

void resetHand(int hand)
{
    moveFinger(THUMB_JOINT, hand, 0);
    delay(25);
    moveFinger(THUMB_FLEX, hand, 0);
    delay(25);
    moveFinger(INDEX_FLEX, hand, 0);
    delay(25);
    moveFinger(MIDDLE_FLEX, hand, 0);
    delay(25);
    moveFinger(PINKY_FLEX, hand, 0);
    delay(25);
}

// Some predetermined moves
void OKMove(int hand)
{
    moveFinger(THUMB_JOINT, hand, 80);
    delay(25);
    moveFinger(THUMB_FLEX, hand, 80);
    delay(25);
    moveFinger(INDEX_FLEX, hand, 80);
    delay(25);
    moveFinger(MIDDLE_FLEX, hand, 0);
    delay(25);
    moveFinger(PINKY_FLEX, hand, 0);
    delay(25);
}

#ifdef TEST_MODE

void gunMove(int hand)
{
    moveFinger(THUMB_JOINT, hand, 0);
    delay(25);
    moveFinger(THUMB_FLEX, hand,  0);
    delay(25);
    moveFinger(INDEX_FLEX, hand,  0);
    delay(25);
    moveFinger(MIDDLE_FLEX, hand, 90);
    delay(25);
    moveFinger(PINKY_FLEX, hand, 90);
    delay(25);
}

void cockedGunMove(int hand)
{
    moveFinger(THUMB_JOINT, hand, 0);
    delay(25);
    moveFinger(THUMB_FLEX, hand,  90);
    delay(25);
    moveFinger(INDEX_FLEX, hand,  0);
    delay(25);
    moveFinger(MIDDLE_FLEX, hand, 90);
    delay(25);
    moveFinger(PINKY_FLEX, hand, 90);
    delay(25);
}

void waveMove(int hand)
{
  for (int ii = 0; ii < 3; ii++) {
      moveFinger(THUMB_JOINT, hand, 40);
      delay(50);
      moveFinger(THUMB_FLEX, hand,  40);
      delay(50);
      moveFinger(INDEX_FLEX, hand,  40);
      delay(200);
      moveFinger(THUMB_JOINT, hand, 0);
      delay(50);
      moveFinger(THUMB_FLEX, hand,  0);
      delay(50);
      moveFinger(MIDDLE_FLEX, hand, 40);
      delay(200);
      moveFinger(INDEX_FLEX, hand,  0);
      delay(50);
      moveFinger(PINKY_FLEX, hand, 40);
      delay(200);
      moveFinger(MIDDLE_FLEX, hand, 0);
      delay(50);
      moveFinger(PINKY_FLEX, hand, 0);
      delay(200);
  }
}

#endif
