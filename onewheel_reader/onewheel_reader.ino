// The goal of this sketch is to find out how to 
// correctly write values to characteristics 
// Mar 17, 2021

// To-Do
// Let MyWheel instantialize without BLEDevice
// So the onewheel name could be MyWheel's attribute


#include <ArduinoBLE.h>
#include "MyWheel.h"
#include "useful_funcs.h"
// #include "LowPower.h"
LiquidCrystal_I2C lcd(0x3f, 16, 2);

void _onConnect(BLEDevice c){
  Serial.println("On Connect Event Handler. ");
}

void _onDisConnect(BLEDevice c){
  Serial.println("On DisConnect Event Handler. Sleep Mode");
  lcd.clear();
  lcd.print("Disconnected.");
  // LowPower.deepSleep()
}



void setup() {
  // put your setup code here, to run once:

  lcd.begin();
  lcd.clear();
  Serial.begin(9600);
  // while(!Serial);

  //Init BLE
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");

    while(1);
  }
  BLE.setEventHandler(BLEConnected, _onConnect);
  BLE.setEventHandler(BLEDisconnected, _onDisConnect);
  BLE.scan();
  Serial.println("Start scanning.....");
  lcd.clear();
  lcd.print("Scanning...");
}

void loop() {
  BLEDevice bleDevice = BLE.available();
  // Serial.println("Scanning...");
  if (bleDevice.localName() == "ow173725") {
    BLE.stopScan();
    MyWheel weee( & bleDevice);
    weee.connect();
    weee.read_HW_ver();
    Serial.println("I'm done read SN");
    
    unsigned long wait_time = 500;
    uint8_t time_out = 0;
    const uint8_t time_out_limit = 3;
    
    while(weee.locked() && time_out < time_out_limit)
    {
      weee.unlock();
      lcd.clear();
      lcd.print("unlocking...");
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

    BLECharacteristic speed_RPM = weee.theWheel->characteristic(UUID_SPEED_RPM);

    BLECharacteristic temperature = weee.theWheel->characteristic(UUID_TEMPERATURE);

    // Print 1's if subscribed successfully
    Serial.println(currentAmps.subscribe());
    Serial.println(batteryRemaining.subscribe());
    Serial.println(temperature.subscribe());

    uint16_t batt;
    uint16_t rpm;
    float a;
    float b;
    int_least16_t temperature_;
    uint8_t controlFlag = 1;
    float metric_speed;
    float speed_kph;
    float speed_mph;
    while(1)
    {
      // if (temperature.valueUpdated() || batteryRemaining.valueUpdated() || currentAmps.valueUpdated())
      if (Serial.available())
      {
        controlFlag = Serial.parseInt();
        Serial.println(Serial.read());
      }
      if (currentAmps.valueUpdated() && controlFlag)
      {
        while (weee.locked()){
          weee.unlock();
          Serial.println("Unlocking............");
          Serial.println("...");
          delay(500);
        }
        currentAmps.read();
        batteryRemaining.read();
        temperature.read();
        speed_RPM.read();

        batt = byte2int(batteryRemaining.value());

        tempArray[0] = currentAmps.value()[1];
        tempArray[1] = currentAmps.value()[0];
        memcpy(&amps, &tempArray, sizeof(amps));
        a = amps*1.0/512.0;

        // tempArray[0] = temperature.value()[1];
        // tempArray[1] = temperature.value()[0];
        // memcpy(&temperature_, &tempArray, sizeof(temperature_));
        // b = temperature_*1.0/512.0;
        rpm = byte2int(speed_RPM.value());
        metric_speed = (848/(2*3.14159)/1000) * (rpm*3.14159*2/60);  // meter per hour
        speed_kph = metric_speed * 3.6;
        speed_mph = metric_speed * 2.23694;

        lcd.setCursor(0,0);
        lcd.print(a);
        lcd.print("A");
        lcd.print(" ");
        lcd.print(batt);
        lcd.print("%");
        lcd.print("     ");

        lcd.setCursor(0,1);
        lcd.print(int(speed_mph));
        lcd.print("mph");
        lcd.print("     ");

        // Serial.println("-------");
        Serial.print("BATT:");
        Serial.print(batt);
        Serial.print("\t");
        Serial.print("Current:");
        Serial.print(a);
        Serial.print(" A");
        Serial.print("\t");
        Serial.print("RPM:");
        Serial.print(rpm);
        Serial.print("\t");
        Serial.print("Speed:");
        Serial.print(speed_mph);
        Serial.print("\t");
        Serial.print("Control Temp: ");
        Serial.print(temperature.value()[0], DEC);    // controller temperature in celsius
        Serial.print("\tMotor Temp: ");
        Serial.print(temperature.value()[1], DEC);    // motor      temperature in celsius
        // byte 2,3 do not look like representing batt temp, as it went from 98.6f to 95f, byte 2,3 didn't change in value
        // also something weird: sometimes byte 2,3 are just 0's even if I reconnect arduino.
        Serial.println();
      }
    }
  }
}

