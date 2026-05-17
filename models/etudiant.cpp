#include "etudiant.h"
#include "ville.h"
#include "cours.h"

etudiant::etudiant(string nom, string num):personne(nom), m_numcarte(num), m_coursEtudiant({})
{
    cout << "contructeur de l'étudiant " << nom << " " << num << endl;
}

etudiant::etudiant(string nom, string num, string sexe):personne(nom, sexe), m_numcarte(num), m_coursEtudiant({})
{
    cout << "contructeur de l'étudiant " << nom << " " << num << " " << sexe << endl;
}

etudiant::etudiant(string nom, string prenom, string num, string sexe, ville* villeEtudiant, vector<cours*> coursEtudiant):personne(nom, prenom, sexe, villeEtudiant), m_numcarte(num), m_coursEtudiant(coursEtudiant)
{
    cout << "contructeur de l'étudiant " << nom << " " << num << " " << sexe << endl;
}

void etudiant::afficher() const {
    cout << "L'etudiant " << getNom();

    if (!this->getSex().empty()) {
        if (this->getSex() == "F") {
            cout << " est une femme";
        } else {
            cout << " est un homme";
        }
    }

    if (this->m_villePersonne && !this->m_villePersonne->getNom().empty()) {
        cout << " (Habitant " << this->m_villePersonne->getNom() << ")";
    }

    if (!this->m_coursEtudiant.empty()) {
        cout << " faisant les cours suivant :";
        for (unsigned long i = 0; i < m_coursEtudiant.size(); i ++) {
            if (i != 0 && i != this->m_coursEtudiant.size()) {
                cout << ", ";
            } else {
                cout << " ";
            }
            cout << m_coursEtudiant[i]->getIntitule();
        }
    }

    if (!this->m_numcarte.empty()) {
        cout << " [Carte n°: " << this->m_numcarte << "]";
    }

    cout << endl;
}
