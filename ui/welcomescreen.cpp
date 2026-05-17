#include "welcomescreen.h"
#include "formationpage.h"
#include "loginpage.h"
#include "adminpage.h"
#include "etudiantspage.h"
#include "session.h"
#include "enseignant.h"
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QtWidgets>

void WelcomeScreen::updateAuthUi() {
    session *currentSession = session::getSession();
    bool canShowAdmin = false;
    bool canShowEtudiants = false;

    // on pilote l'etat des boutons selon le role de la session courante
    if (currentSession->isConnected() && currentSession->getUser() != nullptr) {
        const QString nom = QString::fromStdString(currentSession->getUser()->getNom());
        labelSessionInfo->setText("Connecte : " + nom);
        btnLogin->setText("Se deconnecter");

        canShowAdmin = currentSession->currentUserIsAdmin();
        if (!canShowAdmin) {
            // seul un enseignant connecte voit le bouton etudiants
            canShowEtudiants = (dynamic_cast<enseignant*>(currentSession->getUser()) != nullptr);
        }
    } else {
        labelSessionInfo->setText("Non connecte");
        btnLogin->setText("Se connecter");
    }

    btnAdmin->setVisible(canShowAdmin);
    btnEtudiants->setVisible(canShowEtudiants);
}

void WelcomeScreen::authButtonClicked() {
    session *currentSession = session::getSession();

    // le meme bouton sert de toggle connexion/deconnexion
    if (currentSession->isConnected()) {
        currentSession->disconnectSession();
        updateAuthUi();
        return;
    }

    showLoginClicked();
}

void WelcomeScreen::openFormationPage(){
    formationPage* page = new formationPage();
    page->resize(this->size());
    page->move(this->pos());

    if (this->isMaximized()) {
        page->showMaximized();
    } else {
        page->show();
    }
    page->raise();
    page->activateWindow();
    this->hide();

    // on differe le chargement pour afficher la fenetre tout de suite puis remplir apres
    QTimer::singleShot(0, page, [page]() {
        page->loadFormations();
    });

    connect(page, &formationPage::retourDemande, [this, page]() {
        if (page->isMaximized()) {
            this->showMaximized();
        } else {
            this->setGeometry(page->geometry());
            this->show();
        }
        this->raise();
        this->activateWindow();
        page->hide();
        // deleteLater evite de supprimer la page en plein traitement d'evenement Qt
        page->deleteLater();
    });
}

void WelcomeScreen::openAdminPage() {
    adminPage* page = new adminPage();
    page->resize(this->size());
    page->move(this->pos());

    connect(page, &adminPage::retourDemande, [this, page]() {
        if (page->isMaximized()) {
            this->showMaximized();
        } else {
            this->setGeometry(page->geometry());
            this->show();
        }
        updateAuthUi();
        this->raise();
        this->activateWindow();
        page->hide();
        page->deleteLater();
    });

    if (this->isMaximized()) {
        page->showMaximized();
    } else {
        page->show();
    }
    page->raise();
    page->activateWindow();
    this->hide();

    QTimer::singleShot(0, page, [page]() {
        page->loadAll();
    });
}

void WelcomeScreen::openEtudiantsPage() {
    etudiantsPage *page = new etudiantsPage();
    page->resize(this->size());
    page->move(this->pos());

    connect(page, &etudiantsPage::retourDemande, [this, page]() {
        if (page->isMaximized()) {
            this->showMaximized();
        } else {
            this->setGeometry(page->geometry());
            this->show();
        }
        updateAuthUi();
        this->raise();
        this->activateWindow();
        page->hide();
        page->deleteLater();
    });

    if (this->isMaximized()) {
        page->showMaximized();
    } else {
        page->show();
    }
    page->raise();
    page->activateWindow();
    this->hide();

    QTimer::singleShot(0, page, [page]() {
        page->loadEtudiants();
    });
}

void WelcomeScreen::seeFormationClicked(){
    openFormationPage();
}

void WelcomeScreen::showAdminClicked() {
    session *currentSession = session::getSession();
    if (!currentSession->isConnected() || !currentSession->currentUserIsAdmin()) {
        return;
    }

    openAdminPage();
}

void WelcomeScreen::showEtudiantsClicked() {
    session *currentSession = session::getSession();
    if (!currentSession->isConnected() || currentSession->currentUserIsAdmin()) {
        return;
    }

    // garde-fou supplementaire au cas ou le role ne correspond pas a un objet enseignant
    if (dynamic_cast<enseignant*>(currentSession->getUser()) == nullptr) {
        return;
    }

    openEtudiantsPage();
}

