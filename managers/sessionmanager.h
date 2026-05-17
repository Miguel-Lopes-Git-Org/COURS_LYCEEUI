#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "session.h"
#include <string>

using namespace std;

class sessionManager
{
public:
    sessionManager();
    bool login(string username, string password) const;
    bool updatePassword(int userId, string newPassword) const;
};

#endif // SESSIONMANAGER_H
