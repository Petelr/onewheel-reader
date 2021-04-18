#include "useful_funcs.h"

void printData(const unsigned char data[], int length) {
  for (int i = 0; i < length; i++) {
    unsigned char b = data[i];

    Serial.print(b,HEX);
    
  }
  Serial.println();
}

uint16_t print_16bit_Hex(const uint8_t byteArray[]){
  uint16_t displayValue;
  displayValue = byteArray[1];
  displayValue |= byteArray[0] << 8;
  Serial.println(displayValue);

  return displayValue;
}

uint16_t byte2int(const uint8_t byteArray[]){
  uint16_t displayValue;
  displayValue = byteArray[1];
  displayValue |= byteArray[0] << 8;
  
  return displayValue;
}


void print_currentAmps(char UUID[36]){
  Serial.println(UUID);
}