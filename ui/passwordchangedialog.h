#ifndef PASSWORDCHANGEDIALOG_H
#define PASSWORDCHANGEDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

class passwordChangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit passwordChangeDialog(int userId, QWidget *parent = nullptr);

private:
    void handleSave();

    int m_userId;
    QLineEdit *inputPassword;
    QLineEdit *inputPasswordConfirm;
    QLabel *messageLabel;
    QPushButton *btnSave;
    QPushButton *btnCancel;
};

#endif // PASSWORDCHANGEDIALOG_H
