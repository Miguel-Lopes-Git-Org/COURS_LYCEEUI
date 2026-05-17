#ifndef FILEDROPLINEEDIT_H
#define FILEDROPLINEEDIT_H

#include <QLineEdit>

class fileDropLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit fileDropLineEdit(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

#endif // FILEDROPLINEEDIT_H
