#include <Wire.h>
//Copyright (C) 2025,  Jochen Herrig
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.
    
#define I2C_ADDRESS 0x65 // Adresse der I2C-Komponente(TV-Modulator mit Chip MC44353)

void setup() {
  Serial.begin(9600);
  Serial.println("I2C-Sende-Tool für MC44353 TV-Modulator an Adresse 0x65");
  Serial.println("Eingabesyntax: 4 Blöcke von je 8 Nullen und Einsen!");
  Serial.println("Sollen nur 2 Blocke übertragen werden, die letzten beiden Blöcke mit '0' angeben.");
  Serial.println("z.B. schalten folgende 2 Byte den Modulator auf Kanal 23 mit Testgenerator ein:");
  Serial.println("01011110 01110111 0 0");
  Wire.begin();
}

char rx_byte = 0;
char bufStr[44];
int adr;
int i;


void evaluate() {
       char x1[10];
       char x2[10];
       char x3[10];
       char x4[10];
       byte xb[4];  

      Serial.print("Empfangen auf Seriell: ");
      Serial.println(bufStr);
      sscanf(bufStr, "%s %s %s %s", &x1, &x2, &x3, &x4 );
     // sscanf rx_str = "";                // clear the string for reuse
//      Serial.println("Nach dem parsen:");
//      Serial.println(x1);
//      Serial.println(x2);
//      Serial.println(x3);
//      Serial.println(x4);
      xb[0] = getByteFromString(x1);
      xb[1] = getByteFromString(x2);
      xb[2] = getByteFromString(x3);
      xb[3] = getByteFromString(x4);
//      Serial.println(xb[0],BIN);
//      Serial.println(xb[1],BIN);
//      Serial.println(xb[2],BIN);
//      Serial.println(xb[3],BIN);
      memset(bufStr, 0, sizeof(bufStr));
      //memset(rx_byte, 0, sizeof(rx_byte));
    
      sendData(xb);
      memset(xb, 0, sizeof(xb));
      i=0;
}

byte getByteFromString(char* str){

    // Serial.println("start byte create:");
    // Don't accept strings less than 8 long
    //if(strlen(str) < 8) return;
    byte retval = 0;
    for (int j=0; j<8; j++){
      if(!(str[j]=='0'||str[j] == '1')) {
        Serial.println("Parameter enthält kein/unzulässiges Zeichen oder ist 0 und wird nicht gesendet");
        return 0;
      }
       retval = retval << 1;
       if(str[j] == '1'){
          retval |= 1;
       }
    }
    return retval;
}

 
void sendData(byte* b2send) {
  // Beginne die I2C Übertragung
  Wire.beginTransmission(I2C_ADDRESS);
  for(int k=0; k<4; k++) {
    if(b2send[k] > 0) {
      Wire.write(b2send[k]);
      Serial.print("Gesendetes Byte: ");
      Serial.println(b2send[k]);
    }
 
  }
 
  // Beende die Übertragung
  int retVal = Wire.endTransmission(true);
  Serial.print("Ergebnis der I2C Übertragung: ");
  Serial.println(retVal);
}

void loop() {
    if (Serial.available() > 0) {    // is a character available?
      rx_byte = Serial.read();       // get the character
    
      if (rx_byte != '\n') {
      // a character of the string was received
      bufStr[i++]=rx_byte;
      } else {
      evaluate();
    }
  }
}
