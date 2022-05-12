/* Copyright (C) 2022
 * All rights reserved.
 *
 * Travail Pratique
 * Ecole du Web
* Cours Systèmes embarqués (c)2022
 *  
    @file     MySerial.h
    @author   David Tremblay
    @version  1.0 Avril 2022
    @description
      Démonstration comment utiliser le PORT SERIE RS232 UART ESP32

    platform = ESP32
    OS = Arduino
    Langage : C++


    Historique des versions
        Version    Date       Auteur       Description
        1.0      Avril 2022     DT      Première version du logiciel

**/
#ifndef MYSERIAL_H
#define MYSERIAL_H

#include <Arduino.h>
#include <string>

using namespace std;

class MySerial {
    private:
       
    public:
        MySerial(int speed, uint32_t config, int rxd, int txd);
        ~MySerial() { };

        bool isAvailable();
        int writeIt(string cmdFormat2);
        int readIt(char *data, int len);
};
#endif
