#include "formationwidget.h"

formationWidget::formationWidget(QWidget *parent) : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0); // Enlève les marges pour coller les bordures
    layout->setSpacing(0); // Pas d'espace entre les colonnes

    lblMatiere = new QLabel(this);
    lblS1 = new QLabel(this);
    lblS2 = new QLabel(this);
    lblS34 = new QLabel(this);

    // 1. Colonne Matière
    lblMatiere->setMinimumWidth(450); // Largeur généreuse pour le texte
    lblMatiere->setWordWrap(true);    // Active le retour à la ligne automatique
    lblMatiere->setContentsMargins(10, 15, 10, 15); // Un peu d'air autour du texte

    // 2. Colonnes Heures (Taille fixe + Centrage)
    int hoursWidth = 150; // La même taille pour tous les semestres

    lblS1->setFixedWidth(hoursWidth);
    lblS1->setAlignment(Qt::AlignCenter);

    lblS2->setFixedWidth(hoursWidth);
    lblS2->setAlignment(Qt::AlignCenter);

    lblS34->setFixedWidth(hoursWidth);
    lblS34->setAlignment(Qt::AlignCenter);

    // Ajout au layout
    layout->addWidget(lblMatiere);
    layout->addWidget(lblS1);
    layout->addWidget(lblS2);
    layout->addWidget(lblS34);

    // 3. STYLE ET BORDURES (2px)
    // On met une bordure à droite de chaque label (sauf le dernier) pour faire les colonnes
    QString colStyle = "border-right: 2px solid #bdc3c7; color: #2c3e50;";
    lblMatiere->setStyleSheet(colStyle);
    lblS1->setStyleSheet(colStyle);
    lblS2->setStyleSheet(colStyle);
    lblS34->setStyleSheet("color: #2c3e50;"); // Pas de bordure à droite pour le dernier

    // On met une bordure en bas du widget entier pour faire les lignes
    this->setStyleSheet(
        "formationWidget { "
        "   border-bottom: 2px solid #bdc3c7; "
        "   background-color: #ffffff; "
        "} "
        "formationWidget:hover { background-color: #f1f2f6; }" // Effet au survol !
        );
}

formationWidget::~formationWidget() {}

void formationWidget::setFormationData(formation *f) {
    lblMatiere->setText(f->getMatiere());
    // On retire les "S1:", on garde juste le chiffre et le "h"
    lblS1->setText(QString(f->getNbHeuresS1()) + " h");
    lblS2->setText(QString(f->getNbHeuresS2()) + " h");
    lblS34->setText(QString(f->getNbHeuresS3et4()) + " h");
}
