#include <mcp4728.h>
#include <math.h>

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0


// Maximum, minimum and stopped values for joystick. If values higher than
// max or lower than min are set, the wheelchair controller goes into
// error mode. These values have been found by experimentation.
uint32_t MAXSPEED = ;
uint32_t MINSPEED = 850;
uint32_t STOPPED = 2500;

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
  dac.outWrite(STOPPED,STOPPED,STOPPED,0);
  dac.eepromWrite();
  delay(100);
  val_0 = 2500; targ_0 = MAXSPEED;
  val_1 = 2500; targ_1 = MINSPEED;
}


// Takes two signed 8 bit values for x and y. The value 0 will represent being still.
void joystickWrite(int8_t x, int8_t y) {
  // The third channel needs to be set to the middle of Vdd and Vss or
  // the controller goes into error mode. the fourth channel is not
  // used, so it is set to 0.
  uint32_t xout = STOPPED + (x * (MAXSPEED - MINSPEED))/256;
  uint32_t yout = STOPPED + (y * (MAXSPEED - MINSPEED))/256;
    
  dac.voutWrite(xout, yout, STOPPED, 0);
}

void loop() {
  
  // Write next values to dac
  //dac.voutWrite(next_0, next_1, 2500, 0);

  
  //  delay(10);
}