void WelcomeScreen::showLoginClicked(){
    loginPage* page = new loginPage();
    page->resize(this->size());
    page->move(this->pos());

    connect(page, &loginPage::retourDemande, [this, page]() {
        if (page->isMaximized()) {
            this->showMaximized();
        } else {
            this->setGeometry(page->geometry());
            this->show();
        }
        this->raise();
        this->activateWindow();
        page->hide();
        page->deleteLater();
    });

    // si login ok on revient ici puis on recalcule les boutons selon le nouveau role
    connect(page, &loginPage::connexionReussie, [this, page]() {
        if (page->isMaximized()) {
            this->showMaximized();
        } else {
            this->setGeometry(page->geometry());
            this->show();
        }
        updateAuthUi();
        this->raise();
        this->activateWindow();
        page->hide();
        page->deleteLater();
    });

    if (this->isMaximized()) {
        page->showMaximized();
    } else {
        page->show();
    }
    page->raise();
    page->activateWindow();
    this->hide();
}

WelcomeScreen::WelcomeScreen(QWidget *parent) : QWidget(parent) {
    // Configuration de la fenêtre
    this->setWindowTitle("Lycée Franklin Roosevelt");
    this->setMinimumSize(1000, 600);

    // 1. BARRE DU HAUT (Pour le bouton se connecter)
    QHBoxLayout *topLayout = new QHBoxLayout();
    labelSessionInfo = new QLabel(this);
    labelSessionInfo->setStyleSheet("color: white; font-size: 15px; font-weight: bold;");
    topLayout->addWidget(labelSessionInfo);

    topLayout->addStretch(); // Pousse le bouton vers la droite
    btnLogin = new QPushButton("Se connecter");
    btnLogin->setMinimumSize(150, 40);
    btnLogin->setStyleSheet(
        "QPushButton { "
        "background-color: rgba(255, 255, 255, 200); "
        "border-radius: 5px; font-weight: bold; color: #003366; "
        "} "
        "QPushButton:hover { background-color: white; }"
    );
    connect(btnLogin, &QPushButton::clicked, this, &WelcomeScreen::authButtonClicked);
    topLayout->addWidget(btnLogin);

    // 2. CONTENU CENTRAL (Texte à gauche)
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setContentsMargins(50, 0, 50, 0);
    leftLayout->setSpacing(15);

    labelTitle = new QLabel("Bâtir l'avenir par la\nTradition.");
    labelTitle->setStyleSheet("color: white; font-size: 64px; font-weight: bold; font-family: 'Segoe UI', Arial;");
    labelTitle->setWordWrap(true);

    labelDesc = new QLabel("Le Lycée Franklin Roosevelt forme les leaders de\ndemain dans un cadre historique alliant rigueur\nacadémique et innovation pédagogique.");
    labelDesc->setStyleSheet("color: #E0E0E0; font-size: 18px; line-height: 1.5;");
    labelDesc->setWordWrap(true);

    btnDiscover = new QPushButton("Découvrir la formation  →");
    btnDiscover->setMinimumSize(220, 50);
    btnDiscover->setMaximumWidth(420);
    btnDiscover->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnDiscover->setStyleSheet(
        "QPushButton { "
        "background-color: #004A8D; color: white; border-radius: 4px; "
        "font-size: 16px; font-weight: bold; text-align: left; padding-left: 20px; "
        "} "
        "QPushButton:hover { background-color: #005bb7; }"
    );
    connect(btnDiscover, &QPushButton::clicked, this, &WelcomeScreen::seeFormationClicked);

    leftLayout->addStretch(); // Espace avant le texte
    leftLayout->addWidget(labelTitle);
    leftLayout->addWidget(labelDesc);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(btnDiscover, 0, Qt::AlignLeft);
    leftLayout->addStretch(); // Espace après le texte

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();

    btnAdmin = new QPushButton("Panel Admin", this);
    btnAdmin->setMinimumSize(180, 44);
    btnAdmin->setStyleSheet(
        "QPushButton { "
        "background-color: rgba(30, 30, 30, 210); color: white; border-radius: 6px; "
        "font-size: 14px; font-weight: bold; padding: 8px 14px; "
        "} "
        "QPushButton:hover { background-color: rgba(45, 45, 45, 230); }"
    );
    btnAdmin->setVisible(false);
    connect(btnAdmin, &QPushButton::clicked, this, &WelcomeScreen::showAdminClicked);
    btnEtudiants = new QPushButton("Voir les etudiants", this);
    btnEtudiants->setMinimumSize(180, 44);
    btnEtudiants->setStyleSheet(
        "QPushButton { "
        "background-color: rgba(20, 70, 40, 210); color: white; border-radius: 6px; "
        "font-size: 14px; font-weight: bold; padding: 8px 14px; "
        "} "
        "QPushButton:hover { background-color: rgba(30, 90, 55, 230); }"
    );
    btnEtudiants->setVisible(false);
    connect(btnEtudiants, &QPushButton::clicked, this, &WelcomeScreen::showEtudiantsClicked);

    bottomLayout->addWidget(btnAdmin);
    bottomLayout->addWidget(btnEtudiants);

    // 3. LAYOUT PRINCIPAL
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(bottomLayout);

    updateAuthUi();
    updateResponsiveUi();
}

