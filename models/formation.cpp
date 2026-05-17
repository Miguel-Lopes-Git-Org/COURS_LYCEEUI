#include "formation.h"

formation::formation(QString matiere, QString nbHeuresS1, QString nbHeuresS2, QString nbHeuresS3et4) {
    m_matiere = matiere;
    m_nbHeuresS1 = nbHeuresS1;
    m_nbHeuresS2 = nbHeuresS2;
    m_nbHeuresS3et4 = nbHeuresS3et4;
}

QString formation::getMatiere() const {
    return m_matiere;
}

QString formation::getNbHeuresS1() const {
    return m_nbHeuresS1;
}

QString formation::getNbHeuresS2() const {
    return m_nbHeuresS2;
}

QString formation::getNbHeuresS3et4() const {
    return m_nbHeuresS3et4;
}
