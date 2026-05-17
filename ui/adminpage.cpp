#include "adminpage.h"

#include "adminManager.h"
#include "filedroplineedit.h"

#include <QCheckBox>
#include <QClipboard>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QGuiApplication>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>

adminPage::adminPage(QWidget *parent)
    : QWidget(parent),
      tabs(nullptr),
      btnRetour(nullptr),
      pageTitle(nullptr),
      tableVilles(nullptr),
      tableMatieres(nullptr),
      tableCours(nullptr),
      tableEnseignants(nullptr),
      tableEtudiants(nullptr)
{
    setWindowTitle("Lycee Franklin Roosevelt | Administration");
    setMinimumSize(1000, 600);

    setupUi();
}

adminPage::~adminPage() {}

void adminPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topLayout = new QHBoxLayout();
    btnRetour = new QPushButton("⬅ Retour", this);
    pageTitle = new QLabel("Panel Administration", this);
    pageTitle->setStyleSheet("font-size: 22px; font-weight: bold;");

    topLayout->addWidget(btnRetour);
    topLayout->addStretch();
    topLayout->addWidget(pageTitle);
    topLayout->addStretch();

    tabs = new QTabWidget(this);
    tabs->addTab(buildEnseignantsTab(), "Enseignants");
    tabs->addTab(buildEtudiantsTab(), "Etudiants");
    tabs->addTab(buildVillesTab(), "Villes");
    tabs->addTab(buildCoursTab(), "Cours");
    tabs->addTab(buildMatieresTab(), "Matieres");

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(tabs);

    connect(btnRetour, &QPushButton::clicked, this, &adminPage::retourDemande);
}

void adminPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // meme logique responsive que les autres pages pour garder une UI homogene
    const double scaleW = static_cast<double>(width()) / 1000.0;
    const double scaleH = static_cast<double>(height()) / 600.0;
    const double scale = qBound(1.0, qMin(scaleW, scaleH), 1.75);

    QFont pageFont = this->font();
    pageFont.setPointSize(qRound(11 * scale));
    this->setFont(pageFont);

    if (pageTitle != nullptr) {
        pageTitle->setStyleSheet(QString("font-size: %1px; font-weight: bold;").arg(qRound(22 * scale)));
    }

    const int headerHeight = qRound(30 * scale);
    const int buttonHeight = qRound(34 * scale);
    const int rowHeight = qRound(30 * scale);

    const QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    for (QPushButton *button : buttons) {
        if (button != nullptr) {
            button->setMinimumHeight(buttonHeight);
        }
    }

    if (tableVilles) {
        tableVilles->horizontalHeader()->setFixedHeight(headerHeight);
        tableVilles->verticalHeader()->setDefaultSectionSize(rowHeight);
    }
    if (tableMatieres) {
        tableMatieres->horizontalHeader()->setFixedHeight(headerHeight);
        tableMatieres->verticalHeader()->setDefaultSectionSize(rowHeight);
    }
    if (tableCours) {
        tableCours->horizontalHeader()->setFixedHeight(headerHeight);
        tableCours->verticalHeader()->setDefaultSectionSize(rowHeight);
    }
    if (tableEnseignants) {
        tableEnseignants->horizontalHeader()->setFixedHeight(headerHeight);
        tableEnseignants->verticalHeader()->setDefaultSectionSize(rowHeight);
    }
    if (tableEtudiants) {
        tableEtudiants->horizontalHeader()->setFixedHeight(headerHeight);
        tableEtudiants->verticalHeader()->setDefaultSectionSize(rowHeight);
    }
}

QWidget *adminPage::buildVillesTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    tableVilles = new QTableWidget(tab);
    tableVilles->setColumnCount(3);
    tableVilles->setHorizontalHeaderLabels(QStringList() << "ID" << "Nom" << "Code postal");
    tableVilles->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableVilles->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableVilles->setSelectionMode(QAbstractItemView::SingleSelection);
    tableVilles->verticalHeader()->setVisible(false);
    tableVilles->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QHBoxLayout *actions = new QHBoxLayout();
    QPushButton *btnAdd = new QPushButton("Creer", tab);
    QPushButton *btnEdit = new QPushButton("Modifier", tab);
    QPushButton *btnDelete = new QPushButton("Supprimer", tab);
    actions->addWidget(btnAdd);
    actions->addWidget(btnEdit);
    actions->addWidget(btnDelete);
    actions->addStretch();

    layout->addWidget(tableVilles);
    layout->addLayout(actions);

    connect(btnAdd, &QPushButton::clicked, this, &adminPage::addVille);
    connect(btnEdit, &QPushButton::clicked, this, &adminPage::editVille);
    connect(btnDelete, &QPushButton::clicked, this, &adminPage::deleteVille);

    return tab;
}

