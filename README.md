TM1637
![TM1637](/display.jpg?raw=true "TM1637")
<br>
TM1637 is a simple C library for working with 7-segment LED controllers from
Titan Micro Electronics. It features a 2-wire interface (Clock + Data), but doesn't conform to I2C nor SPI standards. It can control up to 6 7-segment digits along with decimal points or colons as well as has 16 button/keyboard inputs.
The datasheet says that it runs on 5V with 5V logic, but I've run it on 3.3V (both Vcc and signals) and it seems to behave correctly. Since the communication protocol is custom, it requires "bit-banging" of 2 IO pins to control it.
<br>
This repo contains both an Arduino Library + demo sketch as well as a Linux
library + demo app. It allows controlling the display with 3 simple functions:<br>
<br>
1) Init<br>
2) Set Brightness<br>
3) Display string (formatted text)<br>

Building on Linux
-----------------
make<br>
make -f make_demo<br>
sudo ./demo<br>
<br>
Copyright (c) 2018 BitBank Software, Inc.<br>
Written by Larry Bank<br>
bitbank@pobox.com<br>

