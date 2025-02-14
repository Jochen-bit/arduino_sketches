// This sketch was first published without a licence information on https://www.onetransistor.eu/2018/01/i2c-analog-tv-modulator-arduino-lcd.html#

#include <LiquidCrystal.h>
#include <Wire.h>
#include <EEPROM.h>

#define MOD_ADDRESS (0xCA >> 1)

// 4.5MHz, 5.5MHz, 6MHz, 6.5MHz
const byte mod_sound[4] = { 0x00, 0x08, 0x10, 0x18 };
byte eChannel, eSoundIdx, eOn, eTest;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

unsigned long keyTime = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

/****** Keypad functions *****/
byte readKeypad() {
  int adc = analogRead(A0);

  if (adc < 50) return btnRIGHT;
  if (adc < 250) return btnUP;
  if (adc < 450) return btnDOWN;
  if (adc < 650) return btnLEFT;
  if (adc < 850) return btnSELECT;

  return btnNONE;
}

void keypadAction() {
  unsigned long currentTime = millis();
  if (currentTime - keyTime < 300) return; // less than 100 ms passed; quit

  byte key = readKeypad();
  keyTime = currentTime; // store last call time

  // perform actions
  if (key == btnNONE) return; // no key is pressed; quit
  switch (key)
  {
    case btnRIGHT:
      {
        if (eChannel < 69) eChannel++;
        EEPROM.write(0, eChannel);

        if (eOn) mod_changeChannel(eChannel, eTest);
        break;
      }
    case btnLEFT:
      {
        if (eChannel > 21) eChannel--;
        EEPROM.write(0, eChannel);

        if (eOn) mod_changeChannel(eChannel, eTest);
        break;
      }
    case btnUP:
      {
        if (eOn) eOn = 0;
        else eOn = 1;
        EEPROM.write(2, eOn);

        if (eOn) {
          mod_start(eChannel, eSoundIdx, eTest);
          lcd.setCursor(0, 0);
          lcd.print("  << ON AIR >>  ");
        }
        else {
          mod_sleep();
          lcd.setCursor(0, 0);
          lcd.print("   << OFF >>    ");
        }
        break;
      }
    case btnDOWN:
      {
        if (eTest) eTest = 0;
        else eTest = 1;

        if (eOn) mod_changeChannel(eChannel, eTest);
        break;
      }
    case btnSELECT:
      {
        eSoundIdx++;
        eSoundIdx &= 3;
        EEPROM.write(1, eSoundIdx);

        if (eOn) mod_setSoundCarrier(eSoundIdx);
        break;
      }
  }
  lcdUpdate(eChannel, eSoundIdx, eTest);

  delay(100);
  mod_getStatus();
}

/***** Modulator functions *****/
void mod_setFreqBytes(byte &fm, byte &fl, byte channel, boolean testMode) {
  if ((channel < 21) || (channel > 69)) channel = 21;

  uint16_t freq = 32 * channel + 1213;
  fm = freq >> 6;
  if (testMode) fm |= 0x40;
  fl = (freq & 0x3F) << 2;
}

void mod_start(byte channel, byte soundIdx, boolean testMode) {
  byte fm, fl;

  mod_setFreqBytes(fm, fl, channel, testMode);

  if (soundIdx > 3) soundIdx = 1;

  Wire.beginTransmission(MOD_ADDRESS);
  Wire.write(0x88);
  Wire.write(0x40 | mod_sound[soundIdx]);
  Wire.write(fm);
  Wire.write(fl);
  Wire.endTransmission();
}

void mod_sleep() {
  Wire.beginTransmission(MOD_ADDRESS);
  Wire.write(0xA8);
  Wire.write(0x20);
  Wire.endTransmission();
}

void mod_changeChannel(byte channel, boolean testMode) {
  byte fm, fl;

  mod_setFreqBytes(fm, fl, channel, testMode);

  Wire.beginTransmission(MOD_ADDRESS);
  Wire.write(fm);
  Wire.write(fl | 0x01);
  Wire.endTransmission();
}

void mod_setSoundCarrier(byte idx) {
  if (idx > 3) idx = 1;

  Wire.beginTransmission(MOD_ADDRESS);
  Wire.write(0x88);
  Wire.write(0x40 | mod_sound[idx]);
  Wire.endTransmission();
}

void mod_getStatus() {
  if (!eOn) return;
  byte s = 1;
  int c = Wire.requestFrom(MOD_ADDRESS, 1);
  if (c == 1) s = Wire.read();

  if (s & 0x01) {
    lcd.setCursor(0, 0);
    lcd.print("  << ERROR !! >>  ");
  }
}

void lcdUpdate(byte channel, byte soundIdx, boolean testMode) {
  lcd.setCursor(0, 1);
  lcd.print("CH");
  lcd.print(channel);

  lcd.setCursor(6, 1);
  if (testMode) lcd.print("TEST");
  else lcd.print("    ");

  lcd.setCursor(12, 1);
  switch (soundIdx) {
    case 0: lcd.print("4.5M"); break;
    case 1: lcd.print("5.5M"); break;
    case 2: lcd.print("6.0M"); break;
    case 3: lcd.print("6.5M"); break;
  }
}

void setup() {
  pinMode(10, INPUT); // for LCD shield

  Wire.begin();
  lcd.begin(16, 2);

  lcd.print("  TV Modulator  ");
  delay(1000);
  lcd.clear();

  // get/set initial EEPROM parameters
  eChannel = EEPROM.read(0);
  if ((eChannel < 21) || (eChannel > 69)) eChannel = 21;
  EEPROM.write(0, eChannel);

  eSoundIdx = EEPROM.read(1);
  if (eSoundIdx > 3) eSoundIdx = 1;
  EEPROM.write(1, eSoundIdx);

  eTest = 0;

  eOn = EEPROM.read(2);
  if (eOn) {
    lcd.setCursor(0, 0);
    lcd.print("  << ON AIR >>  ");

    mod_start(eChannel, eSoundIdx, eTest);
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("   << OFF >>    ");
  }

  lcdUpdate(eChannel, eSoundIdx, eTest);
}

void loop() {
  keypadAction();
}

