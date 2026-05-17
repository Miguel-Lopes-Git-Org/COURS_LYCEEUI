#include "ville.h"

ville::ville(string nom, string codePostale) {
    m_nom = nom;
    m_codePostale = codePostale;
}

ville::~ville() {}

string ville::getNom() const {
    return m_nom;
}

string ville::getCodePostale() const {
    return m_codePostale;
}
