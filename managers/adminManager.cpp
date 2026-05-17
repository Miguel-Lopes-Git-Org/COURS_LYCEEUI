#include "adminManager.h"

#include "dbConnexion.h"
#include "passwordutils.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

adminManager::adminManager() {}

bool adminManager::databaseHasColumn(const QString &tableName, const QString &columnName) const
{
    // cette verif nous laisse gerer plusieurs versions de schema sans casser le code
    QSqlQuery query;
    query.prepare(
        "SELECT 1 "
        "FROM information_schema.columns "
        "WHERE table_name = :table_name AND column_name = :column_name"
    );
    query.bindValue(":table_name", tableName);
    query.bindValue(":column_name", columnName);

    if (!query.exec()) {
        qDebug() << "Erreur verif colonne" << tableName << columnName << ":" << query.lastError().text();
        return false;
    }

    return query.next();
}

bool adminManager::getVilles(vector<villeAdminData> &villes) const
{
    villes.clear();

    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    if (!query.exec("SELECT id, nom, codePostal FROM ville ORDER BY nom")) {
        qDebug() << "Erreur getVilles:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        villeAdminData row;
        row.id = query.value("id").toInt();
        row.nom = query.value("nom").toString();
        row.codePostal = query.value("codePostal").toString();
        villes.push_back(row);
    }

    return true;
}

bool adminManager::createVille(const QString &nom, const QString &codePostal) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO ville(nom, codePostal) VALUES(:nom, :codePostal)");
    query.bindValue(":nom", nom.trimmed());
    query.bindValue(":codePostal", codePostal.trimmed());

    if (!query.exec()) {
        qDebug() << "Erreur createVille:" << query.lastError().text();
        return false;
    }

    return true;
}

bool adminManager::updateVille(int id, const QString &nom, const QString &codePostal) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE ville SET nom = :nom, codePostal = :codePostal WHERE id = :id");
    query.bindValue(":nom", nom.trimmed());
    query.bindValue(":codePostal", codePostal.trimmed());
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur updateVille:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool adminManager::deleteVille(int id) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM ville WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur deleteVille:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool adminManager::getMatieres(vector<matiereAdminData> &matieres) const
{
    matieres.clear();

    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    if (!query.exec("SELECT id, nom, alias FROM matiere ORDER BY nom")) {
        qDebug() << "Erreur getMatieres:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        matiereAdminData row;
        row.id = query.value("id").toInt();
        row.nom = query.value("nom").toString();
        row.alias = query.value("alias").toString();
        matieres.push_back(row);
    }

    return true;
}

bool adminManager::createMatiere(const QString &nom, const QString &alias) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO matiere(nom, alias) VALUES(:nom, :alias)");
    query.bindValue(":nom", nom.trimmed());
    query.bindValue(":alias", alias.trimmed());

    if (!query.exec()) {
        qDebug() << "Erreur createMatiere:" << query.lastError().text();
        return false;
    }

    return true;
}

bool adminManager::updateMatiere(int id, const QString &nom, const QString &alias) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE matiere SET nom = :nom, alias = :alias WHERE id = :id");
    query.bindValue(":nom", nom.trimmed());
    query.bindValue(":alias", alias.trimmed());
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur updateMatiere:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool adminManager::deleteMatiere(int id) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM matiere WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur deleteMatiere:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool adminManager::getCours(vector<coursAdminData> &cours) const
{
    cours.clear();

    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    if (!query.exec("SELECT id, nom FROM cours ORDER BY nom")) {
        qDebug() << "Erreur getCours:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        coursAdminData row;
        row.id = query.value("id").toInt();
        row.nom = query.value("nom").toString();
        cours.push_back(row);
    }

    return true;
}

bool adminManager::createCours(const QString &nom) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO cours(nom) VALUES(:nom)");
    query.bindValue(":nom", nom.trimmed());

    if (!query.exec()) {
        qDebug() << "Erreur createCours:" << query.lastError().text();
        return false;
    }

    return true;
}