QWidget *adminPage::buildMatieresTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    tableMatieres = new QTableWidget(tab);
    tableMatieres->setColumnCount(3);
    tableMatieres->setHorizontalHeaderLabels(QStringList() << "ID" << "Nom" << "Alias");
    tableMatieres->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableMatieres->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableMatieres->setSelectionMode(QAbstractItemView::SingleSelection);
    tableMatieres->verticalHeader()->setVisible(false);
    tableMatieres->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QHBoxLayout *actions = new QHBoxLayout();
    QPushButton *btnAdd = new QPushButton("Creer", tab);
    QPushButton *btnEdit = new QPushButton("Modifier", tab);
    QPushButton *btnDelete = new QPushButton("Supprimer", tab);
    actions->addWidget(btnAdd);
    actions->addWidget(btnEdit);
    actions->addWidget(btnDelete);
    actions->addStretch();

    layout->addWidget(tableMatieres);
    layout->addLayout(actions);

    connect(btnAdd, &QPushButton::clicked, this, &adminPage::addMatiere);
    connect(btnEdit, &QPushButton::clicked, this, &adminPage::editMatiere);
    connect(btnDelete, &QPushButton::clicked, this, &adminPage::deleteMatiere);

    return tab;
}

QWidget *adminPage::buildCoursTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    tableCours = new QTableWidget(tab);
    tableCours->setColumnCount(2);
    tableCours->setHorizontalHeaderLabels(QStringList() << "ID" << "Nom");
    tableCours->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableCours->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableCours->setSelectionMode(QAbstractItemView::SingleSelection);
    tableCours->verticalHeader()->setVisible(false);
    tableCours->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QHBoxLayout *actions = new QHBoxLayout();
    QPushButton *btnAdd = new QPushButton("Creer", tab);
    QPushButton *btnEdit = new QPushButton("Modifier", tab);
    QPushButton *btnDelete = new QPushButton("Supprimer", tab);
    actions->addWidget(btnAdd);
    actions->addWidget(btnEdit);
    actions->addWidget(btnDelete);
    actions->addStretch();

    layout->addWidget(tableCours);
    layout->addLayout(actions);

    connect(btnAdd, &QPushButton::clicked, this, &adminPage::addCours);
    connect(btnEdit, &QPushButton::clicked, this, &adminPage::editCours);
    connect(btnDelete, &QPushButton::clicked, this, &adminPage::deleteCours);

    return tab;
}

QWidget *adminPage::buildEnseignantsTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    tableEnseignants = new QTableWidget(tab);
    tableEnseignants->setColumnCount(12);
    tableEnseignants->setHorizontalHeaderLabels(
        QStringList() << "enseignant_id" << "personne_id" << "Nom" << "Prenom" << "Sexe"
                      << "Username" << "ville_id" << "Ville" << "matiere_id" << "Matiere"
                      << "Salaire" << "Reset MDP"
    );
    tableEnseignants->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableEnseignants->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableEnseignants->setSelectionMode(QAbstractItemView::SingleSelection);
    tableEnseignants->verticalHeader()->setVisible(false);
    tableEnseignants->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableEnseignants->setColumnHidden(0, true);
    tableEnseignants->setColumnHidden(1, true);
    tableEnseignants->setColumnHidden(6, true);
    tableEnseignants->setColumnHidden(8, true);

    QHBoxLayout *actions = new QHBoxLayout();
    QPushButton *btnAdd = new QPushButton("Creer", tab);
    QPushButton *btnEdit = new QPushButton("Modifier", tab);
    QPushButton *btnDelete = new QPushButton("Supprimer", tab);
    QPushButton *btnReset = new QPushButton("Regenerer mot de passe", tab);
    actions->addWidget(btnAdd);
    actions->addWidget(btnEdit);
    actions->addWidget(btnDelete);
    actions->addWidget(btnReset);
    actions->addStretch();

    layout->addWidget(tableEnseignants);
    layout->addLayout(actions);

    connect(btnAdd, &QPushButton::clicked, this, &adminPage::addEnseignant);
    connect(btnEdit, &QPushButton::clicked, this, &adminPage::editEnseignant);
    connect(btnDelete, &QPushButton::clicked, this, &adminPage::deleteEnseignant);
    connect(btnReset, &QPushButton::clicked, this, &adminPage::resetPasswordEnseignant);

    return tab;
}

QWidget *adminPage::buildEtudiantsTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    tableEtudiants = new QTableWidget(tab);
    tableEtudiants->setColumnCount(13);
    tableEtudiants->setHorizontalHeaderLabels(
        QStringList() << "etudiant_id" << "personne_id" << "Nom" << "Prenom" << "Sexe"
                      << "Username" << "ville_id" << "Ville" << "Numero carte" << "Photo"
                      << "Cours" << "Admin" << "Reset MDP"
    );
    tableEtudiants->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableEtudiants->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableEtudiants->setSelectionMode(QAbstractItemView::SingleSelection);
    tableEtudiants->verticalHeader()->setVisible(false);
    tableEtudiants->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableEtudiants->setColumnHidden(0, true);
    tableEtudiants->setColumnHidden(1, true);
    tableEtudiants->setColumnHidden(6, true);
    tableEtudiants->setColumnHidden(9, true);

    QHBoxLayout *actions = new QHBoxLayout();
    QPushButton *btnAdd = new QPushButton("Creer", tab);
    QPushButton *btnEdit = new QPushButton("Modifier", tab);
    QPushButton *btnDelete = new QPushButton("Supprimer", tab);
    QPushButton *btnReset = new QPushButton("Regenerer mot de passe", tab);
    actions->addWidget(btnAdd);
    actions->addWidget(btnEdit);
    actions->addWidget(btnDelete);
    actions->addWidget(btnReset);
    actions->addStretch();

    layout->addWidget(tableEtudiants);
    layout->addLayout(actions);

    connect(btnAdd, &QPushButton::clicked, this, &adminPage::addEtudiant);
    connect(btnEdit, &QPushButton::clicked, this, &adminPage::editEtudiant);
    connect(btnDelete, &QPushButton::clicked, this, &adminPage::deleteEtudiant);
    connect(btnReset, &QPushButton::clicked, this, &adminPage::resetPasswordEtudiant);

    return tab;
}

