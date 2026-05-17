#ifndef PERSONNE_H
#define PERSONNE_H
#include <string>
#include <iostream>

using namespace std;

class ville;

class personne
{
protected:
    string m_nom;
    string m_prenom;
    string m_sexe;
    ville* m_villePersonne = nullptr;
    bool m_idAdmin = false;
    string getSex() const;

public:
    personne(string nom, bool idAdmin = false);
    personne(string nom, string sexe, bool idAdmin = false);
    personne(string nom, string prenom, string sexe, ville* villePersonne, bool idAdmin = false);
    virtual ~personne();
    void virtual afficher() const;
    string getNom() const;
    string getPrenom() const;
    ville* getVillePersonne() const;
    bool getIdAdmin() const;
    void setIdAdmin(bool idAdmin);
};

#endif // PERSONNE_H
