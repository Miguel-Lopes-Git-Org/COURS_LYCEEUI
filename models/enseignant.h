#ifndef ENSEIGNANT_H
#define ENSEIGNANT_H
#include <string>
#include <iostream>
#include "personne.h"

using namespace std;

class cours;

class enseignant: public personne
{
private:
    float m_salaire;
    string m_matiere;
    cours* m_coursEnseignant;
public:
    enseignant(string nom, string prenom, string sexe, float salaire, string matiere, ville* villeEnseignant, cours* coursEnseignant);
    float verifSalaire(float salaire) const;
    float getSalaire() const;
    string getMatiere() const;
    void afficher() const;
};

#endif // ENSEIGNANT_H
