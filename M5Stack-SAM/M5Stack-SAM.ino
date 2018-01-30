#include <M5Stack.h>
#include "EEPROM.h"
#include "utility/DHT12.h"
#include <Adafruit_Sensor.h>			   	// appBME280
#include <Adafruit_BME280.h>			   	// appBME280
#include <Wire.h>
#include "SimpleBeacon.h"

SimpleBeacon ble;
DHT12 dht12;

#define SEALEVELPRESSURE_HPA (1013.25)		// appBME280
Adafruit_BME280 bme280; 				    // appBME280  
bool status;  							  	// appBME280

#define TFT_GREY 0x5AEB
#define TFT_BROWN 0x38E0

#define TFT_W 320
#define TFT_H 240

#define EEPROM_SIZE 64

unsigned int sys_menucolor;
unsigned int sys_windowcolor;
unsigned int sys_menutextcolor;

byte menuidx = 0;
byte menuidxmin = 0;
byte menuidxmax = 3;
byte menulock = 0;
boolean menuIsMenu = HIGH;

unsigned long tmp_tmr = 0;

void setup(void) {
  Serial.begin(115200);
  
 status = bme280.begin(0x76);  
// if (!status) {  
//    Serial.println("Could not find a valid BME280 sensor, check wiring!");  
//    while (1);  
// }  
  

  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to initialise EEPROM");
    while(HIGH);
  }


  M5.begin();
  Wire.begin();

  M5.lcd.setBrightness(byte(EEPROM.read(0)));

  sys_menutextcolor = TFT_WHITE;
  sys_menucolor = setrgb(0,0,128);
  sys_windowcolor = TFT_GREY;

  menuUpdate(menuidx, menulock);
  
  
}

void loop() {
  if(M5.BtnA.wasPressed()) {
    if(menuidx>menuidxmin)menuidx--;
    menuUpdate(menuidx, menulock);
  }
  if(M5.BtnC.wasPressed()) {
    if(menuidx<menuidxmax)menuidx++;
    menuUpdate(menuidx, menulock);
  }
  if(M5.BtnB.wasPressed()) {
    if(menuIsMenu){
      menulock = menuidx;
      menuidx = 0;
      menuUpdate(menuidx, menulock);
    }else{
      menuRunApp(menuidx, menulock);
    }
  }
  M5.update();
}