bool adminManager::updateCours(int id, const QString &nom) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE cours SET nom = :nom WHERE id = :id");
    query.bindValue(":nom", nom.trimmed());
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur updateCours:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool adminManager::deleteCours(int id) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM cours WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur deleteCours:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool adminManager::getEnseignants(vector<enseignantAdminData> &enseignants) const
{
    enseignants.clear();

    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    if (!query.exec(
            "SELECT e.id AS enseignant_id, p.id AS personne_id, p.nom, p.prenom, p.sexe, "
            "p.ville_id, COALESCE(v.nom, '') AS ville, COALESCE(p.username, '') AS username, "
            "COALESCE(p.isAdmin, FALSE) AS isAdmin, COALESCE(p.needPasswordChange, FALSE) AS needPasswordChange, "
            "e.matiere_id, COALESCE(m.nom, '') AS matiere, e.salaire "
            "FROM enseignant e "
            "JOIN personne p ON p.id = e.personne_id "
            "LEFT JOIN ville v ON v.id = p.ville_id "
            "LEFT JOIN matiere m ON m.id = e.matiere_id "
            "ORDER BY p.nom, p.prenom"
        )) {
        qDebug() << "Erreur getEnseignants admin:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        enseignantAdminData row;
        row.enseignantId = query.value("enseignant_id").toInt();
        row.personneId = query.value("personne_id").toInt();
        row.nom = query.value("nom").toString();
        row.prenom = query.value("prenom").toString();
        row.sexe = query.value("sexe").toString();
        row.villeId = query.value("ville_id").toInt();
        row.ville = query.value("ville").toString();
        row.username = query.value("username").toString();
        row.isAdmin = query.value("isAdmin").toBool();
        row.needPasswordChange = query.value("needPasswordChange").toBool();
        row.matiereId = query.value("matiere_id").toInt();
        row.matiere = query.value("matiere").toString();
        row.salaire = query.value("salaire").toDouble();
        enseignants.push_back(row);
    }

    return true;
}

bool adminManager::createEnseignant(const QString &nom, const QString &prenom, const QString &sexe, int villeId,
                                    const QString &username, int matiereId, double salaire,
                                    QString &generatedPassword) const
{
    // on genere un mdp temporaire fort pour le premier login
    generatedPassword = passwordUtils::generateRandomStrongPassword(14);

    dbConnexion db;
    if (!db.isConnected()) {
        generatedPassword.clear();
        return false;
    }

    QSqlDatabase sqlDb = QSqlDatabase::database();
    // transaction pour garantir que personne + enseignant sont crees ensemble
    if (!sqlDb.transaction()) {
        qDebug() << "Erreur ouverture transaction createEnseignant:" << sqlDb.lastError().text();
        return false;
    }

    // fallback de stockage selon presence de la colonne password_salt
    const bool hasSaltColumn = databaseHasColumn("personne", "password_salt");
    const QString salt = passwordUtils::generateSalt();
    const QString hash = passwordUtils::hashPassword(generatedPassword, salt);
    const QString storedPassword = hasSaltColumn ? hash : (salt + ":" + hash);

    QSqlQuery qPersonne;
    if (hasSaltColumn) {
        qPersonne.prepare(
            "INSERT INTO personne(nom, prenom, username, password, password_salt, needPasswordChange, ville_id, sexe, isAdmin) "
            "VALUES(:nom, :prenom, :username, :password, :password_salt, TRUE, :ville_id, :sexe, FALSE) RETURNING id"
        );
        qPersonne.bindValue(":password_salt", salt);
    } else {
        qPersonne.prepare(
            "INSERT INTO personne(nom, prenom, username, password, needPasswordChange, ville_id, sexe, isAdmin) "
            "VALUES(:nom, :prenom, :username, :password, TRUE, :ville_id, :sexe, FALSE) RETURNING id"
        );
    }

    qPersonne.bindValue(":nom", nom.trimmed());
    qPersonne.bindValue(":prenom", prenom.trimmed());
    qPersonne.bindValue(":username", username.trimmed());
    qPersonne.bindValue(":password", storedPassword);
    qPersonne.bindValue(":ville_id", villeId);
    qPersonne.bindValue(":sexe", sexe.trimmed());

    if (!qPersonne.exec() || !qPersonne.next()) {
        qDebug() << "Erreur insert personne enseignant:" << qPersonne.lastError().text();
        // rollback evite un compte partiellement cree
        sqlDb.rollback();
        generatedPassword.clear();
        return false;
    }

    const int personneId = qPersonne.value(0).toInt();

    QSqlQuery qEnseignant;
    qEnseignant.prepare("INSERT INTO enseignant(personne_id, matiere_id, salaire) VALUES(:personne_id, :matiere_id, :salaire)");
    qEnseignant.bindValue(":personne_id", personneId);
    qEnseignant.bindValue(":matiere_id", matiereId);
    qEnseignant.bindValue(":salaire", salaire);

    if (!qEnseignant.exec()) {
        qDebug() << "Erreur insert enseignant:" << qEnseignant.lastError().text();
        sqlDb.rollback();
        generatedPassword.clear();
        return false;
    }

    if (!sqlDb.commit()) {
        qDebug() << "Erreur commit createEnseignant:" << sqlDb.lastError().text();
        sqlDb.rollback();
        generatedPassword.clear();
        return false;
    }

    return true;
}

