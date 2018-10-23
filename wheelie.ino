#include <mcp4728.h>
#include <math.h>

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0


// Maximum, minimum and stopped values for joystick. If values higher than
// max or lower than min are set, the wheelchair controller goes into
// error mode. These values have been found by experimentation.
uint32_t MAXSPEED = 4150;
uint32_t MINSPEED = 850;
uint32_t STOPPED = 2500;
unsigned long KEEPALIVETIME = (1000*30);

bool moving = false;
unsigned long stopAt = 0;

unsigned long lastMovement = 0;

void setup() {
  Serial.begin(9600);

  dac.begin();
  //  Wire.setClock(100000);

  // Set vdd to 5000mV and channels 0 and 1 to use that as vref
  // Usable voltage range is 0-4999mV
  dac.vdd(5000); 
  dac.setVref(0,0,0,0);

  // We don't want the wheelchair to start moving on power up so set
  // power on voltages to 2500 and write those to eeprom.
  delay(10);
  dac.voutWrite(STOPPED,STOPPED,STOPPED,0);
  dac.eepromWrite();

}


// Takes two signed 8 bit values for x and y. The value 0 will represent being still.
void joystickWrite(int8_t x, int8_t y) {
  // The third channel needs to be set to the middle of Vdd and Vss or
  // the controller goes into error mode. the fourth channel is not
  // used, so it is set to 0.
  uint32_t xout = STOPPED + (x * (MAXSPEED - MINSPEED))/256;
  uint32_t yout = STOPPED + (y * (MAXSPEED - MINSPEED))/256;
  
  dac.voutWrite(xout, yout, STOPPED, 0);
  lastMovement = millis();
}

void keepAlive() {
  unsigned long now = millis();
  if(lastMovement + KEEPALIVETIME < now ) {
    joystickWrite(20,0);
    delay(500);
    joystickWrite(0,0);
    delay(1000);
    joystickWrite(-20,0);
    delay(500);
    joystickWrite(0,0);
    lastMovement = now;
    
  }
}

void loop() {
  int x = 0, y = 0;

  unsigned long now = millis();

  if(moving && stopAt <= now) {
    joystickWrite(0,0);
  }

  if(Serial.available() >=2 ) {
    x = Serial.read();
    y = Serial.read();

    Serial.write("got bytes "); Serial.write(x); Serial.write(" ") ;Serial.write(y); Serial.write("\n");

    moving = true;
    stopAt = now + 3000;
    
    joystickWrite(x, y);
  }
  
  keepAlive();
}
