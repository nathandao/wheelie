#include <mcp4728.h>
#include <math.h>

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0

uint32_t val_0, targ_0, val_1, targ_1;

uint32_t TARGET_SPEED = 50;

uint32_t towardsTarget(uint32_t cur, uint32_t targ) {
  int32_t dist = targ-cur;
  int32_t sig = abs(dist)/dist;
  
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

  val_0 = 2500; targ_0 = 4999;
  val_1 = 2500; targ_1 = 0;
}

void loop() {
  if(val_0 <= 0) {targ_0 = 4999;}
  if(val_0 >= 4999) {targ_0 = 0;}
  if(val_1 <= 0) {targ_1 = 4999;}
  if(val_1 >= 4999) {targ_1 = 0;}
  
  uint32_t next_0 = towardsTarget(val_0, targ_0);
  uint32_t next_1 = towardsTarget(val_1, targ_1);

  dac.voutWrite(next_0, next_1, 0, 0);
  
  val_0 = next_0;
  val_1 = next_1;
  
  delay(1);
}