bool adminManager::updateEnseignant(int enseignantId, int personneId, const QString &nom, const QString &prenom,
                                    const QString &sexe, int villeId, const QString &username, int matiereId, double salaire) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlDatabase sqlDb = QSqlDatabase::database();
    // on garde update personne + update enseignant dans une seule transaction
    if (!sqlDb.transaction()) {
        qDebug() << "Erreur ouverture transaction updateEnseignant:" << sqlDb.lastError().text();
        return false;
    }

    QSqlQuery qPersonne;
    qPersonne.prepare(
        "UPDATE personne "
        "SET nom = :nom, prenom = :prenom, sexe = :sexe, ville_id = :ville_id, username = :username "
        "WHERE id = :id"
    );
    qPersonne.bindValue(":nom", nom.trimmed());
    qPersonne.bindValue(":prenom", prenom.trimmed());
    qPersonne.bindValue(":sexe", sexe.trimmed());
    qPersonne.bindValue(":ville_id", villeId);
    qPersonne.bindValue(":username", username.trimmed());
    qPersonne.bindValue(":id", personneId);

    if (!qPersonne.exec()) {
        qDebug() << "Erreur update personne enseignant:" << qPersonne.lastError().text();
        sqlDb.rollback();
        return false;
    }

    QSqlQuery qEnseignant;
    qEnseignant.prepare(
        "UPDATE enseignant SET matiere_id = :matiere_id, salaire = :salaire WHERE id = :id AND personne_id = :personne_id"
    );
    qEnseignant.bindValue(":matiere_id", matiereId);
    qEnseignant.bindValue(":salaire", salaire);
    qEnseignant.bindValue(":id", enseignantId);
    qEnseignant.bindValue(":personne_id", personneId);

    if (!qEnseignant.exec()) {
        qDebug() << "Erreur update enseignant:" << qEnseignant.lastError().text();
        sqlDb.rollback();
        return false;
    }

    if (!sqlDb.commit()) {
        qDebug() << "Erreur commit updateEnseignant:" << sqlDb.lastError().text();
        sqlDb.rollback();
        return false;
    }

    return true;
}

bool adminManager::deleteEnseignant(int enseignantId, int personneId) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlDatabase sqlDb = QSqlDatabase::database();
    // suppression atomique pour eviter un compte orphelin en cas d'erreur
    if (!sqlDb.transaction()) {
        qDebug() << "Erreur ouverture transaction deleteEnseignant:" << sqlDb.lastError().text();
        return false;
    }

    QSqlQuery qEnseignant;
    qEnseignant.prepare("DELETE FROM enseignant WHERE id = :id AND personne_id = :personne_id");
    qEnseignant.bindValue(":id", enseignantId);
    qEnseignant.bindValue(":personne_id", personneId);

    if (!qEnseignant.exec()) {
        qDebug() << "Erreur delete enseignant:" << qEnseignant.lastError().text();
        sqlDb.rollback();
        return false;
    }

    QSqlQuery qPersonne;
    qPersonne.prepare("DELETE FROM personne WHERE id = :id");
    qPersonne.bindValue(":id", personneId);

    if (!qPersonne.exec()) {
        qDebug() << "Erreur delete personne enseignant:" << qPersonne.lastError().text();
        sqlDb.rollback();
        return false;
    }

    if (!sqlDb.commit()) {
        qDebug() << "Erreur commit deleteEnseignant:" << sqlDb.lastError().text();
        sqlDb.rollback();
        return false;
    }

    return true;
}