void adminPage::loadAll()
{
    loadVilles();
    loadMatieres();
    loadCours();
    loadEnseignants();
    loadEtudiants();
}

void adminPage::loadVilles()
{
    adminManager manager;
    vector<villeAdminData> rows;

    if (!manager.getVilles(rows)) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les villes.");
        return;
    }

    tableVilles->setRowCount(static_cast<int>(rows.size()));
    for (int i = 0; i < static_cast<int>(rows.size()); ++i) {
        tableVilles->setItem(i, 0, new QTableWidgetItem(QString::number(rows[i].id)));
        tableVilles->setItem(i, 1, new QTableWidgetItem(rows[i].nom));
        tableVilles->setItem(i, 2, new QTableWidgetItem(rows[i].codePostal));
    }
}

void adminPage::loadMatieres()
{
    adminManager manager;
    vector<matiereAdminData> rows;

    if (!manager.getMatieres(rows)) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les matieres.");
        return;
    }

    tableMatieres->setRowCount(static_cast<int>(rows.size()));
    for (int i = 0; i < static_cast<int>(rows.size()); ++i) {
        tableMatieres->setItem(i, 0, new QTableWidgetItem(QString::number(rows[i].id)));
        tableMatieres->setItem(i, 1, new QTableWidgetItem(rows[i].nom));
        tableMatieres->setItem(i, 2, new QTableWidgetItem(rows[i].alias));
    }
}

void adminPage::loadCours()
{
    adminManager manager;
    vector<coursAdminData> rows;

    if (!manager.getCours(rows)) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les cours.");
        return;
    }

    tableCours->setRowCount(static_cast<int>(rows.size()));
    for (int i = 0; i < static_cast<int>(rows.size()); ++i) {
        tableCours->setItem(i, 0, new QTableWidgetItem(QString::number(rows[i].id)));
        tableCours->setItem(i, 1, new QTableWidgetItem(rows[i].nom));
    }
}

void adminPage::loadEnseignants()
{
    adminManager manager;
    vector<enseignantAdminData> rows;

    if (!manager.getEnseignants(rows)) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les enseignants.");
        return;
    }

    tableEnseignants->setRowCount(static_cast<int>(rows.size()));
    for (int i = 0; i < static_cast<int>(rows.size()); ++i) {
        tableEnseignants->setItem(i, 0, new QTableWidgetItem(QString::number(rows[i].enseignantId)));
        tableEnseignants->setItem(i, 1, new QTableWidgetItem(QString::number(rows[i].personneId)));
        tableEnseignants->setItem(i, 2, new QTableWidgetItem(rows[i].nom));
        tableEnseignants->setItem(i, 3, new QTableWidgetItem(rows[i].prenom));
        tableEnseignants->setItem(i, 4, new QTableWidgetItem(rows[i].sexe));
        tableEnseignants->setItem(i, 5, new QTableWidgetItem(rows[i].username));
        tableEnseignants->setItem(i, 6, new QTableWidgetItem(QString::number(rows[i].villeId)));
        tableEnseignants->setItem(i, 7, new QTableWidgetItem(rows[i].ville));
        tableEnseignants->setItem(i, 8, new QTableWidgetItem(QString::number(rows[i].matiereId)));
        tableEnseignants->setItem(i, 9, new QTableWidgetItem(rows[i].matiere));
        tableEnseignants->setItem(i, 10, new QTableWidgetItem(QString::number(rows[i].salaire, 'f', 2)));
        tableEnseignants->setItem(i, 11, new QTableWidgetItem(rows[i].needPasswordChange ? "Oui" : "Non"));
    }
}

