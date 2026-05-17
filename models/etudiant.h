#ifndef ETUDIANT_H
#define ETUDIANT_H
#include <string>
#include <vector>
#include <iostream>
#include "personne.h"

class cours;

using namespace std;

class etudiant : public personne
{
private:
    string m_numcarte;
    vector<cours*> m_coursEtudiant = {};
public:
    etudiant(string nom, string num);
    etudiant(string nom, string num, string sexe);
    etudiant(string nom, string prenom, string num, string sexe, ville* villeEtudiant, vector<cours*> coursEtudiant);
    void afficher() const;
};

#endif // ETUDIANT_H
