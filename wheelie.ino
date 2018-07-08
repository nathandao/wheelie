#include <mcp4728.h>
#include <math.h>

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0

uint32_t LEFT  = 15;
uint32_t RIGHT = 37;
uint32_t UP    = 5;
uint32_t DOWN  = 36;

uint32_t val_0, targ_0, val_1, targ_1;

uint32_t TARGET_SPEED = 500;
uint32_t MAXSPEED = 4150;
uint32_t MINSPEED = 850;

uint32_t towardsTarget(uint32_t cur, uint32_t targ) {
  int32_t dist = targ-cur;
  int32_t sig = abs(dist)/dist;
  
  if(abs(dist) > TARGET_SPEED) {
    return cur + sig*TARGET_SPEED;
  }
  return targ;
}

void setup() {
  //Serial.begin(9600);

  pinMode(LEFT , INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(UP   , INPUT);
  pinMode(DOWN , INPUT);
  
  dac.begin();
  Wire.setClock(100000);

  // Set vdd to 5000mV and channels 0 and 1 to use that as vref
  // Usable voltage range is 0-4999mV
  dac.vdd(5000); 
  dac.setVref(0,0,0,0);

  // We don't want the wheelchair to start moving on power up so set
  // power on voltages to 2500 and write those to eeprom.
  dac.voutWrite(2500,2500,2500,0);
  dac.eepromWrite();
  delay(100);

  val_0 = 2500; targ_0 = MAXSPEED;
  val_1 = 2500; targ_1 = MINSPEED;
}

void loop() {
  // Handle input
  uint32_t up    = digitalRead(UP);
  uint32_t down  = digitalRead(DOWN);
  uint32_t left  = digitalRead(LEFT);
  uint32_t right = digitalRead(RIGHT);

  if(up == LOW) {
    targ_0 = MAXSPEED;
  } else if (down == LOW) {
    targ_0 = MINSPEED;
  } else {
    targ_0 = 2500;
  }

  if(left == LOW) {
    targ_1 = MAXSPEED;
  } else if (right == LOW) {
    targ_1 = MINSPEED;
  } else {
    targ_1 = 2500;
  }

  /* Serial.print("1: "); Serial.print(targ_0); */
  /* Serial.print(" 2: "); Serial.println(targ_1); */

  uint32_t next_0 = towardsTarget(val_0, targ_0);
  uint32_t next_1 = towardsTarget(val_1, targ_1);

  // Write next values to dac
  dac.voutWrite(next_0, next_1, 2500, 0);

  // End loop
  val_0 = next_0;
  val_1 = next_1;

  //  delay(10);
}
