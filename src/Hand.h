#include <Wire.h>

//#define TEST_MODE

#define RIGHT_HAND 0
#define LEFT_HAND 1

#define THUMB_JOINT 0
#define THUMB_FLEX 1
#define INDEX_FLEX 2
#define MIDDLE_FLEX 3
#define PINKY_FLEX 4

void setupHands();
void moveFinger(int finger, int hand, int percent);
void resetHand(int hand);

// Some predetermined moves
void OKMove(int hand);

#ifdef TEST_MODE
void gunMove(int hand);
void cockedGunMove(int hand);
void waveMove(int hand);
#endif
