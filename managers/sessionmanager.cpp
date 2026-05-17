#include "sessionmanager.h"
#include "dbConnexion.h"
#include "passwordutils.h"

#include <QDebug>
#include <QSqlRecord>

namespace {
bool databaseHasColumn(const QString &tableName, const QString &columnName)
{
    // on passe par information_schema pour rester compatible avec plusieurs versions du schema
    QSqlQuery query;
    query.prepare(
        "SELECT 1 "
        "FROM information_schema.columns "
        "WHERE table_name = :table_name AND column_name = :column_name"
    );
    query.bindValue(":table_name", tableName);
    query.bindValue(":column_name", columnName);

    if (!query.exec()) {
        qDebug() << "Erreur verification colonne" << tableName << columnName << ":" << query.lastError().text();
        return false;
    }

    return query.next();
}

bool verifyPassword(const QString &typedPassword, const QString &storedPassword, const QString &dbSalt)
{
    // nouveau format: hash en base + sel dans la colonne password_salt
    if (!dbSalt.isEmpty()) {
        const QString computed = passwordUtils::hashPassword(typedPassword, dbSalt);
        return computed == storedPassword;
    }

    // format de transition: "salt:hash" stocke dans la colonne password
    if (storedPassword.contains(':')) {
        const QStringList parts = storedPassword.split(':');
        if (parts.size() == 2) {
            const QString computed = passwordUtils::hashPassword(typedPassword, parts[0]);
            return computed == parts[1];
        }
    }

    // Compatibilite avec les anciens mots de passe stockes en clair.
    return typedPassword == storedPassword;
}
}

sessionManager::sessionManager() {}

bool sessionManager::login(string username, string password) const{
    // Variable de la valeur de retour
    bool isConnected = false;

    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;

    query.prepare("SELECT * FROM getLogin WHERE username = :username");

    query.bindValue(":username", QString::fromStdString(username));

    if(!query.exec()) {
        qDebug() << "Erreur lors de la requête SQL :" << query.lastError().text();
        return false;
    }

    if(query.next()) {
        const QSqlRecord record = query.record();
        const QString storedPassword = query.value("password").toString();
        QString storedSalt;

        // indexOf permet de lire la colonne seulement si elle existe vraiment dans la vue getLogin
        const int saltIndex = record.indexOf("password_salt");
        if (saltIndex >= 0) {
            storedSalt = query.value(saltIndex).toString();
        }

        if (verifyPassword(QString::fromStdString(password), storedPassword, storedSalt)) {
            qDebug() << "Mot de passe correct. Initialisation de la session...";

            int id = query.value("id").toInt();
            string nom = query.value("nom").toString().toStdString();
            string prenom = query.value("prenom").toString().toStdString();
            string sexe = query.value("sexe").toString().toStdString();
            string role = query.value("role").toString().toStdString();
            bool needPasswordChange = false;
            bool isAdmin = (role == "Admin");

            // fallback pratique si la vue SQL ne renvoie pas encore ces colonnes
            const int needChangeIndex = record.indexOf("needPasswordChange");
            if (needChangeIndex >= 0) {
                needPasswordChange = query.value(needChangeIndex).toBool();
            }

            const int isAdminIndex = record.indexOf("isAdmin");
            if (isAdminIndex >= 0) {
                isAdmin = query.value(isAdminIndex).toBool();
            }

            string nomVille = query.value("ville").toString().toStdString();
            string cpVille = query.value("codePostal").toString().toStdString();
            ville* villeUser = new ville(nomVille, cpVille);

            session* s = session::getSession();

            // on route vers le bon type d'utilisateur pour garder les infos metier du role
            if (role == "Enseignant") {
                float salaire = query.value("salaire").toFloat();
                string matiere = query.value("matiere").toString().toStdString();
                s->setSessionTeacher(id, nom, prenom, sexe, villeUser, matiere, nullptr, salaire, needPasswordChange);
            }
            else if (role == "Etudiant") {
                string numCarte = query.value("numCarte").toString().toStdString();
                s->setSessionStudent(id, nom, prenom, sexe, numCarte, villeUser, vector<cours*>(), needPasswordChange);
            }
            else {
                s->setSessionAdmin(id, nom, prenom, sexe, villeUser, needPasswordChange, isAdmin);
            }

            qDebug() << "Utilisateur connecté avec le rôle :" << QString::fromStdString(role);
            isConnected = true;
        } else {
            qDebug() << "Mot de passe incorrect.";
        }
    }

    // Retourner le status de connexion
    return isConnected;
}

bool sessionManager::updatePassword(int userId, string newPassword) const
{
    const QString newPasswordQt = QString::fromStdString(newPassword);
    QString errorMessage;

    if (!passwordUtils::isStrongPassword(newPasswordQt, &errorMessage)) {
        qDebug() << "Mot de passe invalide :" << errorMessage;
        return false;
    }

    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    // selon le schema on stocke soit hash seul + colonne salt, soit salt:hash dans password
    const bool hasSaltColumn = databaseHasColumn("personne", "password_salt");
    const QString salt = passwordUtils::generateSalt();
    const QString hash = passwordUtils::hashPassword(newPasswordQt, salt);
    const QString storedPassword = hasSaltColumn ? hash : (salt + ":" + hash);

    QSqlQuery query;
    if (hasSaltColumn) {
        query.prepare(
            "UPDATE personne "
            "SET password = :password, password_salt = :password_salt, needPasswordChange = FALSE "
            "WHERE id = :id"
        );
        query.bindValue(":password_salt", salt);
    } else {
        query.prepare(
            "UPDATE personne "
            "SET password = :password, needPasswordChange = FALSE "
            "WHERE id = :id"
        );
    }

    query.bindValue(":password", storedPassword);
    query.bindValue(":id", userId);

    if (!query.exec()) {
        qDebug() << "Erreur updatePassword:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() <= 0) {
        qDebug() << "Aucun utilisateur mis a jour pour l'id" << userId;
        return false;
    }

    session::getSession()->setNeedPasswordChange(false);
    return true;
}
