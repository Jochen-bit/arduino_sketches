#include <Wire.h>
// Aktiviert TV-UHF Modulator MC44353 auf Kanal 23, mit Testgenerartor
#define I2C_ADDRESS 0x65 // Adresse der I2C-Komponente

void setup() {
  // Initialisierung der I2C-Bibliothek
  Wire.begin();
  
  delay(1500);
  
  // Sende die beiden Byte-Werte
  sendData();
}

void loop() {
  // In der loop-Funktion passiert nichts, da wir nur einmal senden
}

void sendData() {
  // Beginne die I2C Übertragung
  Wire.beginTransmission(I2C_ADDRESS);
  
  // Sende die ersten 2 Byte C1 und C0
  Wire.write(136); //C1
  Wire.write(72); //C0
  Wire.write(94); //FM
  Wire.write(117); //FL
  
  // Beende die Übertragung
  Wire.endTransmission();
 
}