bool adminManager::getEtudiants(vector<etudiantAdminData> &etudiants) const
{
    etudiants.clear();

    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    if (!query.exec(
            "SELECT et.id AS etudiant_id, p.id AS personne_id, et.numCarte, p.nom, p.prenom, p.sexe, "
            "p.ville_id, COALESCE(v.nom, '') AS ville, COALESCE(p.username, '') AS username, "
            "COALESCE(et.photoUrl, '') AS photoUrl, "
            "COALESCE(string_agg(c.nom, ', ' ORDER BY c.nom), 'Aucun cours') AS cours, "
            "COALESCE(p.isAdmin, FALSE) AS isAdmin, COALESCE(p.needPasswordChange, FALSE) AS needPasswordChange "
            "FROM etudiant et "
            "JOIN personne p ON p.id = et.personne_id "
            "LEFT JOIN ville v ON v.id = p.ville_id "
            "LEFT JOIN etudiant_cours ec ON ec.etudiant_id = et.id "
            "LEFT JOIN cours c ON c.id = ec.cours_id "
            "GROUP BY et.id, p.id, et.numCarte, p.nom, p.prenom, p.sexe, p.ville_id, v.nom, p.username, et.photoUrl, p.isAdmin, p.needPasswordChange "
            "ORDER BY p.nom, p.prenom"
        )) {
        qDebug() << "Erreur getEtudiants admin:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        etudiantAdminData row;
        row.etudiantId = query.value("etudiant_id").toInt();
        row.personneId = query.value("personne_id").toInt();
        row.numCarte = query.value("numCarte").toInt();
        row.nom = query.value("nom").toString();
        row.prenom = query.value("prenom").toString();
        row.sexe = query.value("sexe").toString();
        row.villeId = query.value("ville_id").toInt();
        row.ville = query.value("ville").toString();
        row.username = query.value("username").toString();
        row.photoUrl = query.value("photoUrl").toString();
        row.cours = query.value("cours").toString();
        row.isAdmin = query.value("isAdmin").toBool();
        row.needPasswordChange = query.value("needPasswordChange").toBool();
        etudiants.push_back(row);
    }

    return true;
}