void adminPage::loadEtudiants()
{
    adminManager manager;
    vector<etudiantAdminData> rows;

    if (!manager.getEtudiants(rows)) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les etudiants.");
        return;
    }

    tableEtudiants->setRowCount(static_cast<int>(rows.size()));
    for (int i = 0; i < static_cast<int>(rows.size()); ++i) {
        tableEtudiants->setItem(i, 0, new QTableWidgetItem(QString::number(rows[i].etudiantId)));
        tableEtudiants->setItem(i, 1, new QTableWidgetItem(QString::number(rows[i].personneId)));
        tableEtudiants->setItem(i, 2, new QTableWidgetItem(rows[i].nom));
        tableEtudiants->setItem(i, 3, new QTableWidgetItem(rows[i].prenom));
        tableEtudiants->setItem(i, 4, new QTableWidgetItem(rows[i].sexe));
        tableEtudiants->setItem(i, 5, new QTableWidgetItem(rows[i].username));
        tableEtudiants->setItem(i, 6, new QTableWidgetItem(QString::number(rows[i].villeId)));
        tableEtudiants->setItem(i, 7, new QTableWidgetItem(rows[i].ville));
        tableEtudiants->setItem(i, 8, new QTableWidgetItem(QString::number(rows[i].numCarte)));
        tableEtudiants->setItem(i, 9, new QTableWidgetItem(rows[i].photoUrl));
        tableEtudiants->setItem(i, 10, new QTableWidgetItem(rows[i].cours));
        tableEtudiants->setItem(i, 11, new QTableWidgetItem(rows[i].isAdmin ? "Oui" : "Non"));
        tableEtudiants->setItem(i, 12, new QTableWidgetItem(rows[i].needPasswordChange ? "Oui" : "Non"));
    }
}

void adminPage::loadVillesInCombo(QComboBox *combo)
{
    combo->clear();

    adminManager manager;
    vector<villeAdminData> rows;
    if (!manager.getVilles(rows)) {
        return;
    }

    for (const villeAdminData &row : rows) {
        combo->addItem(row.nom + " (" + row.codePostal + ")", row.id);
    }
}

void adminPage::loadMatieresInCombo(QComboBox *combo)
{
    combo->clear();

    adminManager manager;
    vector<matiereAdminData> rows;
    if (!manager.getMatieres(rows)) {
        return;
    }

    for (const matiereAdminData &row : rows) {
        combo->addItem(row.nom + " [" + row.alias + "]", row.id);
    }
}

void adminPage::loadCoursInList(QListWidget *listWidget, const QVector<int> &selectedCoursIds)
{
    listWidget->clear();

    adminManager manager;
    vector<coursAdminData> rows;
    if (!manager.getCours(rows)) {
        return;
    }

    for (const coursAdminData &row : rows) {
        QListWidgetItem *item = new QListWidgetItem(row.nom, listWidget);
        // UserRole stocke l'id technique sans l'afficher dans le texte
        item->setData(Qt::UserRole, row.id);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(selectedCoursIds.contains(row.id) ? Qt::Checked : Qt::Unchecked);
    }
}

QVector<int> adminPage::selectedCoursIds(QListWidget *listWidget) const
{
    QVector<int> ids;
    for (int i = 0; i < listWidget->count(); ++i) {
        QListWidgetItem *item = listWidget->item(i);
        if (item != nullptr && item->checkState() == Qt::Checked) {
            ids.push_back(item->data(Qt::UserRole).toInt());
        }
    }
    return ids;
}

bool adminPage::selectedIdFromTable(QTableWidget *table, int idColumn, int &id) const
{
    // helper centralise pour lire un id numerique depuis la ligne selectionnee
    const int row = table->currentRow();
    if (row < 0 || table->item(row, idColumn) == nullptr) {
        return false;
    }

    bool ok = false;
    const int value = table->item(row, idColumn)->text().toInt(&ok);
    if (!ok) {
        return false;
    }

    id = value;
    return true;
}

void adminPage::addVille()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Nouvelle ville");

    QFormLayout form(&dialog);
    QLineEdit inputNom;
    QLineEdit inputCp;
    form.addRow("Nom", &inputNom);
    form.addRow("Code postal", &inputCp);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(&buttons);
    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    adminManager manager;
    if (!manager.createVille(inputNom.text(), inputCp.text())) {
        QMessageBox::warning(this, "Erreur", "Creation de la ville impossible.");
        return;
    }

    loadVilles();
}

void adminPage::editVille()
{
    int villeId = -1;
    if (!selectedIdFromTable(tableVilles, 0, villeId)) {
        QMessageBox::information(this, "Selection", "Selectionnez une ville.");
        return;
    }

    const int row = tableVilles->currentRow();

    QDialog dialog(this);
    dialog.setWindowTitle("Modifier ville");

    QFormLayout form(&dialog);
    QLineEdit inputNom(tableVilles->item(row, 1)->text());
    QLineEdit inputCp(tableVilles->item(row, 2)->text());
    form.addRow("Nom", &inputNom);
    form.addRow("Code postal", &inputCp);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(&buttons);
    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    adminManager manager;
    if (!manager.updateVille(villeId, inputNom.text(), inputCp.text())) {
        QMessageBox::warning(this, "Erreur", "Modification de la ville impossible.");
        return;
    }

    loadVilles();
}

void adminPage::deleteVille()
{
    int villeId = -1;
    if (!selectedIdFromTable(tableVilles, 0, villeId)) {
        QMessageBox::information(this, "Selection", "Selectionnez une ville.");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer cette ville ?") != QMessageBox::Yes) {
        return;
    }

    adminManager manager;
    if (!manager.deleteVille(villeId)) {
        QMessageBox::warning(this, "Erreur", "Suppression de la ville impossible.");
        return;
    }

    loadVilles();
}

