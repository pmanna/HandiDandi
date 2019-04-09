#include "Hand.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16,2);

#define ROCK  0
#define PAPER 1
#define SCISSORS 2
#define NUM_ROUNDS 30

int score[2] = {0,0};
int roundCount;

void blinkMessage(const char *msg, int row)
{
  static bool lcdOnOff = true;

  if (lcdOnOff) {
    lcd.clear();
    lcdOnOff = false;
  } else {
    int initCol = (16 - strlen(msg)) / 2;

    lcd.setCursor(initCol, row);
    lcd.printstr(msg);
    
    lcdOnOff = true;
  }
}

#ifdef TEST_MODE

void testHand(int hand)
{
  for (int ii = 0; ii < 5; ii++) {
    lcd.setCursor(0,1); //Second line
    lcd.printstr("Close finger ");
    lcd.write('0' + ii);
    for (int jj = 0; jj <= 100; jj += 10) {
      moveFinger(ii,hand,jj);
      delay(50);
    }
  }
  
  for (int ii = 0; ii < 5; ii++) {
    lcd.setCursor(0,1); //Second line
    lcd.printstr("Open finger  ");
    lcd.write('0' + ii);
    for (int jj = 100; jj >= 0; jj -= 10) {
      moveFinger(ii,hand,jj);
      delay(50);
    }
  }
}

void testMoves(int hand)
{
  resetHand(hand);
  blinkMessage("TRAINING...", hand);
  delay(2000);
  
  waveMove(hand);
  blinkMessage("TRAINING...", hand);
  delay(1000);
  playRock(hand);
  blinkMessage("TRAINING...", hand);
  delay(1000);
  gunMove(hand);
  blinkMessage("TRAINING...", hand);
  delay(1000);
  cockedGunMove(hand);
  blinkMessage("TRAINING...", hand);
  delay(1000);
  OKMove(hand);
  blinkMessage("TRAINING...", hand);
  delay(2000);
}

#endif // TEST_MODE

void keepScore(long turn, long left, long right)
{
  lcd.setCursor(0, RIGHT_HAND);
  lcd.printstr("R");
  lcd.setCursor(2, RIGHT_HAND);
  lcd.printstr(String(turn).c_str());
  
  // Same choice, no score
  if (left == right) { return; }

  if (left == ROCK) {
    if (right == PAPER) { score[RIGHT_HAND]++; }
    else { score[LEFT_HAND]++; }  // Can only be scissors
  }

  if (left == PAPER) {
    if (right == SCISSORS) { score[RIGHT_HAND]++; }
    else { score[LEFT_HAND]++; }  // Can only be scissors
  }

  if (left == SCISSORS) {
    if (right == ROCK) { score[RIGHT_HAND]++; }
    else { score[LEFT_HAND]++; }  // Can only be scissors
  }

  lcd.setCursor(14, RIGHT_HAND);
  lcd.printstr(String(score[RIGHT_HAND]).c_str());
  lcd.setCursor(0, LEFT_HAND);
  lcd.printstr(String(score[LEFT_HAND]).c_str());
}

void playRock(int hand)
{
  moveFinger(THUMB_JOINT, hand, 80);
  delay(50);
  moveFinger(THUMB_FLEX, hand, 80);
  delay(50);
  moveFinger(INDEX_FLEX, hand, 80);
  delay(50);
  moveFinger(MIDDLE_FLEX, hand, 90);
  delay(50);
  moveFinger(PINKY_FLEX, hand, 90);
  delay(50);
  
  if (hand == RIGHT_HAND) {
    lcd.setCursor(5,hand);
    lcd.printstr("    Rock");
  } else {
    lcd.setCursor(3,hand);
    lcd.printstr("Rock    ");
  }
}

void playScissors(int hand)
{
  moveFinger(THUMB_JOINT, hand, 80);
  delay(50);
  moveFinger(THUMB_FLEX, hand, 80);
  delay(50);
  moveFinger(INDEX_FLEX, hand, 0);
  delay(50);
  moveFinger(MIDDLE_FLEX, hand, 0);
  delay(50);
  moveFinger(PINKY_FLEX, hand, 80);
  delay(50);
  
  if (hand == RIGHT_HAND) {
    lcd.setCursor(5,hand);
    lcd.printstr("Scissors");
  } else {
    lcd.setCursor(3,hand);
    lcd.printstr("Scissors");
  }
}

void playPaper(int hand)
{
  moveFinger(THUMB_JOINT, hand, 0);
  delay(50);
  moveFinger(THUMB_FLEX, hand, 30);
  delay(50);
  moveFinger(INDEX_FLEX, hand, 10);
  delay(50);
  moveFinger(MIDDLE_FLEX, hand, 10);
  delay(50);
  moveFinger(PINKY_FLEX, hand, 10);
  delay(50);
  
  if (hand == RIGHT_HAND) {
    lcd.setCursor(5,hand);
    lcd.printstr("   Paper");
  } else {
    lcd.setCursor(3,hand);
    lcd.printstr("Paper   ");
  }
}

void setup() 
{
  Serial.begin(9600);

  setupHands();

  roundCount = 0;
  
  lcd.begin();   // Init the LCD for 16 chars 2 lines
  lcd.backlight();   // Turn on the backligt (try lcd.noBaklight() to turn it off)
  lcd.clear();
  lcd.noBlink();
  lcd.noCursor();
  
  blinkMessage("START...", RIGHT_HAND);
  
  randomSeed(analogRead(PA0));
}

void loop() {

#ifdef TEST_MODE
  testMoves(RIGHT_HAND);
  resetHand(RIGHT_HAND);
  testMoves(LEFT_HAND);
  resetHand(LEFT_HAND);
  
  /*
  testHand(LEFT_HAND);
  
  delay(1000);
  
  testHand(RIGHT_HAND);
  
  delay(1000);
  */
#endif
  
  resetHand(LEFT_HAND);
  resetHand(RIGHT_HAND);
  delay(1000);

  if (roundCount >= NUM_ROUNDS) {
    if (score[LEFT_HAND] > score[RIGHT_HAND]) {
      OKMove(LEFT_HAND);
    } else if (score[RIGHT_HAND] > score[LEFT_HAND]) {
      OKMove(RIGHT_HAND);
    }
    delay(3000);
    return;
    
  }
  
  long leftChoice = random(3);

  switch (leftChoice) {
    case ROCK:
      playRock(LEFT_HAND);
      break;
    case PAPER:
      playPaper(LEFT_HAND);
      break;
    case SCISSORS:
      playScissors(LEFT_HAND);
      break;
  }
  
  long rightChoice = random(3);

  switch (rightChoice) {
    case ROCK:
      playRock(RIGHT_HAND);
      break;
    case PAPER:
      playPaper(RIGHT_HAND);
      break;
    case SCISSORS:
      playScissors(RIGHT_HAND);
      break;
  }

  roundCount++;
  keepScore(roundCount, leftChoice, rightChoice);
  
  delay(1000);
}
