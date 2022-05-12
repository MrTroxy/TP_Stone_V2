/*
 * MyTemp.h
 *
 *  Created on: Avril 2022
 *      Author: David Tremblay
 */

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Dht_U.h>

#ifndef MyTemp_H
#define MyTemp_H

class MyTemp
{
private:
    DHT_Unified *dht;
public:
    MyTemp(/* args */);
    ~MyTemp();
    float getTemp();
    void init();
};

#endif