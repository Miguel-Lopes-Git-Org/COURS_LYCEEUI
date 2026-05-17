#include "passwordchangedialog.h"

#include "passwordutils.h"
#include "sessionmanager.h"

#include <QtWidgets>

passwordChangeDialog::passwordChangeDialog(int userId, QWidget *parent)
    : QDialog(parent), m_userId(userId)
{
    setWindowTitle("Changement de mot de passe obligatoire");
    setModal(true);
    setMinimumWidth(500);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Vous devez changer votre mot de passe", this);
    title->setStyleSheet("font-size: 18px; font-weight: bold;");

    QLabel *description = new QLabel(
        "Votre compte est marque comme premiere connexion."
        "\nLe nouveau mot de passe doit contenir 12 caracteres minimum,"
        "\navec majuscule, minuscule, chiffre et caractere special.",
        this
    );

    inputPassword = new QLineEdit(this);
    inputPassword->setPlaceholderText("Nouveau mot de passe");
    inputPassword->setEchoMode(QLineEdit::Password);

    inputPasswordConfirm = new QLineEdit(this);
    inputPasswordConfirm->setPlaceholderText("Confirmer le mot de passe");
    inputPasswordConfirm->setEchoMode(QLineEdit::Password);

    messageLabel = new QLabel(this);
    messageLabel->setStyleSheet("color: #b33939;");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    btnCancel = new QPushButton("Annuler", this);
    btnSave = new QPushButton("Enregistrer", this);
    btnSave->setStyleSheet(
        "QPushButton { background-color: #004A8D; color: white; font-weight: bold; padding: 8px 16px; }"
        "QPushButton:hover { background-color: #005bb7; }"
    );

    buttonLayout->addWidget(btnCancel);
    buttonLayout->addWidget(btnSave);

    mainLayout->addWidget(title);
    mainLayout->addWidget(description);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(inputPassword);
    mainLayout->addWidget(inputPasswordConfirm);
    mainLayout->addWidget(messageLabel);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonLayout);

    connect(btnSave, &QPushButton::clicked, this, &passwordChangeDialog::handleSave);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void passwordChangeDialog::handleSave()
{
    const QString password = inputPassword->text();
    const QString confirm = inputPasswordConfirm->text();

    if (password.isEmpty() || confirm.isEmpty()) {
        messageLabel->setText("Veuillez remplir les deux champs.");
        return;
    }

    if (password != confirm) {
        messageLabel->setText("Les mots de passe ne correspondent pas.");
        return;
    }

    QString errorMessage;

    // meme regle de robustesse que partout ailleurs pour centraliser la politique mdp
    if (!passwordUtils::isStrongPassword(password, &errorMessage)) {
        messageLabel->setText(errorMessage);
        return;
    }

    sessionManager manager;

    // updatePassword gere hash + salt puis retire le flag needPasswordChange
    if (!manager.updatePassword(m_userId, password.toStdString())) {
        messageLabel->setText("Impossible de mettre a jour le mot de passe.");
        return;
    }

    accept();
}