bool adminManager::createEtudiant(const QString &nom, const QString &prenom, const QString &sexe, int villeId,
                                  const QString &username, const QString &photoUrl,
                                  const QVector<int> &coursIds, QString &generatedPassword) const
{
    // on genere un mdp temporaire comme pour les autres comptes crees par l'admin
    generatedPassword = passwordUtils::generateRandomStrongPassword(14);

    dbConnexion db;
    if (!db.isConnected()) {
        generatedPassword.clear();
        return false;
    }

    QSqlDatabase sqlDb = QSqlDatabase::database();
    // transaction necessaire car on touche plusieurs tables liees
    if (!sqlDb.transaction()) {
        qDebug() << "Erreur ouverture transaction createEtudiant:" << sqlDb.lastError().text();
        return false;
    }

    // meme strategie de compatibilite que pour createEnseignant
    const bool hasSaltColumn = databaseHasColumn("personne", "password_salt");
    const QString salt = passwordUtils::generateSalt();
    const QString hash = passwordUtils::hashPassword(generatedPassword, salt);
    const QString storedPassword = hasSaltColumn ? hash : (salt + ":" + hash);

    QSqlQuery qPersonne;
    if (hasSaltColumn) {
        qPersonne.prepare(
            "INSERT INTO personne(nom, prenom, username, password, password_salt, needPasswordChange, ville_id, sexe, isAdmin) "
            "VALUES(:nom, :prenom, :username, :password, :password_salt, TRUE, :ville_id, :sexe, FALSE) RETURNING id"
        );
        qPersonne.bindValue(":password_salt", salt);
    } else {
        qPersonne.prepare(
            "INSERT INTO personne(nom, prenom, username, password, needPasswordChange, ville_id, sexe, isAdmin) "
            "VALUES(:nom, :prenom, :username, :password, TRUE, :ville_id, :sexe, FALSE) RETURNING id"
        );
    }

    qPersonne.bindValue(":nom", nom.trimmed());
    qPersonne.bindValue(":prenom", prenom.trimmed());
    qPersonne.bindValue(":username", username.trimmed());
    qPersonne.bindValue(":password", storedPassword);
    qPersonne.bindValue(":ville_id", villeId);
    qPersonne.bindValue(":sexe", sexe.trimmed());

    if (!qPersonne.exec() || !qPersonne.next()) {
        qDebug() << "Erreur insert personne etudiant:" << qPersonne.lastError().text();
        sqlDb.rollback();
        generatedPassword.clear();
        return false;
    }

    const int personneId = qPersonne.value(0).toInt();

    QSqlQuery qEtudiant;
    qEtudiant.prepare("INSERT INTO etudiant(personne_id, photoUrl) VALUES(:personne_id, :photoUrl)");
    qEtudiant.bindValue(":personne_id", personneId);
    qEtudiant.bindValue(":photoUrl", photoUrl.trimmed());

    if (!qEtudiant.exec()) {
        qDebug() << "Erreur insert etudiant:" << qEtudiant.lastError().text();
        sqlDb.rollback();
        generatedPassword.clear();
        return false;
    }

    int etudiantId = -1;
    if (qEtudiant.lastInsertId().isValid()) {
        etudiantId = qEtudiant.lastInsertId().toInt();
    }

    // certains drivers peuvent ne pas remonter lastInsertId donc on relit via personne_id
    if (etudiantId <= 0) {
        QSqlQuery qFindEtudiant;
        qFindEtudiant.prepare("SELECT id FROM etudiant WHERE personne_id = :personne_id");
        qFindEtudiant.bindValue(":personne_id", personneId);
        if (!qFindEtudiant.exec() || !qFindEtudiant.next()) {
            qDebug() << "Erreur lecture id etudiant:" << qFindEtudiant.lastError().text();
            sqlDb.rollback();
            generatedPassword.clear();
            return false;
        }
        etudiantId = qFindEtudiant.value(0).toInt();
    }

    // insertion des cours coches dans la table de liaison many-to-many
    for (int coursId : coursIds) {
        if (coursId <= 0) {
            continue;
        }

        QSqlQuery qCours;
        qCours.prepare("INSERT INTO etudiant_cours(etudiant_id, cours_id) VALUES(:etudiant_id, :cours_id)");
        qCours.bindValue(":etudiant_id", etudiantId);
        qCours.bindValue(":cours_id", coursId);

        if (!qCours.exec()) {
            qDebug() << "Erreur insert etudiant_cours:" << qCours.lastError().text();
            sqlDb.rollback();
            generatedPassword.clear();
            return false;
        }
    }

    if (!sqlDb.commit()) {
        qDebug() << "Erreur commit createEtudiant:" << sqlDb.lastError().text();
        sqlDb.rollback();
        generatedPassword.clear();
        return false;
    }

    return true;
}

