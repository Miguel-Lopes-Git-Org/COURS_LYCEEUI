#ifndef ETUDIANTSPAGE_H
#define ETUDIANTSPAGE_H

#include <QWidget>

class QLabel;
class QPushButton;
class QScrollArea;
class QVBoxLayout;

class etudiantsPage : public QWidget
{
    Q_OBJECT

public:
    explicit etudiantsPage(QWidget *parent = nullptr);
    ~etudiantsPage();
    void loadEtudiants();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void retourDemande();

private:
    void clearCards();
    QWidget *buildStudentCard(const QString &photoPath, const QString &nom, const QString &prenom,
                              const QString &numCarte, const QString &ville, const QString &cours);
    void updateResponsiveUi();
    QLabel *titleLabel;
    QScrollArea *scrollArea;
    QWidget *cardsContainer;
    QVBoxLayout *cardsLayout;
    QLabel *emptyLabel;
    QPushButton *btnRetour;
};

#endif // ETUDIANTSPAGE_H
