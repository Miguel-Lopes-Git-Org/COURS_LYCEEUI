#include "enseignantpage.h"
#include "scolariteManager.h"
#include "enseignant.h"
#include "session.h"

#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

enseignantPage::enseignantPage(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("Lycée Franklin Roosevelt | Enseignants");
    this->setMinimumSize(1000, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topLayout = new QHBoxLayout();
    btnRetour = new QPushButton("⬅ Retour", this);
    topLayout->addWidget(btnRetour);
    topLayout->addStretch();

    titleLabel = new QLabel("Liste des Enseignants", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels(QStringList() << "Nom" << "Prenom" << "Matiere" << "Ville" << "Salaire");
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    emptyLabel = new QLabel(this);
    emptyLabel->setAlignment(Qt::AlignCenter);
    emptyLabel->setStyleSheet("font-size: 15px; color: #57606f;");
    emptyLabel->hide();

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(table);
    mainLayout->addWidget(emptyLabel);

    connect(btnRetour, &QPushButton::clicked, this, &enseignantPage::retourDemande);

    updateResponsiveUi();
}

enseignantPage::~enseignantPage() {}

void enseignantPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateResponsiveUi();
}

void enseignantPage::updateResponsiveUi()
{
    const double scaleW = static_cast<double>(width()) / 1000.0;
    const double scaleH = static_cast<double>(height()) / 600.0;
    const double scale = qBound(1.0, qMin(scaleW, scaleH), 1.8);

    QFont pageFont = this->font();
    pageFont.setPointSize(qRound(11 * scale));
    this->setFont(pageFont);

    titleLabel->setStyleSheet(QString("font-size: %1px; font-weight: bold;").arg(qRound(24 * scale)));
    btnRetour->setMinimumHeight(qRound(36 * scale));
    table->verticalHeader()->setDefaultSectionSize(qRound(34 * scale));
}

void enseignantPage::loadEnseignants()
{
    table->setRowCount(0);
    table->show();
    emptyLabel->hide();

    scolariteManager manager;
    vector<enseignantData> rows;
    if (!manager.getAllEnseignants(rows)) {
        table->hide();
        emptyLabel->setText("Impossible de charger les enseignants (connexion base de donnees). ");
        emptyLabel->show();
        return;
    }

    if (rows.empty()) {
        table->hide();
        emptyLabel->setText("Aucun enseignant trouve.");
        emptyLabel->show();
        return;
    }

    session *currentSession = session::getSession();
    enseignant *connectedTeacher = nullptr;
    if (currentSession->isConnected()) {
        // dynamic_cast securise le cast selon le type reel de l'objet personne
        connectedTeacher = dynamic_cast<enseignant*>(currentSession->getUser());
    }

    if (connectedTeacher != nullptr) {
        const QString detailNom = QString::fromStdString(connectedTeacher->getNom());
        const QString detailPrenom = QString::fromStdString(connectedTeacher->getPrenom());

        QString detailVille = "-";
        if (connectedTeacher->getVillePersonne() != nullptr) {
            const QString villeNom = QString::fromStdString(connectedTeacher->getVillePersonne()->getNom());
            if (!villeNom.isEmpty()) {
                detailVille = villeNom;
            }
        }

        const QString detailSalaire = QString::number(connectedTeacher->getSalaire(), 'f', 2);

        // on remplace les infos manquantes de la liste avec les details de la session courante
        for (size_t i = 0; i < rows.size(); ++i) {
            if (rows[i].nom == detailNom && rows[i].prenom == detailPrenom) {
                rows[i].ville = detailVille;
                rows[i].salaire = detailSalaire;
                break;
            }
        }
    }

    table->setRowCount(static_cast<int>(rows.size()));

    for (size_t i = 0; i < rows.size(); ++i) {
        const int rowIndex = static_cast<int>(i);
        table->setItem(rowIndex, 0, new QTableWidgetItem(rows[i].nom));
        table->setItem(rowIndex, 1, new QTableWidgetItem(rows[i].prenom));
        table->setItem(rowIndex, 2, new QTableWidgetItem(rows[i].matiere));
        table->setItem(rowIndex, 3, new QTableWidgetItem(rows[i].ville));
        table->setItem(rowIndex, 4, new QTableWidgetItem(rows[i].salaire));
    }

    table->resizeRowsToContents();
}
