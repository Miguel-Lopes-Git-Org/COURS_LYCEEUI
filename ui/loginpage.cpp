#include "loginpage.h"
#include "sessionmanager.h"
#include "session.h"
#include "passwordchangedialog.h"

#include <QtWidgets>

loginPage::loginPage(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("Lycée Franklin Roosevelt | Connexion");
    this->setMinimumSize(1000, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topLayout = new QHBoxLayout();
    btnRetour = new QPushButton("⬅ Retour", this);
    topLayout->addWidget(btnRetour);
    topLayout->addStretch();

    titleLabel = new QLabel("Connexion", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #1f2d3d;");

    inputUsername = new QLineEdit(this);
    inputUsername->setPlaceholderText("Nom d'utilisateur");
    inputUsername->setMinimumHeight(42);

    inputPassword = new QLineEdit(this);
    inputPassword->setPlaceholderText("Mot de passe");
    inputPassword->setEchoMode(QLineEdit::Password);
    inputPassword->setMinimumHeight(42);

    btnConnect = new QPushButton("Se connecter", this);
    btnConnect->setMinimumHeight(44);
    btnConnect->setStyleSheet(
        "QPushButton {"
        "background-color: #004A8D;"
        "color: white;"
        "font-weight: bold;"
        "border-radius: 6px;"
        "padding: 8px 16px;"
        "}"
        "QPushButton:hover { background-color: #005bb7; }"
    );

    messageLabel = new QLabel(this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("font-size: 14px; color: #b33939;");
    messageLabel->setMinimumHeight(24);

    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->setSpacing(12);
    formLayout->addWidget(titleLabel);
    formLayout->addSpacing(12);
    formLayout->addWidget(inputUsername);
    formLayout->addWidget(inputPassword);
    formLayout->addWidget(btnConnect);
    formLayout->addWidget(messageLabel);

    formContainer = new QWidget(this);
    formContainer->setLayout(formLayout);
    formContainer->setMaximumWidth(420);

    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addStretch();
    centerLayout->addWidget(formContainer);
    centerLayout->addStretch();

    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch();

    // on branche les signaux Qt pour que les actions UI déclenchent les bonnes méthodes
    connect(btnRetour, &QPushButton::clicked, this, &loginPage::retourDemande);
    connect(btnConnect, &QPushButton::clicked, this, &loginPage::tryLogin);
    connect(inputPassword, &QLineEdit::returnPressed, this, &loginPage::tryLogin);

    updateResponsiveUi();
}

loginPage::~loginPage() {}

void loginPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateResponsiveUi();
}

void loginPage::updateResponsiveUi()
{
    // on calcule un ratio basé sur la taille de référence 1000x600
    const double scaleW = static_cast<double>(width()) / 1000.0;
    const double scaleH = static_cast<double>(height()) / 600.0;

    // qBound évite que le zoom soit trop petit ou trop grand
    const double scale = qBound(1.0, qMin(scaleW, scaleH), 1.8);

    QFont pageFont = this->font();
    pageFont.setPointSize(qRound(11 * scale));
    this->setFont(pageFont);

    // on adapte tous les éléments importants en gardant des proportions cohérentes
    titleLabel->setStyleSheet(QString("font-size: %1px; font-weight: bold; color: #1f2d3d;").arg(qRound(32 * scale)));
    inputUsername->setMinimumHeight(qRound(44 * scale));
    inputPassword->setMinimumHeight(qRound(44 * scale));
    btnConnect->setMinimumHeight(qRound(46 * scale));
    formContainer->setMaximumWidth(qRound(440 * qBound(1.0, scale, 1.4)));
}

void loginPage::tryLogin()
{
    // trimmed enlève les espaces en début/fin pour éviter les faux échecs sur le login
    const QString username = inputUsername->text().trimmed();
    const QString password = inputPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        messageLabel->setStyleSheet("font-size: 14px; color: #b33939;");
        messageLabel->setText("Veuillez remplir tous les champs.");
        return;
    }

    sessionManager manager;

    // conversion QString -> std::string car le manager backend attend du std::string
    const bool ok = manager.login(username.toStdString(), password.toStdString());

    if (!ok) {
        messageLabel->setStyleSheet("font-size: 14px; color: #b33939;");
        messageLabel->setText("Identifiants invalides.");
        return;
    }

    session *currentSession = session::getSession();

    // si c'est un premier mot de passe, on bloque la suite tant que l'utilisateur ne l'a pas changé
    if (currentSession->needsPasswordChange()) {
        passwordChangeDialog dialog(currentSession->getCurrentUserId(), this);
        const int result = dialog.exec();

        // on coupe la session si la popup est fermée/refusée pour forcer le changement de mdp
        if (result != QDialog::Accepted) {
            currentSession->disconnectSession();
            messageLabel->setStyleSheet("font-size: 14px; color: #b33939;");
            messageLabel->setText("Changement de mot de passe obligatoire.");
            return;
        }
    }

    // ici on notifie l'UI par un signal pour laisser la fenêtre parente gérer la navigation
    messageLabel->setStyleSheet("font-size: 14px; color: #218c5f;");
    messageLabel->setText("Connexion réussie.");
    emit connexionReussie();
}
