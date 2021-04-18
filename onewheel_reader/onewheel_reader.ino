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
  Serial.println("Start scanning.....");

}

void loop() {
  BLEDevice bleDevice = BLE.available();
  // Serial.println("Scanning...");
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

    BLECharacteristic batteryRemaining = weee.theWheel->characteristic(UUID_BATTERY_REMAINING);
    batteryRemaining.read();
    Serial.print("Battery Remaining: ");
    print_16bit_Hex(batteryRemaining.value());

    // Current amps is an int16, divided by 512 to get the float value
    BLECharacteristic currentAmps = weee.theWheel->characteristic(UUID_CURRENT_AMPS);
    currentAmps.read();
    Serial.print("Current Amps: ");
    int_least16_t amps;
    char tempArray[2];
    tempArray[0] = currentAmps.value()[1];
    tempArray[1] = currentAmps.value()[0];
    memcpy(&amps, &tempArray, sizeof(amps));
    Serial.println(amps*1.0/512.0); 

    BLECharacteristic temperature = weee.theWheel->characteristic(UUID_TEMPERATURE);

    Serial.println(currentAmps.subscribe());
    Serial.println(batteryRemaining.subscribe());
    Serial.println(temperature.subscribe());

    uint16_t batt;
    float a;
    float b;
    int_least16_t temperature_;
    while(1)
    {
      // if (temperature.valueUpdated() || batteryRemaining.valueUpdated() || currentAmps.valueUpdated())
      if (currentAmps.valueUpdated())
      {
        while (weee.locked()){
          weee.unlock();
          // Serial.println("Unlocking............");
          Serial.println("...");
          delay(500);
        }
        currentAmps.read();
        // batteryRemaining.read();
        // temperature.read();

        // batt = byte2int(batteryRemaining.value());

        tempArray[0] = currentAmps.value()[1];
        tempArray[1] = currentAmps.value()[0];
        memcpy(&amps, &tempArray, sizeof(amps));
        a = amps*1.0/512.0;

        // tempArray[0] = temperature.value()[1];
        // tempArray[1] = temperature.value()[0];
        // memcpy(&temperature_, &tempArray, sizeof(temperature_));
        // b = temperature_*1.0/512.0;

        // Serial.println("-------");
        // Serial.print("BATT:");
        // Serial.print(batt);
        // Serial.print("\t");
        Serial.print("Current:");
        Serial.print(a);
        Serial.print(" A");
        // Serial.print("\t");
        // Serial.print("Temp:");
        
        // Serial.println(temperature.value()[0], HEX);
        // Serial.println(temperature.value()[1], HEX);
        // Serial.println(temperature.value()[2], HEX);
        // Serial.println(temperature.value()[3], HEX);
        Serial.println();
      }
    }
  }
}