void adminPage::addMatiere()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Nouvelle matiere");

    QFormLayout form(&dialog);
    QLineEdit inputNom;
    QLineEdit inputAlias;
    form.addRow("Nom", &inputNom);
    form.addRow("Alias", &inputAlias);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(&buttons);
    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    adminManager manager;
    if (!manager.createMatiere(inputNom.text(), inputAlias.text())) {
        QMessageBox::warning(this, "Erreur", "Creation de la matiere impossible.");
        return;
    }

    loadMatieres();
}

void adminPage::editMatiere()
{
    int matiereId = -1;
    if (!selectedIdFromTable(tableMatieres, 0, matiereId)) {
        QMessageBox::information(this, "Selection", "Selectionnez une matiere.");
        return;
    }

    const int row = tableMatieres->currentRow();

    QDialog dialog(this);
    dialog.setWindowTitle("Modifier matiere");

    QFormLayout form(&dialog);
    QLineEdit inputNom(tableMatieres->item(row, 1)->text());
    QLineEdit inputAlias(tableMatieres->item(row, 2)->text());
    form.addRow("Nom", &inputNom);
    form.addRow("Alias", &inputAlias);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(&buttons);
    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    adminManager manager;
    if (!manager.updateMatiere(matiereId, inputNom.text(), inputAlias.text())) {
        QMessageBox::warning(this, "Erreur", "Modification de la matiere impossible.");
        return;
    }

    loadMatieres();
}

void adminPage::deleteMatiere()
{
    int matiereId = -1;
    if (!selectedIdFromTable(tableMatieres, 0, matiereId)) {
        QMessageBox::information(this, "Selection", "Selectionnez une matiere.");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer cette matiere ?") != QMessageBox::Yes) {
        return;
    }

    adminManager manager;
    if (!manager.deleteMatiere(matiereId)) {
        QMessageBox::warning(this, "Erreur", "Suppression de la matiere impossible.");
        return;
    }

    loadMatieres();
}

void adminPage::addCours()
{
    bool ok = false;
    const QString nom = QInputDialog::getText(this, "Nouveau cours", "Nom", QLineEdit::Normal, "", &ok);
    if (!ok || nom.trimmed().isEmpty()) return;

    adminManager manager;
    if (!manager.createCours(nom)) {
        QMessageBox::warning(this, "Erreur", "Creation du cours impossible.");
        return;
    }

    loadCours();
}

void adminPage::editCours()
{
    int coursId = -1;
    if (!selectedIdFromTable(tableCours, 0, coursId)) {
        QMessageBox::information(this, "Selection", "Selectionnez un cours.");
        return;
    }

    const int row = tableCours->currentRow();
    bool ok = false;
    const QString currentName = tableCours->item(row, 1)->text();
    const QString nom = QInputDialog::getText(this, "Modifier cours", "Nom", QLineEdit::Normal, currentName, &ok);
    if (!ok || nom.trimmed().isEmpty()) return;

    adminManager manager;
    if (!manager.updateCours(coursId, nom)) {
        QMessageBox::warning(this, "Erreur", "Modification du cours impossible.");
        return;
    }

    loadCours();
}

void adminPage::deleteCours()
{
    int coursId = -1;
    if (!selectedIdFromTable(tableCours, 0, coursId)) {
        QMessageBox::information(this, "Selection", "Selectionnez un cours.");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer ce cours ?") != QMessageBox::Yes) {
        return;
    }

    adminManager manager;
    if (!manager.deleteCours(coursId)) {
        QMessageBox::warning(this, "Erreur", "Suppression du cours impossible.");
        return;
    }

    loadCours();
}

void adminPage::addEnseignant()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Nouvel enseignant");

    QFormLayout form(&dialog);
    QLineEdit inputNom;
    QLineEdit inputPrenom;
    QComboBox inputSexe;
    inputSexe.addItems(QStringList() << "M" << "F");
    QLineEdit inputUsername;
    QComboBox inputVille;
    QComboBox inputMatiere;
    QDoubleSpinBox inputSalaire;
    inputSalaire.setRange(0, 1000000);
    inputSalaire.setDecimals(2);
    inputSalaire.setValue(2500.0);

    loadVillesInCombo(&inputVille);
    loadMatieresInCombo(&inputMatiere);

    form.addRow("Nom", &inputNom);
    form.addRow("Prenom", &inputPrenom);
    form.addRow("Sexe", &inputSexe);
    form.addRow("Username", &inputUsername);
    form.addRow("Ville", &inputVille);
    form.addRow("Matiere", &inputMatiere);
    form.addRow("Salaire", &inputSalaire);

    QLabel generatedInfo("Le mot de passe temporaire sera genere automatiquement.");
    generatedInfo.setStyleSheet("color: #57606f;");
    form.addRow("Info", &generatedInfo);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(&buttons);
    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    adminManager manager;
    QString generatedPassword;

    // le manager renvoie le mot de passe temporaire pour l'afficher juste apres la creation
    if (!manager.createEnseignant(
            inputNom.text(),
            inputPrenom.text(),
            inputSexe.currentText(),
            inputVille.currentData().toInt(),
            inputUsername.text(),
            inputMatiere.currentData().toInt(),
            inputSalaire.value(),
            generatedPassword)) {
        QMessageBox::warning(this, "Erreur", "Creation de l'enseignant impossible.\nVerifiez l'unicite du username.");
        return;
    }

    showTemporaryPasswordDialog(
        "Compte enseignant cree",
        inputUsername.text(),
        generatedPassword,
        "Identifiants temporaires (a communiquer):"
    );

    loadEnseignants();
}

