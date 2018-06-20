#include <mcp4728.h>
#include <math.h>

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0

uint32_t current, target;
uint32_t TARGET_SPEED = 50;

uint32_t towardsTarget(uint32_t cur, uint32_t targ) {
  int32_t dist = targ-cur;
  int32_t sig = abs(dist)/dist;

  /* Serial.print("sig: ");Serial.print(sig);Serial.print("\n"); */
  /* Serial.print("target: ");Serial.print(target);Serial.print("\n"); */
  
  if(abs(dist) > TARGET_SPEED) {
    return cur + sig*TARGET_SPEED;
  }
  return targ;
}

void setup() {
  Serial.begin(9600);
  dac.begin();

  // Set vdd to 5000mV and channels 0 and 1 to use that as vref
  // Usable voltage range is 0-4999mV
  dac.vdd(5000); 
  dac.setVref(0,0,0,0);

  // We don't want the wheelchair to start moving on power up so set
  // power on voltages to 2500 and write those to eeprom.
  dac.voutWrite(2500,2500,0,0);
  dac.eepromWrite();
  delay(100);

  current = 2500;
  target = 4999;
}

void loop() {
  if(current <= 0) {target = 4999;}
  if(current >= 4999) {target = 0;}

  uint32_t next = towardsTarget(current, target);
  Serial.print("now: "); Serial.println(next);
  dac.voutWrite(0, next);

  current = next;
  delay(1);
}
