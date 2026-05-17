#include "scolariteManager.h"
#include "dbConnexion.h"

#include <QDebug>

scolariteManager::scolariteManager() {}

vector<formation*> scolariteManager::getAllForamtions() const{
    // Ouvrir la connexion à la bdd
    dbConnexion db;
    if (!db.isConnected()) {
        return vector<formation*>();
    }

    // Executer la requete pour les formations
    QSqlQuery query;

    if (!query.exec("SELECT matiere, nb_heures_semestre_1, nb_heures_semestre_2, nb_heures_semestres_3_et_4 FROM getFormations")) {
        qDebug() << "Erreur requete getFormations:" << query.lastError().text();
        return vector<formation*>();
    }

    // Création du tableau avec les instances de formation
    vector<formation*> formationTab;

    while(query.next()) {
        QString matiere = query.value(0).toString();
        QString nbHeureS1 = query.value(1).toString();
        QString nbHeureS2 = query.value(2).toString();
        QString nbHeureS3et4 = query.value(3).toString();

        if (!matiere.isEmpty()) {
            formationTab.push_back(new formation(matiere, nbHeureS1, nbHeureS2, nbHeureS3et4));
        }
    }

    // Retourner le vecteur avec les instances de la classe formation
    return formationTab;
}

bool scolariteManager::getAllEnseignants(vector<enseignantData> &enseignants) const {
    enseignants.clear();

    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;
    if (!query.exec("SELECT nom, prenom, matiere FROM getEnseignants")) {
        qDebug() << "Erreur requete getEnseignants:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        enseignantData row;
        row.nom = query.value(0).toString();
        row.prenom = query.value(1).toString();
        row.matiere = query.value(2).toString();
        row.ville = "-";
        row.salaire = "-";
        enseignants.push_back(row);
    }

    return true;
}

bool scolariteManager::getAllEtudiantsDetails(vector<etudiantDetailsData> &etudiants) const {
    etudiants.clear();

    dbConnexion db;
    if (!db.isConnected()) {
        return false;
    }

    QSqlQuery query;

    // requete agregee avec string_agg pour sortir tous les cours d'un etudiant sur une seule ligne
    if (!query.exec(
            "SELECT et.id AS etudiant_id, p.nom, p.prenom, "
            "COALESCE(et.numCarte::text, '') AS numCarte, "
            "COALESCE(v.nom, '-') AS ville, "
            "COALESCE(et.photoUrl, '') AS photoUrl, "
            "COALESCE(string_agg(c.nom, ', ' ORDER BY c.nom), 'Aucun cours') AS cours "
            "FROM etudiant et "
            "JOIN personne p ON p.id = et.personne_id "
            "LEFT JOIN ville v ON v.id = p.ville_id "
            "LEFT JOIN etudiant_cours ec ON ec.etudiant_id = et.id "
            "LEFT JOIN cours c ON c.id = ec.cours_id "
            "GROUP BY et.id, p.nom, p.prenom, et.numCarte, v.nom, et.photoUrl "
            "ORDER BY p.nom, p.prenom"
        )) {
        qDebug() << "Erreur requete getAllEtudiantsDetails:" << query.lastError().text();
        return false;
    }

    // on mappe les alias SQL vers le struct utilise par l'UI
    while (query.next()) {
        etudiantDetailsData row;
        row.etudiantId = query.value("etudiant_id").toInt();
        row.nom = query.value("nom").toString();
        row.prenom = query.value("prenom").toString();
        row.numCarte = query.value("numCarte").toString();
        row.ville = query.value("ville").toString();
        row.photoUrl = query.value("photoUrl").toString();
        row.cours = query.value("cours").toString();
        etudiants.push_back(row);
    }

    return true;
}