bool adminManager::updateEtudiant(int etudiantId, int personneId, const QString &nom, const QString &prenom,
                                  const QString &sexe, int villeId, const QString &username,
                                  const QString &photoUrl, const QVector<int> &coursIds) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlDatabase sqlDb = QSqlDatabase::database();
    // transaction pour garder personne, etudiant et ses cours coherents
    if (!sqlDb.transaction()) {
        qDebug() << "Erreur ouverture transaction updateEtudiant:" << sqlDb.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "UPDATE personne "
        "SET nom = :nom, prenom = :prenom, sexe = :sexe, ville_id = :ville_id, username = :username "
        "WHERE id = :id"
    );
    query.bindValue(":nom", nom.trimmed());
    query.bindValue(":prenom", prenom.trimmed());
    query.bindValue(":sexe", sexe.trimmed());
    query.bindValue(":ville_id", villeId);
    query.bindValue(":username", username.trimmed());
    query.bindValue(":id", personneId);

    if (!query.exec()) {
        qDebug() << "Erreur updateEtudiant:" << query.lastError().text();
        sqlDb.rollback();
        return false;
    }

    QSqlQuery qEtudiant;
    qEtudiant.prepare("UPDATE etudiant SET photoUrl = :photoUrl WHERE id = :id AND personne_id = :personne_id");
    qEtudiant.bindValue(":photoUrl", photoUrl.trimmed());
    qEtudiant.bindValue(":id", etudiantId);
    qEtudiant.bindValue(":personne_id", personneId);

    if (!qEtudiant.exec()) {
        qDebug() << "Erreur update photo etudiant:" << qEtudiant.lastError().text();
        sqlDb.rollback();
        return false;
    }

    // on resynchronise la liste de cours en mode delete + insert
    QSqlQuery qDeleteCours;
    qDeleteCours.prepare("DELETE FROM etudiant_cours WHERE etudiant_id = :etudiant_id");
    qDeleteCours.bindValue(":etudiant_id", etudiantId);
    if (!qDeleteCours.exec()) {
        qDebug() << "Erreur delete etudiant_cours (update):" << qDeleteCours.lastError().text();
        sqlDb.rollback();
        return false;
    }

    for (int coursId : coursIds) {
        if (coursId <= 0) {
            continue;
        }

        QSqlQuery qCours;
        qCours.prepare("INSERT INTO etudiant_cours(etudiant_id, cours_id) VALUES(:etudiant_id, :cours_id)");
        qCours.bindValue(":etudiant_id", etudiantId);
        qCours.bindValue(":cours_id", coursId);

        if (!qCours.exec()) {
            qDebug() << "Erreur insert etudiant_cours (update):" << qCours.lastError().text();
            sqlDb.rollback();
            return false;
        }
    }

    if (!sqlDb.commit()) {
        qDebug() << "Erreur commit updateEtudiant:" << sqlDb.lastError().text();
        sqlDb.rollback();
        return false;
    }

    return true;
}

bool adminManager::getEtudiantCoursIds(int etudiantId, QVector<int> &coursIds) const
{
    coursIds.clear();

    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT cours_id FROM etudiant_cours WHERE etudiant_id = :etudiant_id ORDER BY cours_id");
    query.bindValue(":etudiant_id", etudiantId);

    if (!query.exec()) {
        qDebug() << "Erreur getEtudiantCoursIds:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        coursIds.push_back(query.value(0).toInt());
    }

    return true;
}

bool adminManager::deleteEtudiant(int etudiantId, int personneId) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlDatabase sqlDb = QSqlDatabase::database();
    // ordre de suppression important pour respecter les cles etrangeres
    if (!sqlDb.transaction()) {
        qDebug() << "Erreur ouverture transaction deleteEtudiant:" << sqlDb.lastError().text();
        return false;
    }

    QSqlQuery qCours;
    qCours.prepare("DELETE FROM etudiant_cours WHERE etudiant_id = :etudiant_id");
    qCours.bindValue(":etudiant_id", etudiantId);
    if (!qCours.exec()) {
        qDebug() << "Erreur delete etudiant_cours:" << qCours.lastError().text();
        sqlDb.rollback();
        return false;
    }

    QSqlQuery qEtudiant;
    qEtudiant.prepare("DELETE FROM etudiant WHERE id = :id AND personne_id = :personne_id");
    qEtudiant.bindValue(":id", etudiantId);
    qEtudiant.bindValue(":personne_id", personneId);

    if (!qEtudiant.exec()) {
        qDebug() << "Erreur delete etudiant:" << qEtudiant.lastError().text();
        sqlDb.rollback();
        return false;
    }

    QSqlQuery qPersonne;
    qPersonne.prepare("DELETE FROM personne WHERE id = :id");
    qPersonne.bindValue(":id", personneId);

    if (!qPersonne.exec()) {
        qDebug() << "Erreur delete personne etudiant:" << qPersonne.lastError().text();
        sqlDb.rollback();
        return false;
    }

    if (!sqlDb.commit()) {
        qDebug() << "Erreur commit deleteEtudiant:" << sqlDb.lastError().text();
        sqlDb.rollback();
        return false;
    }

    return true;
}

