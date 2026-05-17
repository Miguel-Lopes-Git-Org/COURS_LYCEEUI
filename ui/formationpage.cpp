#include "formationpage.h"
#include "scolariteManager.h"
#include "formationwidget.h"
#include "enseignantpage.h"

#include <QTimer>

formationPage::formationPage(QWidget *parent) : QWidget(parent) {
    // Configuration de la fenêtre
    this->setWindowTitle("Lycée Franklin Roosevelt | Formation");
    this->setMinimumSize(1000, 600);

    // 1. Layout principal de toute la fenêtre
    mainLayout = new QVBoxLayout(this);

    // 2. L'en-tête (Boutons de navigation)
    QHBoxLayout *topLayout = new QHBoxLayout();

    btnRetour = new QPushButton("⬅ Retour", this);
    // On relie le clic du bouton à notre signal personnalisé
    connect(btnRetour, &QPushButton::clicked, this, &formationPage::retourDemande);
    topLayout->addWidget(btnRetour);
    topLayout->addStretch();

    btnEnseignants = new QPushButton("Les enseignants", this);
    connect(btnEnseignants, &QPushButton::clicked, this, &formationPage::showEnseignants);
    topLayout->addWidget(btnEnseignants);

    mainLayout->addLayout(topLayout);

    titleLabel = new QLabel("Liste des Formations", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    // 3. Création du ScrollArea
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true); // TRÈS IMPORTANT : permet au conteneur de s'adapter à la largeur

    // 4. Création du widget "conteneur" et de son layout
    scrollContent = new QWidget();
    listLayout = new QVBoxLayout(scrollContent);
    listLayout->setAlignment(Qt::AlignTop); // Pour que les lignes s'empilent en haut et ne s'étalent pas

    // 5. On insère le conteneur dans le ScrollArea
    scrollArea->setWidget(scrollContent);

    // 6. On insère le ScrollArea dans la fenêtre
    mainLayout->addWidget(scrollArea);

    emptyLabel = new QLabel("", this);
    emptyLabel->setAlignment(Qt::AlignCenter);
    emptyLabel->setStyleSheet("font-size: 15px; color: #57606f;");
    emptyLabel->hide();
    mainLayout->addWidget(emptyLabel);

    updateResponsiveUi();
}

formationPage::~formationPage() {}

void formationPage::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateResponsiveUi();
}

void formationPage::updateResponsiveUi() {
    const double scaleW = static_cast<double>(width()) / 1000.0;
    const double scaleH = static_cast<double>(height()) / 600.0;
    const double scale = qBound(1.0, qMin(scaleW, scaleH), 1.8);

    QFont pageFont = this->font();
    pageFont.setPointSize(qRound(11 * scale));
    this->setFont(pageFont);

    titleLabel->setStyleSheet(QString("font-size: %1px; font-weight: bold;").arg(qRound(24 * scale)));
    btnRetour->setMinimumHeight(qRound(36 * scale));
    btnEnseignants->setMinimumHeight(qRound(36 * scale));
}

void formationPage::loadFormations() {
    clearFormationList();
    emptyLabel->hide();
    scrollArea->show();

    scolariteManager manager;
    std::vector<formation*> liste = manager.getAllForamtions(); // Attention à ta faute de frappe ici dans ton manager ;)

    if (liste.empty()) {
        scrollArea->hide();
        emptyLabel->setText("Aucune formation disponible.");
        emptyLabel->show();
        return;
    }

    // On boucle pour créer et ajouter chaque ligne
    for (formation* f : liste) {
        formationWidget *ligne = new formationWidget(this);
        ligne->setFormationData(f);

        // On ajoute la ligne dans le layout du CONTENEUR (pas dans le scrollArea direct)
        listLayout->addWidget(ligne);
    }
}

void formationPage::clearFormationList() {
    QLayoutItem *item;
    while ((item = listLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

void formationPage::showEnseignants() {
    enseignantPage *page = new enseignantPage();
    page->resize(this->size());
    page->move(this->pos());

    connect(page, &enseignantPage::retourDemande, [this, page]() {
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

    if (this->isMaximized()) {
        page->showMaximized();
    } else {
        page->show();
    }
    page->raise();
    page->activateWindow();
    this->hide();

    QTimer::singleShot(0, page, [page]() {
        page->loadEnseignants();
    });
}
