This arduino project was posted by Cornelius on his blog 'One Transistor': https://www.onetransistor.eu/2018/01/i2c-analog-tv-modulator-arduino-lcd.html#

These are the first lines from this site (just for introduction):

 I2C Analog TV Modulator controlled by Arduino
 Posted by:  Cornelius  Posted on:  January 21, 2018  Updated on:  January 24, 2018

Using a RMUP74055AD or TNF0170U722 device to build an analog UHF TV modulator with Arduino and LCD display.
Analog video is getting replaced by digital signals which provide better resolution and picture without noise or interference. But, analog video signal is easy to generate with simple hardware and then it can be FM modulated for broadcasting over a wire. I2C controlled RF modulators are common modules in obsolete VCRs and set top boxes. Most of them cover the entire UHF band and support multistandard sound carrier frequencies. Once taken out of its device, the modulator needs a microcontroller to set up its frequency and other parameters.

Using an Arduino board with LCD and keypad shield a full featured modulator can be built. Arduino can be used to generate video too, but a single board can't use I2C and generate video in the same sketch. You'll need different boards if that's what you want to do.

I used for this project a Samsung RMUP74055AD modulator with MBS74T1AEF controller. Some searching reveals the same IC is also used by Tena TNF0170U722 modulator. Some datasheets will come up too, if you search for them. Anyway, these modulators are 5V devices.
