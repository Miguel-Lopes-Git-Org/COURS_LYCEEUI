#ifndef PASSWORDUTILS_H
#define PASSWORDUTILS_H

#include <QString>

class passwordUtils
{
public:
    static bool isStrongPassword(const QString &password, QString *errorMessage = nullptr);
    static QString generateSalt(int length = 16);
    static QString hashPassword(const QString &password, const QString &salt);
    static QString generateRandomStrongPassword(int length = 14);
};

#endif // PASSWORDUTILS_H
