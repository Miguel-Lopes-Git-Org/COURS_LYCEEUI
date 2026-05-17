#ifndef ADMINMANAGER_H
#define ADMINMANAGER_H

#include <QString>
#include <QVector>
#include <vector>

using namespace std;

struct villeAdminData {
    int id;
    QString nom;
    QString codePostal;
};

struct matiereAdminData {
    int id;
    QString nom;
    QString alias;
};

struct coursAdminData {
    int id;
    QString nom;
};

struct enseignantAdminData {
    int enseignantId;
    int personneId;
    QString nom;
    QString prenom;
    QString sexe;
    int villeId;
    QString ville;
    QString username;
    bool isAdmin;
    bool needPasswordChange;
    int matiereId;
    QString matiere;
    double salaire;
};

struct etudiantAdminData {
    int etudiantId;
    int personneId;
    int numCarte;
    QString nom;
    QString prenom;
    QString sexe;
    int villeId;
    QString ville;
    QString username;
    QString photoUrl;
    QString cours;
    bool isAdmin;
    bool needPasswordChange;
};

struct compteUtilisateurData {
    int id;
    QString nom;
    QString prenom;
    QString username;
    QString sexe;
    int villeId;
    QString ville;
    bool isAdmin;
    bool needPasswordChange;
    QString role;
};

class adminManager
{
public:
    adminManager();

    bool getVilles(vector<villeAdminData> &villes) const;
    bool createVille(const QString &nom, const QString &codePostal) const;
    bool updateVille(int id, const QString &nom, const QString &codePostal) const;
    bool deleteVille(int id) const;

    bool getMatieres(vector<matiereAdminData> &matieres) const;
    bool createMatiere(const QString &nom, const QString &alias) const;
    bool updateMatiere(int id, const QString &nom, const QString &alias) const;
    bool deleteMatiere(int id) const;

    bool getCours(vector<coursAdminData> &cours) const;
    bool createCours(const QString &nom) const;
    bool updateCours(int id, const QString &nom) const;
    bool deleteCours(int id) const;

    bool getEnseignants(vector<enseignantAdminData> &enseignants) const;
    bool createEnseignant(const QString &nom, const QString &prenom, const QString &sexe, int villeId,
                         const QString &username, int matiereId, double salaire,
                         QString &generatedPassword) const;
    bool updateEnseignant(int enseignantId, int personneId, const QString &nom, const QString &prenom,
                         const QString &sexe, int villeId, const QString &username, int matiereId, double salaire) const;
    bool deleteEnseignant(int enseignantId, int personneId) const;

    bool getEtudiants(vector<etudiantAdminData> &etudiants) const;
    bool createEtudiant(const QString &nom, const QString &prenom, const QString &sexe, int villeId,
                        const QString &username, const QString &photoUrl,
                        const QVector<int> &coursIds, QString &generatedPassword) const;
    bool updateEtudiant(int etudiantId, int personneId, const QString &nom, const QString &prenom,
                        const QString &sexe, int villeId, const QString &username,
                        const QString &photoUrl, const QVector<int> &coursIds) const;
    bool deleteEtudiant(int etudiantId, int personneId) const;
    bool getEtudiantCoursIds(int etudiantId, QVector<int> &coursIds) const;

    bool getComptesUtilisateurs(vector<compteUtilisateurData> &comptes) const;
    bool createCompteUtilisateur(const QString &nom, const QString &prenom, const QString &username,
                                 const QString &sexe, int villeId, bool isAdmin, QString &generatedPassword) const;
    bool updateCompteUtilisateur(int personneId, const QString &nom, const QString &prenom, const QString &username,
                                 const QString &sexe, int villeId, bool isAdmin) const;
    bool deleteCompteUtilisateur(int personneId) const;
    bool resetPasswordUtilisateur(int personneId, QString &temporaryPassword) const;

private:
    bool databaseHasColumn(const QString &tableName, const QString &columnName) const;
};

#endif // ADMINMANAGER_H
