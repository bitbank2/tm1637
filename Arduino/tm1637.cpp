#include <Arduino.h>
#include <tm1637.h>
//
// Larry's TM1637 library
// Copyright (c) 2018 BitBank Software, Inc.
// Project started 3/17/2018
// written by Larry Bank (bitbank@pobox.com)
//
// The TM1637 from Titan Micro Electronics is a 7-segment LED controller
// It features a 2-wire interface (Clock + Data), but doesn't conform to I2C nor SPI standards
// It can control up to 6 7-segment digits along with decimal points or colons as well as has
// 16 button/keyboard inputs
// The datasheet says that it runs on 5V with 5V logic, but I've run it on 3.3V (both Vcc and signals)
// and it seems to behave correctly.
// Since the communication protocol is custom, it requires "bit-banging" of 2 IO pins to control it.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//
// table which translates a digit into the segments
//
const byte cDigit2Seg[] = {0x3f, 0x6, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
static byte bClockPin, bDataPin;
#define CLOCK_DELAY 5
//
// Initialize the tm1637 by specifying the digital pins used to communicate
//
void tm1637Init(byte bClock, byte bData)
{
   bClockPin = bClock;
   bDataPin = bData;
   pinMode(bClockPin, OUTPUT);
   pinMode(bDataPin, OUTPUT);
   digitalWrite(bClockPin, LOW);
   digitalWrite(bDataPin, LOW);
} /* tm1637Init() */
//
// Start wire transaction
//
static void tm1637Start(void)
{
   digitalWrite(bDataPin, HIGH);
   digitalWrite(bClockPin, HIGH);
   delayMicroseconds(CLOCK_DELAY);
   digitalWrite(bDataPin, LOW);
} /* tm1637Start() */
//
// Stop wire transaction
//
static void tm1637Stop(void)
{
  digitalWrite(bClockPin, LOW); // clock low
  delayMicroseconds(CLOCK_DELAY);
  digitalWrite(bDataPin, LOW); // data low
  delayMicroseconds(CLOCK_DELAY);
  digitalWrite(bClockPin, HIGH); // clock high
  delayMicroseconds(CLOCK_DELAY);
  digitalWrite(bDataPin, HIGH); // data high
} /* tm1637Stop() */
//
// Get data ack
//
static byte tm1637GetAck(void)
{
byte bAck;

 // read ack
   digitalWrite(bClockPin, LOW); // clock to low
   pinMode(bDataPin, INPUT_PULLUP); // data as input
   delayMicroseconds(CLOCK_DELAY);
   
   bAck = digitalRead(bDataPin);
   digitalWrite(bClockPin, HIGH); // clock high
   delayMicroseconds(CLOCK_DELAY);
   digitalWrite(bClockPin, LOW); // clock to low

   pinMode(bDataPin, OUTPUT);
   
   return bAck;
} /* tm1637GetAck() */
//
// Write a byte to the controller
//
static void tm1637WriteByte(byte b)
{
byte i;

   for (i=0; i<8; i++)
   {
      digitalWrite(bClockPin, LOW); // clock low
      if (b & 1) // LSB to MSB
         digitalWrite(bDataPin, HIGH);
      else
         digitalWrite(bDataPin, LOW);
      delayMicroseconds(CLOCK_DELAY);
      digitalWrite(bClockPin, HIGH); // clock high
      delayMicroseconds(CLOCK_DELAY);
      b >>= 1;
   }
} /* tm1637WriteByte() */
//
// Write a sequence of bytes to the controller
//
static void tm1637Write(byte *pData, byte bLen)
{
byte b, bAck;
   bAck = 1;
   tm1637Start();
   for (b=0; b<bLen; b++)
   {
      tm1637WriteByte(pData[b]);
      bAck &= tm1637GetAck();
   }
   tm1637Stop();
} /* tm1637Write() */
//
// Set brightness (0-8)
//
void tm1637SetBrightness(byte b)
{
byte bControl;
   if (b == 0) // display off
      bControl = 0x80; // display off
   else
   {
      if (b > 8) b = 8;
      bControl = 0x88 | (b - 1);
   }
   tm1637Write(&bControl, 1);
} /* tm1637SetBrightness() */
//
// Display a string of 4 digits and optional colon
// by passing a string such as "12:34" or "45 67"
//
void tm1637ShowDigits(char *pString)
{
byte b, bTemp[16]; // commands and data to transmit
byte i, j;
   
   j = 0;
   bTemp[0] = 0x40; // memory write command (auto increment mode)
   tm1637Write(bTemp, 1);

   bTemp[j++] = 0xc0; // set display address to first digit command
   for (i=0; i<5; i++)
   {
      if (i == 2) // position of the colon
      {
         if (pString[i] == ':') // turn on correct bit
            bTemp[2] |= 0x80; // second digit high bit controls colon LEDs
      }
      else
      {
         b = 0;
         if (pString[i] >= '0' && pString[i] <= '9')
         {
            b = cDigit2Seg[pString[i] & 0xf]; // segment data
         }
         bTemp[j++] = b;
      }
   }
   tm1637Write(bTemp, j); // send to the display
}