bool adminManager::getComptesUtilisateurs(vector<compteUtilisateurData> &comptes) const
{
    comptes.clear();

    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    // la colonne role est calculee a la volee avec CASE selon les liaisons trouvees
    if (!query.exec(
            "SELECT p.id, p.nom, p.prenom, COALESCE(p.username, '') AS username, p.sexe, p.ville_id, "
            "COALESCE(v.nom, '') AS ville, COALESCE(p.isAdmin, FALSE) AS isAdmin, "
            "COALESCE(p.needPasswordChange, FALSE) AS needPasswordChange, "
            "CASE "
            "    WHEN COALESCE(p.isAdmin, FALSE) = TRUE THEN 'Admin' "
            "    WHEN e.id IS NOT NULL THEN 'Enseignant' "
            "    WHEN et.id IS NOT NULL THEN 'Etudiant' "
            "    ELSE 'Utilisateur' "
            "END AS role "
            "FROM personne p "
            "LEFT JOIN ville v ON v.id = p.ville_id "
            "LEFT JOIN enseignant e ON e.personne_id = p.id "
            "LEFT JOIN etudiant et ON et.personne_id = p.id "
            "ORDER BY p.nom, p.prenom"
        )) {
        qDebug() << "Erreur getComptesUtilisateurs:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        compteUtilisateurData row;
        row.id = query.value("id").toInt();
        row.nom = query.value("nom").toString();
        row.prenom = query.value("prenom").toString();
        row.username = query.value("username").toString();
        row.sexe = query.value("sexe").toString();
        row.villeId = query.value("ville_id").toInt();
        row.ville = query.value("ville").toString();
        row.isAdmin = query.value("isAdmin").toBool();
        row.needPasswordChange = query.value("needPasswordChange").toBool();
        row.role = query.value("role").toString();
        comptes.push_back(row);
    }

    return true;
}

bool adminManager::createCompteUtilisateur(const QString &nom, const QString &prenom, const QString &username,
                                           const QString &sexe, int villeId, bool isAdmin, QString &generatedPassword) const
{
    generatedPassword = passwordUtils::generateRandomStrongPassword(14);

    dbConnexion db;
    if (!db.isConnected()) {
        generatedPassword.clear();
        return false;
    }

    const bool hasSaltColumn = databaseHasColumn("personne", "password_salt");
    const QString salt = passwordUtils::generateSalt();
    const QString hash = passwordUtils::hashPassword(generatedPassword, salt);
    const QString storedPassword = hasSaltColumn ? hash : (salt + ":" + hash);

    QSqlQuery query;
    if (hasSaltColumn) {
        query.prepare(
            "INSERT INTO personne(nom, prenom, username, password, password_salt, needPasswordChange, ville_id, sexe, isAdmin) "
            "VALUES(:nom, :prenom, :username, :password, :password_salt, TRUE, :ville_id, :sexe, :isAdmin)"
        );
        query.bindValue(":password_salt", salt);
    } else {
        query.prepare(
            "INSERT INTO personne(nom, prenom, username, password, needPasswordChange, ville_id, sexe, isAdmin) "
            "VALUES(:nom, :prenom, :username, :password, TRUE, :ville_id, :sexe, :isAdmin)"
        );
    }

    query.bindValue(":nom", nom.trimmed());
    query.bindValue(":prenom", prenom.trimmed());
    query.bindValue(":username", username.trimmed());
    query.bindValue(":password", storedPassword);
    query.bindValue(":ville_id", villeId);
    query.bindValue(":sexe", sexe.trimmed());
    query.bindValue(":isAdmin", isAdmin);

    if (!query.exec()) {
        qDebug() << "Erreur createCompteUtilisateur:" << query.lastError().text();
        generatedPassword.clear();
        return false;
    }

    return true;
}

