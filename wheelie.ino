#include <mcp4728.h>
#include <math.h>

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0


// Maximum and minimum values that can be set for joystick before
// wheelchair controller goes into error mode
uint32_t MAXSPEED = 4150;
uint32_t MINSPEED = 850;

void setup() {
  Serial.begin(9600);

  dac.begin();
  Wire.setClock(100000);

  // Set vdd to 5000mV and channels 0 and 1 to use that as vref
  // Usable voltage range is 0-4999mV
  dac.vdd(5000); 
  dac.setVref(0,0,0,0);

  // We don't want the wheelchair to start moving on power up so set
  // power on voltages to 2500 and write those to eeprom.
  dac.outWrite(2500,2500,2500,0);
  dac.eepromWrite();
  delay(100);
  val_0 = 2500; targ_0 = MAXSPEED;
  val_1 = 2500; targ_1 = MINSPEED;
}

void loop() {
  
  // Write next values to dac
  //dac.voutWrite(next_0, next_1, 2500, 0);

  
  //  delay(10);
}
