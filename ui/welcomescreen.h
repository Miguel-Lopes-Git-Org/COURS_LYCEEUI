#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class WelcomeScreen : public QWidget {
    Q_OBJECT

public:
    explicit WelcomeScreen(QWidget *parent = nullptr);

protected:
    // On surcharge paintEvent pour dessiner l'image de fond proprement
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateAuthUi();
    void authButtonClicked();
    void openFormationPage();
    void openAdminPage();
    void openEtudiantsPage();
    void seeFormationClicked();
    void showLoginClicked();
    void showAdminClicked();
    void showEtudiantsClicked();
    void updateResponsiveUi();
    QPushButton *btnDiscover;
    QPushButton *btnLogin;
    QPushButton *btnAdmin;
    QPushButton *btnEtudiants;
    QLabel *labelSessionInfo;
    QLabel *labelTagline;
    QLabel *labelTitle;
    QLabel *labelDesc;
};

#endif // WELCOMESCREEN_H
