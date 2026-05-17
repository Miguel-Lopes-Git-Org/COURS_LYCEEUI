#include "personne.h"
#include "ville.h"

personne::personne(string nom, bool idAdmin): m_nom(nom), m_villePersonne(nullptr), m_idAdmin(idAdmin)
{
}

personne::personne(string nom, string sexe, bool idAdmin): m_nom(nom), m_villePersonne(nullptr), m_idAdmin(idAdmin)
{
    if (sexe == "F" || sexe == "M") {
        this->m_sexe = sexe;
    } else {
        throw invalid_argument("Le sexe doit être F ou M");
    }
}

personne::personne(string nom, string prenom, string sexe, ville* villePersonne, bool idAdmin): m_nom(nom), m_prenom(prenom), m_villePersonne(villePersonne), m_idAdmin(idAdmin)
{
    if (sexe == "F" || sexe == "M") {
        this->m_sexe = sexe;
    } else {
        throw invalid_argument("Le sexe doit être F ou M");
    }
}

personne::~personne() {}

void personne::afficher() const {
    cout << "La personne s'appelle " << this-> m_nom;

    if (!this->m_sexe.empty()) {
        if (this->m_sexe == "F") {
            cout << " cette personne est une femme";
        } else {
            cout << " cette personne est un homme";
        }
    }

    if (this->m_villePersonne && !this->m_villePersonne->getNom().empty()) {
        cout << " (Habitant " << this->m_villePersonne->getNom() << ")";
    }

    cout << endl;
}

string personne::getNom() const {
    return this->m_nom;
}

string personne::getPrenom() const {
    return this->m_prenom;
}

ville* personne::getVillePersonne() const {
    return this->m_villePersonne;
}

string personne::getSex() const {
    return this->m_sexe;
}

bool personne::getIdAdmin() const {
    return this->m_idAdmin;
}

void personne::setIdAdmin(bool idAdmin) {
    this->m_idAdmin = idAdmin;
}
