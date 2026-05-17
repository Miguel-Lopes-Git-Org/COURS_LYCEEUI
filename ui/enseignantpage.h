#ifndef ENSEIGNANTPAGE_H
#define ENSEIGNANTPAGE_H

#include <QWidget>

class QLabel;
class QPushButton;
class QTableWidget;

class enseignantPage : public QWidget
{
    Q_OBJECT

public:
    explicit enseignantPage(QWidget *parent = nullptr);
    ~enseignantPage();
    void loadEnseignants();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void retourDemande();

private:
    void updateResponsiveUi();
    QTableWidget *table;
    QLabel *titleLabel;
    QLabel *emptyLabel;
    QPushButton *btnRetour;
};

#endif // ENSEIGNANTPAGE_H