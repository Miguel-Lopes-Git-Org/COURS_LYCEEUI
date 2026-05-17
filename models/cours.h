#ifndef COURS_H
#define COURS_H
#include <string>
#include <iostream>
#include "personne.h"

using namespace std;

class cours
{
private:
    string m_intitule;
public:
    cours(string intitule);
    string getIntitule() const;
};

#endif // COURS_H
