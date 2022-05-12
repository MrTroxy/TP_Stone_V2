/**
    @file MyButton.cpp
    @author David Tremblay
    @version 1.0 Avril 2022
*/
#include "MySerial.h"

using namespace std;

MySerial::MySerial(int speed, uint32_t config,int rxd, int txd){
  Serial2.begin(speed, config, rxd, txd);
};

 bool MySerial::isAvailable(){
   return(Serial2.available());
   };

int MySerial::writeIt(string cmdFormat2){
  Serial2.write(cmdFormat2.c_str());
  
  return(0);
};
 
int MySerial::readIt(char *data, int len){
  return(Serial2.readBytes(data, len));
  
};