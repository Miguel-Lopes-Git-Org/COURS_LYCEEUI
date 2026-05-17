#ifndef DBCONNEXION_H
#define DBCONNEXION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include "QSqlQuery"

using namespace std;

class dbConnexion {
public:
    dbConnexion();
    ~dbConnexion();
    void dbClose(); // Pour fermer la connexion manuellement
    bool isConnected() const; // Pour vérifier si ça a marché

private:
    void fermerConnection();
    bool m_connected;
    QString m_connectionName; // Pour sauvegarder le nom de la connexion
};

#endif
