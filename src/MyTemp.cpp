/*
 * MyTemp.cpp
 *
 *  Created on:   Avril 2022
 *      Author: David Tremblay
 */
#include "MyTemp.h"

#define DHTTYPE DHT22
#define DHTPIN 23

MyTemp::MyTemp(/* args */){
}

MyTemp::~MyTemp() {
}

void MyTemp::init() {
  dht = new DHT_Unified(DHTPIN, DHTTYPE);
  dht->begin();
}

float MyTemp::getTemp(){
    sensors_event_t event;
    dht->temperature().getEvent(&event);
    return event.temperature;
}