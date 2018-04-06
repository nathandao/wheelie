#include <SPI.h>
#include <math.h>

#define MOSI 23
#define MISO 19
#define SCLK 18
#define SS 5

#define ADR_WIPER0       0b00000000;
#define CMD_READ         0b00001100;
#define CMD_WRITE        0b00000000;

#define ADR_VOLATILE     0b00000000;
#define ADR_NON_VOLATILE 0b00100000;

#define ACCELERATION 5

uint32_t current, target;

void setWiper(uint32_t cs, uint32_t value) {
  // the write command is two bytes wide
  // first 7 bytes for address and command,
  // 9 for the value in range 0...256.
  Serial.print("Value: "); Serial.print(value); Serial.print("\n");
  
  uint8_t byte1 = 0x00, byte2 = 0x00;

  byte1 = CMD_WRITE;
  byte1 |= ADR_WIPER0;
  
  if (value > 255)
    { byte1 |= 0b00000001; } // maximum is 256 so 0x100, byte2 is 0
  else
    { byte2 = (uint8_t)(value & 0x00ff); }
  
  digitalWrite(cs, LOW);
  SPI.transfer(byte1);
  SPI.transfer(byte2);
  digitalWrite(cs, HIGH);
}

uint32_t towardsTarget(uint32_t cur, uint32_t targ) {
  int32_t dist = targ-cur;
  int32_t sig = abs(dist)/dist;

  /* Serial.print("sig: ");Serial.print(sig);Serial.print("\n"); */
  /* Serial.print("target: ");Serial.print(target);Serial.print("\n"); */
  
  if(abs(dist) > ACCELERATION) {
    return cur + sig*ACCELERATION;
  }
  return targ;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(SS, OUTPUT);
  // initialize SPI:
  SPI.begin(SCK, MISO,MOSI, SS);
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));

  current = 0; target = 0;
}

void loop() {
  if(current == 0) {target = 256;}
  if(current == 256) {target = 0;}

  uint32_t next = towardsTarget(current, target);
  setWiper(SS, next);

  current = next;
  delay(100);
}