void adminPage::editEnseignant()
{
    int enseignantId = -1;
    int personneId = -1;
    if (!selectedIdFromTable(tableEnseignants, 0, enseignantId) || !selectedIdFromTable(tableEnseignants, 1, personneId)) {
        QMessageBox::information(this, "Selection", "Selectionnez un enseignant.");
        return;
    }

    const int row = tableEnseignants->currentRow();

    QDialog dialog(this);
    dialog.setWindowTitle("Modifier enseignant");

    QFormLayout form(&dialog);
    QLineEdit inputNom(tableEnseignants->item(row, 2)->text());
    QLineEdit inputPrenom(tableEnseignants->item(row, 3)->text());
    QComboBox inputSexe;
    inputSexe.addItems(QStringList() << "M" << "F");
    inputSexe.setCurrentText(tableEnseignants->item(row, 4)->text());
    QLineEdit inputUsername(tableEnseignants->item(row, 5)->text());
    QComboBox inputVille;
    QComboBox inputMatiere;
    QDoubleSpinBox inputSalaire;
    inputSalaire.setRange(0, 1000000);
    inputSalaire.setDecimals(2);
    inputSalaire.setValue(tableEnseignants->item(row, 10)->text().toDouble());

    loadVillesInCombo(&inputVille);
    loadMatieresInCombo(&inputMatiere);

    const int villeId = tableEnseignants->item(row, 6)->text().toInt();
    const int matiereId = tableEnseignants->item(row, 8)->text().toInt();

    // on repositionne les combos sur les ids reels caches dans le tableau
    for (int i = 0; i < inputVille.count(); ++i) {
        if (inputVille.itemData(i).toInt() == villeId) {
            inputVille.setCurrentIndex(i);
            break;
        }
    }

    for (int i = 0; i < inputMatiere.count(); ++i) {
        if (inputMatiere.itemData(i).toInt() == matiereId) {
            inputMatiere.setCurrentIndex(i);
            break;
        }
    }

    form.addRow("Nom", &inputNom);
    form.addRow("Prenom", &inputPrenom);
    form.addRow("Sexe", &inputSexe);
    form.addRow("Username", &inputUsername);
    form.addRow("Ville", &inputVille);
    form.addRow("Matiere", &inputMatiere);
    form.addRow("Salaire", &inputSalaire);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(&buttons);
    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    adminManager manager;
    if (!manager.updateEnseignant(
            enseignantId,
            personneId,
            inputNom.text(),
            inputPrenom.text(),
            inputSexe.currentText(),
            inputVille.currentData().toInt(),
            inputUsername.text(),
            inputMatiere.currentData().toInt(),
            inputSalaire.value())) {
        QMessageBox::warning(this, "Erreur", "Modification de l'enseignant impossible.");
        return;
    }

    loadEnseignants();
}

void adminPage::deleteEnseignant()
{
    int enseignantId = -1;
    int personneId = -1;
    if (!selectedIdFromTable(tableEnseignants, 0, enseignantId) || !selectedIdFromTable(tableEnseignants, 1, personneId)) {
        QMessageBox::information(this, "Selection", "Selectionnez un enseignant.");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer cet enseignant ?") != QMessageBox::Yes) {
        return;
    }

    adminManager manager;
    if (!manager.deleteEnseignant(enseignantId, personneId)) {
        QMessageBox::warning(this, "Erreur", "Suppression de l'enseignant impossible.");
        return;
    }

    loadEnseignants();
}

void adminPage::resetPasswordEnseignant()
{
    int personneId = -1;
    if (!selectedIdFromTable(tableEnseignants, 1, personneId)) {
        QMessageBox::information(this, "Selection", "Selectionnez un enseignant.");
        return;
    }

    const int row = tableEnseignants->currentRow();
    const QString login = tableEnseignants->item(row, 5) ? tableEnseignants->item(row, 5)->text() : QString();

    adminManager manager;
    QString temporaryPassword;
    if (!manager.resetPasswordUtilisateur(personneId, temporaryPassword)) {
        QMessageBox::warning(this, "Erreur", "Regeneration du mot de passe impossible.");
        return;
    }

    showTemporaryPasswordDialog(
        "Mot de passe enseignant regenere",
        login,
        temporaryPassword,
        "Identifiants temporaires:"
    );

    loadEnseignants();
}

