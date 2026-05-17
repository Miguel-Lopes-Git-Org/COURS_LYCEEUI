#include "session.h"

session* session::m_instance = nullptr;

session* session::getSession() {
    // singleton cree a la demande pour garder une session globale unique
    if (m_instance == nullptr) {
        m_instance = new session();
    }

    return m_instance;
}

void session::setSessionTeacher(int id, string nom, string prenom, string sexe, ville* villeUser, string matiere, cours* coursUser, float salaire, bool needPasswordChange) {
    // on instancie le type metier reel pour acceder ensuite aux infos enseignant dans l'ui
    m_currentUser = new enseignant(nom, prenom, sexe, salaire, matiere, villeUser, coursUser);
    m_currentUser->setIdAdmin(false);
    m_currentUserId = id;
    m_needPasswordChange = needPasswordChange;
    m_currentRole = "Enseignant";
}

void session::setSessionStudent(int id, string nom, string prenom, string sexe, string numCarte, ville* villeUser, vector<cours*> coursUser, bool needPasswordChange) {
    m_currentUser = new etudiant(nom, prenom, numCarte, sexe, villeUser, coursUser);
    m_currentUser->setIdAdmin(false);
    m_currentUserId = id;
    m_needPasswordChange = needPasswordChange;
    m_currentRole = "Etudiant";
}

void session::setSessionAdmin(int id, string nom, string prenom, string sexe, ville* villeUser, bool needPasswordChange, bool isAdmin) {
    // pour admin/utilisateur simple on garde un objet personne sans specialisation
    m_currentUser = new personne(nom, prenom, sexe, villeUser, isAdmin);
    m_currentUserId = id;
    m_needPasswordChange = needPasswordChange;
    m_currentRole = isAdmin ? "Admin" : "Utilisateur";
}

void session::disconnectSession() {
    // reset complet de la session pour repartir sur un etat neutre
    delete m_currentUser;
    m_currentUser = nullptr;
    m_currentUserId = -1;
    m_needPasswordChange = false;
    m_currentRole = "";
}

bool session::isConnected() {
    return m_currentUser != nullptr;
}

personne* session::getUser() const {
    return m_currentUser;
}

int session::getCurrentUserId() const {
    return m_currentUserId;
}

bool session::needsPasswordChange() const {
    return m_needPasswordChange;
}

void session::setNeedPasswordChange(bool needPasswordChange) {
    m_needPasswordChange = needPasswordChange;
}

bool session::currentUserIsAdmin() const {
    return (m_currentUser != nullptr && m_currentUser->getIdAdmin()) || m_currentRole == "Admin";
}

string session::getCurrentRole() const {
    return m_currentRole;
}
