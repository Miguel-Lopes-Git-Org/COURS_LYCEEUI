#ifndef ADMINPAGE_H
#define ADMINPAGE_H

#include <QWidget>

class QTabWidget;
class QTableWidget;
class QPushButton;
class QComboBox;
class QLabel;
class QResizeEvent;
class QListWidget;

class adminPage : public QWidget
{
    Q_OBJECT

public:
    explicit adminPage(QWidget *parent = nullptr);
    ~adminPage();
    void loadAll();

signals:
    void retourDemande();

private:
    void resizeEvent(QResizeEvent *event) override;
    void setupUi();
    QWidget *buildVillesTab();
    QWidget *buildMatieresTab();
    QWidget *buildCoursTab();
    QWidget *buildEnseignantsTab();
    QWidget *buildEtudiantsTab();

    void loadVilles();
    void loadMatieres();
    void loadCours();
    void loadEnseignants();
    void loadEtudiants();

    void loadVillesInCombo(QComboBox *combo);
    void loadMatieresInCombo(QComboBox *combo);
    void loadCoursInList(QListWidget *listWidget, const QVector<int> &selectedCoursIds = QVector<int>());
    QVector<int> selectedCoursIds(QListWidget *listWidget) const;

    bool selectedIdFromTable(QTableWidget *table, int idColumn, int &id) const;

    void addVille();
    void editVille();
    void deleteVille();

    void addMatiere();
    void editMatiere();
    void deleteMatiere();

    void addCours();
    void editCours();
    void deleteCours();

    void addEnseignant();
    void editEnseignant();
    void deleteEnseignant();
    void resetPasswordEnseignant();

    void addEtudiant();
    void editEtudiant();
    void deleteEtudiant();
    void resetPasswordEtudiant();

    void showTemporaryPasswordDialog(const QString &title, const QString &login, const QString &password, const QString &subtitle) const;

    QTabWidget *tabs;
    QPushButton *btnRetour;
    QLabel *pageTitle;

    QTableWidget *tableVilles;
    QTableWidget *tableMatieres;
    QTableWidget *tableCours;
    QTableWidget *tableEnseignants;
    QTableWidget *tableEtudiants;
};

#endif // ADMINPAGE_H
