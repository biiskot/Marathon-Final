#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include "Header.hpp"
using namespace std;

//setter caracteristiques parcours
void Parcours::setParcours(float distance, vector<float> denivele, int ravitaillements, vector<float> vent) {
    this->distanceParcours = distance;
    this->denivele = denivele;
    nb_ravitaillements = ravitaillements;
    this->vitesseVent = vent;
}

//setter stats coureur
void Coureur::SetStats(int id, int taille, int poids, int chaussure, float vitessemoy, int semaines_preparation, float hydratation) {
    this->id = id;
    this->taille = taille;
    this->poids = poids;
    this->chaussure = chaussure;
    this->vitessemoy = vitessemoy;
    this->semaines_preparation = semaines_preparation;
    this->hydratation = hydratation;
}

//getters :
int Coureur::getId() {
    return this->id;
}
int Coureur::getDistance() {
    return this->distance;
}
int Coureur::getTemps() {
    return this->temps;
}
float Coureur::getHydra() {
    return this->hydratation;
}
float Coureur::getSpeed() {
    return this->vitessemoy;
}
bool Coureur::getAbandon() {
    return this->abandon;
}

//Methode avancee d'un coureur n en temps réel :
int Coureur::distanceParc(Parcours p) {
    float vitesse = 0;
    float hydratationInit = 0.5;
    float perfs = 1;
    const float distanceParcoursMetres = p.distanceParcours * 1000;
    float PosRavit = distanceParcoursMetres / 10;
    int indiceRavit = 0;
    int indice_barreProg = 1;
    int tmpVitesse;

    float Ptmax, Pt = 0;
    float PrInit, Pr = 0;
    double PaInit, Pa = 0;
    const float RhoAir = 1.205;

    int i = 0;

    distance = 0;
    hydratation = hydratationInit;

    //Calcul vitesse initiale :

    PrInit = vitessemoy * poids * 0.98;
    PaInit = 0.5 * RhoAir * 0.137 * (taille / 100) * pow(vitessemoy, 2) * vitessemoy;
    Ptmax = PrInit;
    vitesse = (Ptmax - PaInit) / (poids * 0.98);


    while (distance < distanceParcoursMetres) {
        hydratationInit = 0.5 * vitesse / 4;
        tmpVitesse = vitesse;

        // Vent en temps réel : 
        if (distance >= 10000 && distance < 19000) i = 1;
        if (distance >= 19000 && distance < 24000) i = 2;
        if (distance >= 24000 && distance < 40000) i = 3;
        Pa = 0.5 * RhoAir * 0.137 * (taille / 100) * pow((vitesse + p.vitesseVent[i]), 2) * vitesse;
        Pr = Ptmax - Pa;
        vitesse = Pr / (poids * 0.98);

        //PREPA :
        if (semaines_preparation >= 8 && semaines_preparation < 16) {
            if (distance >= distanceParcoursMetres / 2 * (1 + (semaines_preparation - 8) / 8)) {
                vitesse = vitesse * (80 / 100);//
            }
        }
        else if (semaines_preparation == 16) {
            //vitesse ne bouge pas
        }


        //DENIVELE
        int n = 0;
        float rapport = 0;
        if (distance >= 10000 && distance < 19000) n = 1;
        if (distance >= 19000 && distance < 24000) n = 2;
        if (distance >= 24000 && distance < 40000) n = 3;
        if (p.denivele[n] >= 0) {
            rapport = p.denivele[n] * (1 / 1.5);

        }
        else if (p.denivele[n] < 0) {
            rapport = p.denivele[n] * (0.35 / 1.5);
        }
        vitesse = vitesse - (vitesse * rapport);

        //RAVITAILLEMENT / HYDRATATION :

        if (distance >= PosRavit * indiceRavit && indiceRavit <= p.nb_ravitaillements) {
            hydratation += 0.5;
            indiceRavit++;
        }

        if (hydratation == 0.5) {
            distance += vitesse * (1 / 60);
        }

        else if (hydratation < 0.5) {
            perfs = hydratationInit + hydratation / (0.5 * (temps / 3600)); // (temps en heure)
            if (perfs >= 0.4 && perfs <= 0.9) {
                vitesse = vitesse * (80 / 100);
            }
            else if (perfs > 0.9) {
                //pas de chgmt
            }
            if (perfs < 0.4) {
                //coureur abandonne dans 2km
                if (distance < distanceParcoursMetres - 2000) {
                    distance += 2000;
                    //fin course
                    cout << "Coureur abandonne, distance parcourue : " << distance << "vitesse : " << vitesse << endl;
                    abandon = true;
                    return 0;
                }
            }
        }
        temps++;
        distance = (vitesse * 1000) * temps / 3600;

        if (distance >= indice_barreProg * 1000) {
            cout << "#";
            indice_barreProg++;
        }

    }
    cout << endl;
    cout << endl;
    // cout << "Fin pour le coureur num " << id << " \nvitesse finale : " << vitesse << " km/h\ndistance parcourue : " << distance/1000 << " km\nnb de ravitaillements pris : " << indiceRavit - 1 << "\nNombre de litres d'eau bu : " << hydratation << "\ntemps total : "<< temps/60 <<" minutes \n\n"<< endl;
    vitessemoy = vitesse;
    distance = distanceParcoursMetres;
}