bool adminManager::updateCompteUtilisateur(int personneId, const QString &nom, const QString &prenom, const QString &username,
                                           const QString &sexe, int villeId, bool isAdmin) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "UPDATE personne "
        "SET nom = :nom, prenom = :prenom, username = :username, sexe = :sexe, ville_id = :ville_id, isAdmin = :isAdmin "
        "WHERE id = :id"
    );
    query.bindValue(":nom", nom.trimmed());
    query.bindValue(":prenom", prenom.trimmed());
    query.bindValue(":username", username.trimmed());
    query.bindValue(":sexe", sexe.trimmed());
    query.bindValue(":ville_id", villeId);
    query.bindValue(":isAdmin", isAdmin);
    query.bindValue(":id", personneId);

    if (!query.exec()) {
        qDebug() << "Erreur updateCompteUtilisateur:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool adminManager::deleteCompteUtilisateur(int personneId) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlDatabase sqlDb = QSqlDatabase::database();
    // suppression manuelle en cascade pour couvrir etudiant, enseignant et table de liaison
    if (!sqlDb.transaction()) {
        qDebug() << "Erreur ouverture transaction deleteCompteUtilisateur:" << sqlDb.lastError().text();
        return false;
    }

    QSqlQuery qCours;
    qCours.prepare(
        "DELETE FROM etudiant_cours "
        "WHERE etudiant_id IN (SELECT id FROM etudiant WHERE personne_id = :personne_id)"
    );
    qCours.bindValue(":personne_id", personneId);
    if (!qCours.exec()) {
        qDebug() << "Erreur delete cours compte:" << qCours.lastError().text();
        sqlDb.rollback();
        return false;
    }

    QSqlQuery qEtudiant;
    qEtudiant.prepare("DELETE FROM etudiant WHERE personne_id = :personne_id");
    qEtudiant.bindValue(":personne_id", personneId);
    if (!qEtudiant.exec()) {
        qDebug() << "Erreur delete etudiant compte:" << qEtudiant.lastError().text();
        sqlDb.rollback();
        return false;
    }

    QSqlQuery qEnseignant;
    qEnseignant.prepare("DELETE FROM enseignant WHERE personne_id = :personne_id");
    qEnseignant.bindValue(":personne_id", personneId);
    if (!qEnseignant.exec()) {
        qDebug() << "Erreur delete enseignant compte:" << qEnseignant.lastError().text();
        sqlDb.rollback();
        return false;
    }

    QSqlQuery qPersonne;
    qPersonne.prepare("DELETE FROM personne WHERE id = :id");
    qPersonne.bindValue(":id", personneId);
    if (!qPersonne.exec()) {
        qDebug() << "Erreur delete personne compte:" << qPersonne.lastError().text();
        sqlDb.rollback();
        return false;
    }

    if (!sqlDb.commit()) {
        qDebug() << "Erreur commit deleteCompteUtilisateur:" << sqlDb.lastError().text();
        sqlDb.rollback();
        return false;
    }

    return true;
}

bool adminManager::resetPasswordUtilisateur(int personneId, QString &temporaryPassword) const
{
    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    temporaryPassword = passwordUtils::generateRandomStrongPassword(14);

    // on recalcule toujours un nouveau hash + salt et on force un changement au prochain login
    const bool hasSaltColumn = databaseHasColumn("personne", "password_salt");
    const QString salt = passwordUtils::generateSalt();
    const QString hash = passwordUtils::hashPassword(temporaryPassword, salt);
    const QString storedPassword = hasSaltColumn ? hash : (salt + ":" + hash);

    QSqlQuery query;
    if (hasSaltColumn) {
        query.prepare(
            "UPDATE personne "
            "SET password = :password, password_salt = :password_salt, needPasswordChange = TRUE "
            "WHERE id = :id"
        );
        query.bindValue(":password_salt", salt);
    } else {
        query.prepare(
            "UPDATE personne "
            "SET password = :password, needPasswordChange = TRUE "
            "WHERE id = :id"
        );
    }

    query.bindValue(":password", storedPassword);
    query.bindValue(":id", personneId);

    if (!query.exec()) {
        qDebug() << "Erreur resetPasswordUtilisateur:" << query.lastError().text();
        temporaryPassword.clear();
        return false;
    }

    if (query.numRowsAffected() <= 0) {
        temporaryPassword.clear();
        return false;
    }

    return true;
}