void adminPage::addEtudiant()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Nouvel etudiant");

    QFormLayout form(&dialog);
    QLineEdit *inputNom = new QLineEdit(&dialog);
    QLineEdit *inputPrenom = new QLineEdit(&dialog);
    QComboBox *inputSexe = new QComboBox(&dialog);
    inputSexe->addItems(QStringList() << "M" << "F");
    QLineEdit *inputUsername = new QLineEdit(&dialog);
    QComboBox *inputVille = new QComboBox(&dialog);
    fileDropLineEdit *inputPhoto = new fileDropLineEdit(&dialog);
    inputPhoto->setPlaceholderText("Glissez-deposez une image ou utilisez Parcourir");
    QPushButton *btnBrowsePhoto = new QPushButton("Parcourir...", &dialog);
    QListWidget *coursList = new QListWidget(&dialog);
    coursList->setSelectionMode(QAbstractItemView::NoSelection);
    coursList->setMinimumHeight(180);

    // lambda locale pour gerer le picker de fichier sans creer de slot dedie
    QObject::connect(btnBrowsePhoto, &QPushButton::clicked, [&]() {
        const QString filePath = QFileDialog::getOpenFileName(
            &dialog,
            "Selectionner une photo",
            QString(),
            "Images (*.png *.jpg *.jpeg *.bmp *.webp)"
        );
        if (!filePath.isEmpty()) {
            inputPhoto->setText(filePath);
        }
    });

    QWidget *photoContainer = new QWidget(&dialog);
    QHBoxLayout *photoLayout = new QHBoxLayout(photoContainer);
    photoLayout->setContentsMargins(0, 0, 0, 0);
    photoLayout->addWidget(inputPhoto);
    photoLayout->addWidget(btnBrowsePhoto);

    loadVillesInCombo(inputVille);

    // liste multi-coches pour simuler une selection de cours many-to-many
    loadCoursInList(coursList);

    form.addRow("Nom", inputNom);
    form.addRow("Prenom", inputPrenom);
    form.addRow("Sexe", inputSexe);
    form.addRow("Username", inputUsername);
    form.addRow("Ville", inputVille);
    form.addRow("Photo", photoContainer);
    form.addRow("Cours", coursList);

    QLabel generatedInfo("Le mot de passe temporaire sera genere automatiquement.");
    generatedInfo.setStyleSheet("color: #57606f;");
    form.addRow("Info", &generatedInfo);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    adminManager manager;
    QString generatedPassword;
    if (!manager.createEtudiant(
            inputNom->text(),
            inputPrenom->text(),
            inputSexe->currentText(),
            inputVille->currentData().toInt(),
            inputUsername->text(),
            inputPhoto->text(),
            selectedCoursIds(coursList),
            generatedPassword)) {
        QMessageBox::warning(this, "Erreur", "Creation de l'etudiant impossible.\nVerifiez l'unicite du username.");
        return;
    }

    showTemporaryPasswordDialog(
        "Compte etudiant cree",
        inputUsername->text(),
        generatedPassword,
        "Identifiants temporaires (a communiquer):"
    );

    loadEtudiants();
}

void adminPage::editEtudiant()
{
    int etudiantId = -1;
    int personneId = -1;
    if (!selectedIdFromTable(tableEtudiants, 0, etudiantId) || !selectedIdFromTable(tableEtudiants, 1, personneId)) {
        QMessageBox::information(this, "Selection", "Selectionnez un etudiant.");
        return;
    }

    const int row = tableEtudiants->currentRow();

    QDialog dialog(this);
    dialog.setWindowTitle("Modifier etudiant");

    QFormLayout form(&dialog);
    QLineEdit *inputNom = new QLineEdit(tableEtudiants->item(row, 2)->text(), &dialog);
    QLineEdit *inputPrenom = new QLineEdit(tableEtudiants->item(row, 3)->text(), &dialog);
    QComboBox *inputSexe = new QComboBox(&dialog);
    inputSexe->addItems(QStringList() << "M" << "F");
    inputSexe->setCurrentText(tableEtudiants->item(row, 4)->text());
    QLineEdit *inputUsername = new QLineEdit(tableEtudiants->item(row, 5)->text(), &dialog);
    QComboBox *inputVille = new QComboBox(&dialog);
    fileDropLineEdit *inputPhoto = new fileDropLineEdit(&dialog);
    inputPhoto->setText(tableEtudiants->item(row, 9)->text());
    inputPhoto->setPlaceholderText("Glissez-deposez une image ou utilisez Parcourir");
    QPushButton *btnBrowsePhoto = new QPushButton("Parcourir...", &dialog);
    QListWidget *coursList = new QListWidget(&dialog);
    coursList->setSelectionMode(QAbstractItemView::NoSelection);
    coursList->setMinimumHeight(180);

    QObject::connect(btnBrowsePhoto, &QPushButton::clicked, [&]() {
        const QString filePath = QFileDialog::getOpenFileName(
            &dialog,
            "Selectionner une photo",
            inputPhoto->text(),
            "Images (*.png *.jpg *.jpeg *.bmp *.webp)"
        );
        if (!filePath.isEmpty()) {
            inputPhoto->setText(filePath);
        }
    });

    QWidget *photoContainer = new QWidget(&dialog);
    QHBoxLayout *photoLayout = new QHBoxLayout(photoContainer);
    photoLayout->setContentsMargins(0, 0, 0, 0);
    photoLayout->addWidget(inputPhoto);
    photoLayout->addWidget(btnBrowsePhoto);

    loadVillesInCombo(inputVille);

    const int villeId = tableEtudiants->item(row, 6)->text().toInt();
    for (int i = 0; i < inputVille->count(); ++i) {
        if (inputVille->itemData(i).toInt() == villeId) {
            inputVille->setCurrentIndex(i);
            break;
        }
    }

    adminManager manager;
    QVector<int> selectedIds;

    // on recharge les cours deja associes a l'etudiant pour pre-cocher la liste
    manager.getEtudiantCoursIds(etudiantId, selectedIds);
    loadCoursInList(coursList, selectedIds);

    form.addRow("Nom", inputNom);
    form.addRow("Prenom", inputPrenom);
    form.addRow("Sexe", inputSexe);
    form.addRow("Username", inputUsername);
    form.addRow("Ville", inputVille);
    form.addRow("Photo", photoContainer);
    form.addRow("Cours", coursList);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    if (!manager.updateEtudiant(
            etudiantId,
            personneId,
            inputNom->text(),
            inputPrenom->text(),
            inputSexe->currentText(),
            inputVille->currentData().toInt(),
            inputUsername->text(),
            inputPhoto->text(),
            selectedCoursIds(coursList))) {
        QMessageBox::warning(this, "Erreur", "Modification de l'etudiant impossible.");
        return;
    }

    loadEtudiants();
}

