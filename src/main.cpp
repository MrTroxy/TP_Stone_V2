/* Copyright (C) 2022 David Tremblay
 * All rights reserved.
 *
 * Travail Pratique Écran Stone
 * Ecole du Web
 * Cours Systèmes embarqués (c)2022
 *
    @file     main.cpp
    @author   David Tremblay
    @version  1.0 Avril 2022
    @description
      Fichier principal du programme avec la logique applicative permettant de gérer les évènements

    platform = ESP32
    OS : Arduino
    Langage : C++

    Historique des versions
        Version    Date         Auteur                Description
        1.0        Avril 2022   David Tremblay        Première version du logiciel

    Fonctionnalités implantées
        Lecture des evénements envoyés par l'écran
        Envoyer une commande à l'écran
        Optenir la version du Firmware de l'écran
 * */

#include <Arduino.h>

#define RXD2 18//16
#define TXD2 19//17
#define BAUD_RATE 115200

#include <iostream>

using namespace std;

/**
 * David Tremblay
 */
#include "MyTemp.h"
MyTemp *myTemp = new MyTemp();

#include "MyButton.h"
MyButton *myButtonT4 = new MyButton();
MyButton *myButtonT5 = new MyButton();

#include "MyStone.h"
MyStone *myStone;

// Fonction qui permet d'envoyer un int en hexadécimal
string intToHexa(int value){
  char buffer[10];
  sprintf(buffer , "0x%4X", value);
  return (buffer);
};

// Fonction qui permet d'envoyer un char en string
string charToString(char str, string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), str);
  return (buffer);
};

// Fonction qui permet d'envoyer un int en string
string intToString(int value, string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), value);
  return (buffer);
};

string nomBois = "Erable";
string typeBois = "Dur";
string origineBois = "Canada";
string tempsDeSechage = "20 secondes";
string temperatureMin = "25 Celcius";

//Thread qui permet de LOOPER et lire le contenu du port serie
//avec l'aide du la fonction getValidsDatasIfExists
void readStoneData() {
  datasRead rd = myStone->getValidsDatasIfExists();
  //cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
  switch(rd.id){

      case 0x0002: { //Version
          cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
          string stoneVersion = rd.name;
          cout << "Version : " <<  stoneVersion.c_str() << "\n";

          break;
          }
      }

  if(rd.id<0) cout << "Data received ( id: : " << intToHexa(abs(rd.id)) << "  Command: " << rd.command << " Type: " << rd.type<< ")\n";
}


void setup() {
  Serial.begin(9600);

  Serial.println("Stone serial Txd is on pin: "+String(TXD2));
  Serial.println("Stone serial Rxd is on pin: "+String(RXD2));
  myStone = new MyStone(115200, SERIAL_8N1, RXD2, TXD2);


  myTemp->init();


  myButtonT4->init(T4);
  myButtonT4->autoSensibilisation(); //Trouve la sensibilité automatiquement

  myButtonT5->init(T5);
  myButtonT5->autoSensibilisation(); //Trouve la sensibilité automatiquement

  cout << string("Début du script")  << "\n";
}

void loop() {
  float temperature = myTemp->getTemp();
  char strTemperature[64];
  sprintf(strTemperature, "%g Celcius", temperature);
  delay(250);
  myStone->setLabel("lbltemperature", strTemperature);
  delay(250);

  Serial.println("Voici la temperature detecte : " + String(myTemp->getTemp()));

  readStoneData();

  int buttonActionT4 = myButtonT4->checkMyButton();

  if(buttonActionT4 > 2) 
  {  //Si appuyé plus de 0.2 secondes
    Serial.println("Button T4 pressed");
  if(myStone) myStone->changePage();
            }
  int buttonActionT5 = myButtonT5->checkMyButton();
      if(buttonActionT5 > 2)  {  //Si appuyé plus de 0.2 secondes
            Serial.println("Button T5 pressed");
            //Dans la version  1.2, nous allons remplacer ces lignes pour utiliser la
            //méthode getVersion()
            //char cmdFormat2[99];
            //strcpy(cmdFormat2, "ST<{\"cmd_code\":\"sys_version\",\"type\":\"system\"}>ET");
            //cout << cmdFormat2 << "\n";
            //myStone->writeIt((char*)cmdFormat2);
            if(myStone) myStone->getVersion();
          }
  }