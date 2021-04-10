#include "MyWheel.h"

MyWheel::MyWheel(BLEDevice *wheel)
{
  // MyWheel constructor
  
  theWheel = wheel;
  char key[20] = {0x09, 0x8E, 0x56, 0xAC, 0x7C, 0x88, 0xB2, 0x4A, 0xF4, 0x3C, 0x7C, 0x5B, 0x93, 0x5C, 0x92, 0x90, 0x55, 0x9C, 0x22, 0xB8};

  for (int i=0; i<20; i++)
  {
    OW_KEY[i] = key[i];
  }
}

void MyWheel::connect()
{
  // This method is used for connecting to a OW
 
  Serial.println("I'm in connect..");
  Serial.print("Found ");
  Serial.print(theWheel->address());
  Serial.print(" '");
  Serial.print(theWheel->localName());
  Serial.print("' ");
  Serial.print(theWheel->advertisedServiceUuid());
  Serial.println();
  theWheel->connect();
  theWheel->discoverAttributes();

  // BLEService service = theWheel->service(UUID_ONEWHEEL_SERVICE);
  // owService = &service;

}

void MyWheel::read_HW_ver()
{
  // Read serial number from a connected OW
  // Used to determine if the connection is successfull
  
  BLECharacteristic hardware_ver = theWheel->characteristic(UUID_HARDWARE_VER);
  hardware_ver.read();
  Serial.print("HARDWARE: ");
  printData(hardware_ver.value(), hardware_ver.valueLength());
}

bool MyWheel::locked()
{
  // Determine if onewheel is locked
  // Return true when it is locked

  // Characteristic Discovery
  
  uint8_t int_mode; 
  BLECharacteristic ridingMode = theWheel->characteristic(UUID_RIDING_MODE);
  ridingMode.read();

  int_mode = ridingMode.value()[ridingMode.valueLength()-1];
  if (int_mode == 0) {
    
    return true;
  }
  else{

    return false;
  }
}  

bool MyWheel::unlock()
{

  // initializing BLE service
  BLEService service = theWheel->service(UUID_ONEWHEEL_SERVICE);
  owService = &service;

  // initializing BLE characteristics
  BLECharacteristic hardware_ver_BLEchar = owService->characteristic(UUID_HARDWARE_VER);

  BLECharacteristic key_BLEchar = owService->characteristic(UUID_KEY);

  BLECharacteristic speed_rpm_BLEchar = owService->characteristic(UUID_SPEED_RPM);

  BLECharacteristic firmware_ver_BLEchar = owService->characteristic(UUID_FIRMWARE_VER);

  hardware_ver_BLEchar.read();

  byte hardwareWrite[2] = {0x01,0x00};
  hardware_ver_BLEchar.writeValue(hardwareWrite,2);

  firmware_ver_BLEchar.read();

  byte firmwareWrite[2] = {0x00,0x00};
  firmware_ver_BLEchar.writeValue(firmwareWrite,2);

  if(key_BLEchar.writeValue(OW_KEY,20)) 
  {
    Serial.println("Write key success.");
  }
  else
  {
    Serial.println("Write key failed. ");
  }

  speed_rpm_BLEchar.writeValue((byte)0x0101);
}