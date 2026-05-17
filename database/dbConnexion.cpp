#include "dbConnexion.h"

dbConnexion::dbConnexion() {
    // on ouvre une connexion Qt SQL par defaut pour le cycle de vie de cet objet
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("192.168.1.251");
    db.setDatabaseName("COURS_LYCEE");
    db.setUserName("ecole");
    db.setPassword("");

    if (!db.open()) {
        qDebug() << "Erreur connexion :" << db.lastError().text();
        m_connected = false;
    } else {
        qDebug() << "Connexion réussie !";
        m_connectionName = db.connectionName();
        m_connected = true;
    }
}

bool dbConnexion::isConnected() const {
    return m_connected;
}

void dbConnexion::fermerConnection() {
    // on met le handle db dans un scope local pour forcer sa destruction avant removeDatabase
    {
        QSqlDatabase db = QSqlDatabase::database(m_connectionName);
        if (db.isOpen()) {
            db.close();
            qDebug() << "Connexion à la base de données fermée";
        }
    }

    // removeDatabase passe bien uniquement si aucun handle QSqlDatabase actif ne traine
    QSqlDatabase::removeDatabase(m_connectionName);
}

dbConnexion::~dbConnexion() {
    fermerConnection();
}

void dbConnexion::dbClose() {
    fermerConnection();
}
