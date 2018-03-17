#include <tm1637.h>
//
// Larry's TM1637 Library Demo
// Displays counter from 0000 to 9999 with flashing colon
//
void setup() {
  // put your setup code here, to run once:
  tm1637Init(8, 9); // clock, data pins
  tm1637SetBrightness(4);
}

void loop() {
int i;
char szTemp[8];

  // put your main code here, to run repeatedly:
  for (i=0; i<9999; i++)
  {
      if (i & 1)
         sprintf(szTemp, "%02d:%02d", i/100, i% 100);
      else
         sprintf(szTemp, "%02d %02d", i/100, i % 100);
    tm1637ShowDigits(szTemp);
    delay(500);
  }
}