void WelcomeScreen::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateResponsiveUi();
}

void WelcomeScreen::updateResponsiveUi() {
    // on recalcule tailles et styles a chaque resize pour garder un rendu propre sur grand ecran
    const double scaleW = static_cast<double>(width()) / 1000.0;
    const double scaleH = static_cast<double>(height()) / 600.0;
    const double scale = qBound(1.0, qMin(scaleW, scaleH), 1.75);

    const int titleFont = qRound(64 * scale);
    const int descFont = qRound(18 * scale);
    const int sessionFont = qRound(15 * scale);
    const int loginFont = qRound(15 * scale);
    const int discoverFont = qRound(17 * scale);
    const int actionFont = qRound(15 * scale);

    labelSessionInfo->setStyleSheet(QString("color: white; font-size: %1px; font-weight: bold;").arg(sessionFont));
    labelTitle->setStyleSheet(
        QString("color: white; font-size: %1px; font-weight: bold; font-family: 'Segoe UI', Arial;").arg(titleFont)
    );
    labelDesc->setStyleSheet(QString("color: #E0E0E0; font-size: %1px; line-height: 1.4;").arg(descFont));

    btnLogin->setStyleSheet(
        QString(
            "QPushButton { "
            "background-color: rgba(255, 255, 255, 200); "
            "border-radius: 5px; font-weight: bold; color: #003366; font-size: %1px; "
            "} "
            "QPushButton:hover { background-color: white; }"
        ).arg(loginFont)
    );

    btnDiscover->setStyleSheet(
        QString(
            "QPushButton { "
            "background-color: #004A8D; color: white; border-radius: 4px; "
            "font-size: %1px; font-weight: bold; text-align: left; padding-left: 20px; "
            "} "
            "QPushButton:hover { background-color: #005bb7; }"
        ).arg(discoverFont)
    );

    btnAdmin->setStyleSheet(
        QString(
            "QPushButton { "
            "background-color: rgba(30, 30, 30, 210); color: white; border-radius: 6px; "
            "font-size: %1px; font-weight: bold; padding: 8px 14px; "
            "} "
            "QPushButton:hover { background-color: rgba(45, 45, 45, 230); }"
        ).arg(actionFont)
    );

    btnEtudiants->setStyleSheet(
        QString(
            "QPushButton { "
            "background-color: rgba(20, 70, 40, 210); color: white; border-radius: 6px; "
            "font-size: %1px; font-weight: bold; padding: 8px 14px; "
            "} "
            "QPushButton:hover { background-color: rgba(30, 90, 55, 230); }"
        ).arg(actionFont)
    );

    const int loginW = qRound(150 * qBound(1.0, scale, 1.45));
    const int loginH = qRound(40 * qBound(1.0, scale, 1.45));
    btnLogin->setMinimumSize(loginW, loginH);

    const int discoverW = qRound(220 * qBound(1.0, scale, 1.5));
    const int discoverH = qRound(50 * qBound(1.0, scale, 1.5));
    btnDiscover->setMinimumSize(discoverW, discoverH);
    btnDiscover->setMaximumWidth(qRound(420 * qBound(1.0, scale, 1.3)));

    const int actionW = qRound(180 * qBound(1.0, scale, 1.45));
    const int actionH = qRound(44 * qBound(1.0, scale, 1.45));
    btnAdmin->setMinimumSize(actionW, actionH);
    btnEtudiants->setMinimumSize(actionW, actionH);
}

void WelcomeScreen::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    // Chargement de l'image
    QPixmap bg("./images/lycee.png");
    if (!bg.isNull()) {
        // On dessine l'image pour qu'elle remplisse tout le widget
        painter.drawPixmap(0, 0, this->width(), this->height(), bg);
    }

    // on superpose un degrade sombre pour garder la lisibilite du texte sur la photo
    QLinearGradient gradient(0, 0, this->width() * 0.6, 0);
    gradient.setColorAt(0, QColor(0, 0, 0, 220)); // Noir foncé transparent
    gradient.setColorAt(1, QColor(0, 0, 0, 0));      // Transparent
    painter.fillRect(this->rect(), gradient);
}
