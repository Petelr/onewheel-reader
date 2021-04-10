#include "useful_funcs.h"

void printData(const unsigned char data[], int length) {
  for (int i = 0; i < length; i++) {
    unsigned char b = data[i];

    Serial.print(b,HEX);
    
  }
  Serial.println();
}

void print_16bit_Hex(const uint8_t byteArray[]){
  uint16_t displayValue;
  displayValue = byteArray[1];
  displayValue |= byteArray[0] << 8;
  Serial.println(displayValue);
}

void log(String string){
    Serial.println(string);
}