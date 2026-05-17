#include "passwordutils.h"

#include <QCryptographicHash>
#include <QRandomGenerator>

bool passwordUtils::isStrongPassword(const QString &password, QString *errorMessage)
{
    if (password.length() < 12) {
        if (errorMessage) *errorMessage = "Le mot de passe doit contenir au moins 12 caracteres.";
        return false;
    }

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    // on parcourt caractere par caractere pour valider chaque categorie demandee
    for (const QChar c : password) {
        if (c.isUpper()) hasUpper = true;
        else if (c.isLower()) hasLower = true;
        else if (c.isDigit()) hasDigit = true;
        else hasSpecial = true;
    }

    if (!hasUpper || !hasLower || !hasDigit || !hasSpecial) {
        if (errorMessage) *errorMessage = "Le mot de passe doit contenir majuscule, minuscule, chiffre et caractere special.";
        return false;
    }

    if (errorMessage) *errorMessage = "";
    return true;
}

QString passwordUtils::generateSalt(int length)
{
    static const QString chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString salt;
    salt.reserve(length);

    // bounded evite de sortir des bornes de la chaine chars
    for (int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->bounded(chars.length());
        salt.append(chars.at(index));
    }

    return salt;
}

QString passwordUtils::hashPassword(const QString &password, const QString &salt)
{
    // on concatene password + salt avant hash pour eviter les mots de passe identiques en base
    const QByteArray input = (password + salt).toUtf8();
    return QCryptographicHash::hash(input, QCryptographicHash::Sha256).toHex();
}

QString passwordUtils::generateRandomStrongPassword(int length)
{
    static const QString upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static const QString lower = "abcdefghijklmnopqrstuvwxyz";
    static const QString digits = "0123456789";
    static const QString special = "!@#$%^&*()-_=+[]{};:,.?";
    static const QString all = upper + lower + digits + special;

    if (length < 12) {
        length = 12;
    }

    QString password;
    password.reserve(length);

    // on force au moins un char de chaque type avant de completer
    password.append(upper.at(QRandomGenerator::global()->bounded(upper.length())));
    password.append(lower.at(QRandomGenerator::global()->bounded(lower.length())));
    password.append(digits.at(QRandomGenerator::global()->bounded(digits.length())));
    password.append(special.at(QRandomGenerator::global()->bounded(special.length())));

    for (int i = 4; i < length; ++i) {
        password.append(all.at(QRandomGenerator::global()->bounded(all.length())));
    }

    // melange de type fisher-yates pour ne pas laisser les 4 premiers caracteres predictibles
    for (int i = password.length() - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        const QChar tmp = password.at(i);
        password[i] = password.at(j);
        password[j] = tmp;
    }

    return password;
}
