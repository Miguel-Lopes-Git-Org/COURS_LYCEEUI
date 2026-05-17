#ifndef VILLE_H
#define VILLE_H

#include <string>

using namespace std;

class ville
{
public:
    ville(string nom, string codePostale);
    ~ville();
    string getNom() const;
    string getCodePostale() const;

private:
    string m_nom;
    string m_codePostale;
};

#endif // VILLE_H
