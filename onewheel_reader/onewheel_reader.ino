// The goal of this sketch is to find out how to 
// correctly write values to characteristics 
// Mar 17, 2021

// To-Do
// Let MyWheel instantialize without BLEDevice
// So the onewheel name could be MyWheel's attribute


#include <ArduinoBLE.h>
#include "MyWheel.h"
#include "useful_funcs.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);

  //Init BLE
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");

    while(1);
  }
  BLE.scan();

}

void loop() {
  BLEDevice bleDevice = BLE.available();
  Serial.println("Scanning...");
  if (bleDevice.localName() == "ow173725") {
    BLE.stopScan();
    MyWheel weee( & bleDevice);
    weee.connect();
    Serial.println("I'm done connecting!");
    weee.read_HW_ver();
    Serial.println("I'm done read SN");
    
    unsigned long wait_time = 500;
    uint8_t time_out = 0;
    const uint8_t time_out_limit = 3;
    
    while(weee.locked() && time_out < time_out_limit)
    {
      weee.unlock();
      
      delay(wait_time);
      time_out++;

      if (time_out >= time_out_limit)
      {
        Serial.println("Unlock Timeout");
      }

    }
    Serial.print("Lock status: ");
    Serial.println(weee.locked());

    BLECharacteristic lifeOdometer = weee.theWheel->characteristic(UUID_LIFE_ODOMETER);
    lifeOdometer.read();
    Serial.print("life odometer: ");
    print_16bit_Hex(lifeOdometer.value());
    
    while(1);
  }
}

