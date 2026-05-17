#include "enseignant.h"
#include "ville.h"

enseignant::enseignant(string nom, string prenom, string sexe, float salaire, string matiere, ville* villeEnseignant, cours* coursEnseignant):personne(nom, prenom, sexe, villeEnseignant)
{
    m_salaire = salaire;
    m_matiere = matiere;
    m_coursEnseignant = coursEnseignant;
}

string enseignant::getMatiere() const {
    return this->m_matiere;
}

float enseignant::verifSalaire(float salaire) const {
    // Je ne sais pas ce que doit faire cette fonction...
    return 0;
}

float enseignant::getSalaire() const {
    return this->m_salaire;
}

void enseignant::afficher() const {
    cout << this-> m_nom;

    if (!m_matiere.empty()) {
        cout << " en " << this->m_matiere;
    }

    if (this->m_villePersonne && !this->m_villePersonne->getNom().empty()) {
        cout << " (Habitant " << m_villePersonne->getNom() << ")";
    }

    cout << endl;
}
