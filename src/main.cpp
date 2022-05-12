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
string intToHexa(int value) {
  char buffer[10];
  sprintf(buffer , "0x%4X", value);
  return (buffer);
};

// Fonction qui permet d'envoyer un char en string
string charToString(char str, string formatStr) {
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), str);
  return (buffer);
};

// Fonction qui permet d'envoyer un int en string
string intToString(int value, string formatStr) {
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), value);
  return (buffer);
};

// Variables des informations sur le bois pour l'affichage
char nomBoisAffichage[20] = "Bois : Erable";
char typeBoisAffichage[20] = "Type : Dur";
char origineBoisAffichage[20] = "Origine : Canada";
char tempsDeSechageAffichage[40] = "Temps de séchage : 20 Secondes";
char temperatureMinAffichage[40] = "Température Min : 25 Celcius";

// Variable qui contient le compteur pour le temps de séchage
int compteurTemps = 0;
// Variable qui contient le compteur en string 
char strCompteurTemps[64];
// Variable qui contient le temps de sechage
int tempsSechage = 20;
// Variable qui contient la temperature minimum
float temperatureMinimum = 25;
// Variable qui contient la température obtenue en direct
float temperatureEnDirect = 0;
// Variable qui sert de bool pour dire si le four est en marche ou pas
int cuissonDuFourEnCours = 0;


//Thread qui permet de LOOPER et lire le contenu du port serie
//avec l'aide du la fonction getValidsDatasIfExists
void readStoneData() {
  datasRead rd = myStone->getValidsDatasIfExists();

  myStone->setLabel("lbl_car_bois", nomBoisAffichage);
  myStone->setLabel("lbl_car_type", typeBoisAffichage);
  myStone->setLabel("lbl_car_origine", origineBoisAffichage);
  myStone->setLabel("lbl_car_temps_sechage", tempsDeSechageAffichage);
  myStone->setLabel("lbl_car_temp_min", temperatureMinAffichage);

  switch(rd.id) {
    case 0x0002: { //Version

      cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
      string stoneVersion = rd.name;
      cout << "Version : " <<  stoneVersion.c_str() << "\n";

    break;
    }

    case 0x1001: { // Bouton

      string nomDuStone = rd.name;
      string nomDuBouton = "btn_demarrage";
            
      if (nomDuStone == nomDuBouton && rd.type == 1) {
        cout << "Démarrage du four!" << endl;

        // On met le four en fonctionnement
        cuissonDuFourEnCours = 1;
      }

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

  cout << string("Démarrage du logiciel effectué")  << "\n";
}

void loop() {
  temperatureEnDirect = myTemp->getTemp();
  char strTemperature[64];
  sprintf(strTemperature, "%g Celcius", temperatureEnDirect);
  myStone->setLabel("lbl_temperature", strTemperature);

  readStoneData();

  // On vérifie si la cuisson du four doit commencer
  if (cuissonDuFourEnCours == 1) {

    myStone->setLabel("lbl_infos", "Température trop basse!");

    while (temperatureEnDirect > temperatureMinimum) {
      if(compteurTemps < tempsSechage) {
        string stringTemps = "";
        char strTempsSechage[64];

        myStone->setLabel("lbl_infos", "Cuisson démarrée!");

        sprintf(strTempsSechage, "%d s", tempsSechage);

        compteurTemps++;
        sprintf(strCompteurTemps, "%d /", compteurTemps);

        stringTemps += strCompteurTemps;
        stringTemps += strTempsSechage;
                
        const char* charTempsRestant = stringTemps.c_str();

        myStone->setLabel("lbl_secondes_restantes", charTempsRestant);
        delay(1000);
      }
      else {
        myStone->setLabel("lbl_infos", "Le bois est maintenant seche!");
        cout << "Cuisson du bois terminée!" << endl;

        // Nous remettons les compteurs a 0 pour recommencer
        compteurTemps = 0;
        cuissonDuFourEnCours = 0;
        myStone->setLabel("lbl_secondes_restantes", "0 / 20s");
        break;
      } 
    }
  }

  int buttonActionT4 = myButtonT4->checkMyButton();

  if(buttonActionT4 > 2) {  //Si appuyé plus de 0.2 secondes
    Serial.println("Button T4 pressed");
    if(myStone) myStone->changePage();
  }

  int buttonActionT5 = myButtonT5->checkMyButton();

  if(buttonActionT5 > 2)  {  //Si appuyé plus de 0.2 secondes
    Serial.println("Button T5 pressed");

    if(myStone) myStone->getVersion();
  }
}