void adminPage::deleteEtudiant()
{
    int etudiantId = -1;
    int personneId = -1;
    if (!selectedIdFromTable(tableEtudiants, 0, etudiantId) || !selectedIdFromTable(tableEtudiants, 1, personneId)) {
        QMessageBox::information(this, "Selection", "Selectionnez un etudiant.");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer cet etudiant ?") != QMessageBox::Yes) {
        return;
    }

    adminManager manager;
    if (!manager.deleteEtudiant(etudiantId, personneId)) {
        QMessageBox::warning(this, "Erreur", "Suppression de l'etudiant impossible.");
        return;
    }

    loadEtudiants();
}

void adminPage::resetPasswordEtudiant()
{
    int personneId = -1;
    if (!selectedIdFromTable(tableEtudiants, 1, personneId)) {
        QMessageBox::information(this, "Selection", "Selectionnez un etudiant.");
        return;
    }

    const int row = tableEtudiants->currentRow();
    const QString login = tableEtudiants->item(row, 5) ? tableEtudiants->item(row, 5)->text() : QString();

    adminManager manager;
    QString temporaryPassword;
    if (!manager.resetPasswordUtilisateur(personneId, temporaryPassword)) {
        QMessageBox::warning(this, "Erreur", "Regeneration du mot de passe impossible.");
        return;
    }

    showTemporaryPasswordDialog(
        "Mot de passe etudiant regenere",
        login,
        temporaryPassword,
        "Identifiants temporaires:"
    );

    loadEtudiants();
}

void adminPage::showTemporaryPasswordDialog(const QString &title, const QString &login, const QString &password, const QString &subtitle) const
{
    QDialog dialog(const_cast<adminPage*>(this));
    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(560);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *subtitleLabel = new QLabel(subtitle, &dialog);
    subtitleLabel->setWordWrap(true);

    QLineEdit *loginField = new QLineEdit(login, &dialog);
    loginField->setReadOnly(true);
    loginField->setMinimumHeight(38);

    QLineEdit *passwordField = new QLineEdit(password, &dialog);
    passwordField->setReadOnly(true);
    passwordField->setMinimumHeight(38);

    QLabel *hint = new QLabel("Login et mot de passe ont ete copies dans le presse-papiers.", &dialog);
    hint->setStyleSheet("color: #57606f;");

    QHBoxLayout *buttonRow = new QHBoxLayout();
    QPushButton *btnCopy = new QPushButton("Copier", &dialog);
    QPushButton *btnClose = new QPushButton("Fermer", &dialog);
    buttonRow->addStretch();
    buttonRow->addWidget(btnCopy);
    buttonRow->addWidget(btnClose);

    layout->addWidget(subtitleLabel);
    layout->addWidget(new QLabel("Login", &dialog));
    layout->addWidget(loginField);
    layout->addWidget(new QLabel("Mot de passe temporaire", &dialog));
    layout->addWidget(passwordField);
    layout->addWidget(hint);
    layout->addLayout(buttonRow);

    // on copie direct pour eviter les erreurs de recopie quand l'admin cree plusieurs comptes
    QGuiApplication::clipboard()->setText(QString("Login: %1\nMot de passe: %2").arg(loginField->text(), passwordField->text()));

    QObject::connect(btnCopy, &QPushButton::clicked, [&]() {
        QGuiApplication::clipboard()->setText(QString("Login: %1\nMot de passe: %2").arg(loginField->text(), passwordField->text()));
        passwordField->selectAll();
    });
    QObject::connect(btnClose, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}
