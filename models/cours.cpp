#include "cours.h"

cours::cours(string intitule)
{
    m_intitule = intitule;
}

string cours::getIntitule() const {
    return this->m_intitule;
}
