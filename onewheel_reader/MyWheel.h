#include "Arduino.h"
#include "ArduinoBLE.h"
#include "useful_funcs.h"

#define UUID_RIDING_MODE "e659f302-ea98-11e3-ac10-0800200c9a66"
#define UUID_HARDWARE_VER "E659F318-EA98-11E3-AC10-0800200C9A66"
#define UUID_FIRMWARE_VER "E659F311-EA98-11E3-AC10-0800200C9A66"
#define UUID_KEY "e659f3ff-ea98-11e3-ac10-0800200c9a66"
#define UUID_SPEED_RPM "e659f30b-ea98-11e3-ac10-0800200c9a66"
#define UUID_LIFE_ODOMETER "e659f319-ea98-11e3-ac10-0800200c9a66"
#define UUID_ONEWHEEL_SERVICE "e659f300-ea98-11e3-ac10-0800200c9a66"
#define UUID_BATTERY_REMAINING "e659f303-ea98-11e3-ac10-0800200c9a66"
#define UUID_CURRENT_AMPS "e659f312-ea98-11e3-ac10-0800200c9a66"
#define UUID_SAFETY_HEADROOM "e659f317-ea98-11e3-ac10-0800200c9a66"
#define UUID_TEMPERATURE "e659f310-ea98-11e3-ac10-0800200c9a66"



class MyWheel
{
  public:
    MyWheel(BLEDevice *);
    char wheelName[8];
    void connect();
    void read_HW_ver();
    bool locked();
    bool unlock();
    BLEDevice *theWheel;
    BLEService *owService;


  private:
    byte OW_KEY[20];
    // uint8_t _lifeOdometer;
    // uint8_t _batteryRemaining;
    // float _currentAmps;
    // float _temperature;
};