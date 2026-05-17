#ifndef FORMATIONPAGE_H
#define FORMATIONPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class formationPage;
}

class formationPage : public QWidget
{
    Q_OBJECT

public:
    explicit formationPage(QWidget *parent = nullptr);
    ~formationPage();
    void loadFormations();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void retourDemande();

private:
    void updateResponsiveUi();
    void clearFormationList();
    void showEnseignants();
    QVBoxLayout *mainLayout;
    QScrollArea *scrollArea;
    QWidget *scrollContent;     // Le conteneur invisible dans le ScrollArea
    QVBoxLayout *listLayout;    // Le layout qui va contenir tes lignes
    QPushButton *btnRetour;
    QPushButton *btnEnseignants;
    QLabel *titleLabel;
    QLabel *emptyLabel;
};

#endif // FORMATIONPAGE_H
