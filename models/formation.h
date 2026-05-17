#ifndef FORMATION_H
#define FORMATION_H

#include <QString>

using namespace std;

class formation
{
public:
    formation(QString matiere, QString nbHeuresS1, QString nbHeuresS2, QString nbHeuresS3et4);
    ~formation();
    QString getMatiere() const;
    QString getNbHeuresS1() const;
    QString getNbHeuresS2() const;
    QString getNbHeuresS3et4() const;

private:
    QString m_matiere;
    QString m_nbHeuresS1;
    QString m_nbHeuresS2;
    QString m_nbHeuresS3et4;

};

#endif // FORMATION_H
