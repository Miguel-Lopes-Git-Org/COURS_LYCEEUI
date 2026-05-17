#ifndef SESSION_H
#define SESSION_H

#include <string>
#include "ville.h"
#include "enseignant.h"
#include "etudiant.h"
#include "cours.h"
#include "personne.h"

using namespace std;

struct User {
    int id;
    string nom;
    string prenom;
    string sexe;
    ville* villeUser;
    bool isAdmin;
};

class session
{
public:
    static session* getSession();
    void setSessionAdmin();
    void setSessionTeacher(int id, string nom, string prenom, string sexe, ville* villeUser, string matiere, cours* coursUser, float salaire, bool needPasswordChange = false);
    void setSessionStudent(int id, string nom, string prenom, string sexe, string numCarte, ville* villeUser, vector<cours*> coursUser, bool needPasswordChange = false);
    void setSessionAdmin(int id, string nom, string prenom, string sexe, ville* villeUser, bool needPasswordChange = false, bool isAdmin = true);
    void disconnectSession();
    bool isConnected();
    personne* getUser() const;
    int getCurrentUserId() const;
    bool needsPasswordChange() const;
    void setNeedPasswordChange(bool needPasswordChange);
    bool currentUserIsAdmin() const;
    string getCurrentRole() const;
 private:
    static session* m_instance;
    personne* m_currentUser;
    int m_currentUserId;
    bool m_needPasswordChange;
    string m_currentRole;
    session() : m_currentUser(nullptr), m_currentUserId(-1), m_needPasswordChange(false), m_currentRole("") {}

};

#endif // SESSION_H
