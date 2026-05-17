#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;

class loginPage : public QWidget
{
    Q_OBJECT

public:
    explicit loginPage(QWidget *parent = nullptr);
    ~loginPage();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void retourDemande();
    void connexionReussie();

private:
    void updateResponsiveUi();
    void tryLogin();

    QLabel *titleLabel;
    QWidget *formContainer;
    QLineEdit *inputUsername;
    QLineEdit *inputPassword;
    QLabel *messageLabel;
    QPushButton *btnConnect;
    QPushButton *btnRetour;
};

#endif // LOGINPAGE_H