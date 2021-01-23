#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <ctime>
#include "Header.hpp"
#include "methodes.hpp"
#include <iomanip>
#include <algorithm>
using namespace std;


const int nb_coureurs = 100; // Nb coureurs constant

//
string const coureursString("fichiers/listcoureurs.txt");
ofstream coureursList(coureursString.c_str());
int tailleRand = 0, poidsRand = 0, chaussureRand = 0, preparationRand = 0, nCoureurs = 0;
float vitessemoyRand = 0, hydratationRand = 0;

int k = 0, tailleTmp = 0, poidsTmp = 0, chaussureTmp = 0, semainesprepaTmp = 0, idCoureur = 1;
float vitessemoyTmp = 0, hydratationTmp = 0;
string ligneTmp;

int main() {
    srand(time(nullptr));

    //Ecriture valeurs random pour n coureurs :
    if (coureursList) {

        //boucle n lignes de Coureurs :
        while (nCoureurs < nb_coureurs) {
            tailleRand = rand() % (200 - 130) + 130;
            poidsRand = rand() % (120 - 45) + 45;
            chaussureRand = rand() % (300 - 100) + 100;
            vitessemoyRand = rand() % (20 - 7) + 7;
            preparationRand = rand() % (16 - 8) + 8;
            hydratationRand = 0.5;

            coureursList << nCoureurs << " ";
            coureursList << tailleRand << " ";
            coureursList << poidsRand << " ";
            coureursList << chaussureRand << " ";
            coureursList << vitessemoyRand << " ";
            coureursList << preparationRand << " ";
            coureursList << hydratationRand << " ";
            coureursList << endl;
            nCoureurs++;
        }
    }
    else {
        cout << "Erreur ouverture de " << coureursString << endl;
    }

    //création vecteur de coureurs :
    vector<Coureur> c;


    //Lecture -> assignation variables -> parametres SetStats du Coureur k :
    ifstream lectureCoureurs("fichiers/listcoureurs.txt");

    if (lectureCoureurs) {
        
        while (getline(lectureCoureurs, ligneTmp)) {
            lectureCoureurs >> idCoureur;
            lectureCoureurs >> tailleTmp;
            lectureCoureurs >> poidsTmp;
            lectureCoureurs >> chaussureTmp;
            lectureCoureurs >> vitessemoyTmp;
            lectureCoureurs >> semainesprepaTmp;
            lectureCoureurs >> hydratationTmp;
            c.push_back(Coureur());
            c[k].SetStats(idCoureur, tailleTmp, poidsTmp, chaussureTmp, vitessemoyTmp, semainesprepaTmp, hydratationTmp); // On attribue les valeurs écrites précédemment 
                                                                                                                          // aux stats de chaque coureur du vecteur
            k++;
            idCoureur++;
        }
    }
    else {
        cout << "Fichier coureurs non ouvert" << endl;
    }

    // creation Parcours;
    Parcours p;

    //Lecture fichier parcours :
    ifstream parcours1("fichiers/parcours1.txt");
    if (parcours1) {
        float tmp;
        parcours1 >> p.distanceParcours;
        //Affichage des deniveles du parcours avant le debut de la course:
        cout << "\nDifferents deniveles du parcours : " << endl;
        for (int i = 0; i < 4; i++) {
            parcours1 >> tmp;
            p.denivele.push_back(tmp);
            cout << p.denivele[i] << "%" << endl;
        }

        parcours1 >> p.nb_ravitaillements;
        //Affichage vitesses vent variable :
        cout << "\nDifferentes vitesses du vent du parcours : " << endl;
        for (int ij = 0; ij < 4; ij++) {
            parcours1 >> tmp;
            p.vitesseVent.push_back(tmp);
            cout << p.vitesseVent[ij] << "km/h" << endl;
        }
    }

    else {
        cout << "Fichier parcours non ouvert\n" << endl;
    }

    p.setParcours(p.distanceParcours, p.denivele, p.nb_ravitaillements, p.vitesseVent);

    //Affichage autres details du parcours :
    cout << "\nTaille parcours (km) : " << p.distanceParcours << endl << "Nombre de ravitaillements : " << p.nb_ravitaillements << endl;


    //AVANCEE DES COUREURS : 
    ofstream resultats("fichiers/resultats.txt");
    for (int nb = 0; nb < nb_coureurs; nb++) {
        c[nb].distanceParc(p); // Appel de la fonction d'avancée pour chaque coureurs
    }

    sort(c.begin(), c.end(), [](Coureur& a, Coureur& b) { return a.getTemps() < b.getTemps(); }); // fonction lambda permettant de trier le vecteur coureur 
                                                                                                  // en focntion du temps de chacun
    // Ecriture resultats n coureurs :
    for (int nb = 0; nb < nb_coureurs; nb++) {
        if (resultats) {
            if (c[nb].getAbandon()== true)

            {
                resultats << " - Id coureur : "<< setw(4) <<c[nb].getId() << " ";
                resultats << setprecision(3) << " - vitesse : "<<c[nb].getSpeed() << " ";
                resultats << " - distance parcourue (m) : "<< c[nb].getDistance() << " ";
                resultats << " - Temps total : " << c[nb].getTemps() / 3600 << ":" << setw(2) << c[nb].getTemps() / 60 % 60 << ":" << setw(2) << c[nb].getTemps() % 60; //temps
                resultats << " - Litres d'eau bue : " << c[nb].getHydra() << " ";
                resultats << " - Le coureur a abandonné "; // si le coureur abandonne on le spécfifie dans le fichier resultats.
                resultats << endl;
            }
            else {
                resultats << " - Id coureur : " << setw(4) << c[nb].getId();
                resultats << setprecision(3) << " - vitesse : " << setw(4)<<c[nb].getSpeed();
                resultats << " - distance parcourue (m) : " << c[nb].getDistance() << " ";
                resultats << " - Temps total : " << c[nb].getTemps() / 3600 << ":"<<setw(2)<< c[nb].getTemps() / 60%60<< ":"<<setw(2)<<c[nb].getTemps() % 60; //temps
                resultats << " - Litres d'eau bue : " << c[nb].getHydra() << " ";
                resultats << endl;
            }
        }

        else {
            cout << "fichier resultats non ouvert" << endl;
        }
    }
}