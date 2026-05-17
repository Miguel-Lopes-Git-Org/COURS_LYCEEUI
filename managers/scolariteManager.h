#ifndef SCOLARITEMANAGER_H
#define SCOLARITEMANAGER_H

#include <vector>
#include <QString>
#include "formation.h"

struct enseignantData {
    QString nom;
    QString prenom;
    QString matiere;
    QString ville;
    QString salaire;
};

struct etudiantDetailsData {
    int etudiantId;
    QString nom;
    QString prenom;
    QString numCarte;
    QString ville;
    QString photoUrl;
    QString cours;
};

class scolariteManager
{
public:
    scolariteManager();
    vector<formation*> getAllForamtions() const;
    bool getAllEnseignants(vector<enseignantData> &enseignants) const;
    bool getAllEtudiantsDetails(vector<etudiantDetailsData> &etudiants) const;
};

#endif // SCOLARITEMANAGER_H
