#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
using namespace std;


class Parcours {
public:
    float distanceParcours = 0;
    vector<float> denivele;
    int nb_ravitaillements = 0;
    vector<float> vitesseVent;

    //constructeur:
    Parcours();

    //methodes:
    // Récupère infos du fichier du parcours puis attribue ces val au parcours p
    void setParcours(float, vector<float>, int, vector<float>);
};

Parcours::Parcours() {
}

class Coureur {

private:
    bool abandon = false;
    int id = 0;
    int taille = 0;
    int poids = 0;
    int chaussure = 0;
    float vitessemoy = 0;
    int semaines_preparation = 0;
    float hydratation = 0;
    int distance = 0;
    int temps = 0;

public:
    //Constructeur :
    Coureur();

    //methodes:
    void SetStats(int, int, int, int, float, int, float); // Setter des statistiques du coureur
    int distanceParc(Parcours);

    //getters attributs Coureur :
    int getDistance();
    int getTemps();
    float getHydra();
    float getSpeed();
    int getId();
    bool getAbandon();
};


Coureur::Coureur() {

}
