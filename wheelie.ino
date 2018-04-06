#include <SPI.h>

#define MOSI 23
#define MISO 19
#define SCLK 18
#define SS 5

void setup() {
  pinMode(SS, OUTPUT);
  // initialize SPI:
  SPI.begin(SCK, MISO,MOSI, SS);
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  digitalWrite(SS, LOW);
  delay(3000);
  SPI.transfer(0);
  SPI.transfer(255);
  delay(3000);
  SPI.transfer(0);
  SPI.transfer(0);
}
