#ifndef FORMATIONWIDGET_H
#define FORMATIONWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include "formation.h"

namespace Ui {
class formationWidget;
}

class formationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit formationWidget(QWidget *parent = nullptr);
    ~formationWidget();
    void setFormationData(formation *f);

private:
    QLabel *lblMatiere;
    QLabel *lblS1;
    QLabel *lblS2;
    QLabel *lblS34;
};

#endif // FORMATIONWIDGET_H
