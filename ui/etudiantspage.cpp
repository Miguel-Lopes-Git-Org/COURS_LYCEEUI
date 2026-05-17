#include "etudiantspage.h"

#include "scolariteManager.h"

#include <QFrame>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QVariant>
#include <QVBoxLayout>

etudiantsPage::etudiantsPage(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Lycée Franklin Roosevelt | Etudiants");
    setMinimumSize(1000, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topLayout = new QHBoxLayout();
    btnRetour = new QPushButton("⬅ Retour", this);
    topLayout->addWidget(btnRetour);
    topLayout->addStretch();

    titleLabel = new QLabel("Liste des Etudiants", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    cardsContainer = new QWidget(scrollArea);
    cardsLayout = new QVBoxLayout(cardsContainer);
    cardsLayout->setAlignment(Qt::AlignTop);
    cardsLayout->setContentsMargins(8, 8, 8, 8);
    cardsLayout->setSpacing(10);
    scrollArea->setWidget(cardsContainer);

    emptyLabel = new QLabel(this);
    emptyLabel->setAlignment(Qt::AlignCenter);
    emptyLabel->setStyleSheet("font-size: 15px; color: #57606f;");
    emptyLabel->hide();

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(scrollArea);
    mainLayout->addWidget(emptyLabel);

    connect(btnRetour, &QPushButton::clicked, this, &etudiantsPage::retourDemande);

    updateResponsiveUi();
}

etudiantsPage::~etudiantsPage() {}

void etudiantsPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateResponsiveUi();
}

void etudiantsPage::updateResponsiveUi()
{
    const double scaleW = static_cast<double>(width()) / 1000.0;
    const double scaleH = static_cast<double>(height()) / 600.0;
    const double scale = qBound(1.0, qMin(scaleW, scaleH), 1.8);

    QFont pageFont = this->font();
    pageFont.setPointSize(qRound(11 * scale));
    this->setFont(pageFont);

    titleLabel->setStyleSheet(QString("font-size: %1px; font-weight: bold;").arg(qRound(24 * scale)));
    btnRetour->setMinimumHeight(qRound(36 * scale));
    cardsLayout->setSpacing(qRound(12 * scale));

    const int photoSize = qRound(106 * scale);
    const int nameSize = qRound(20 * scale);
    const int textSize = qRound(13 * scale);

    // on recupere tous les labels des cartes puis on applique le style selon des proprietes custom
    const QList<QLabel*> labels = cardsContainer->findChildren<QLabel*>();
    for (QLabel *label : labels) {
        if (label->property("studentPhoto").toBool()) {
            label->setFixedSize(photoSize, photoSize);
            const QVariant original = label->property("originalPhotoPath");
            const QString photoPath = original.toString();

            // on recharge le pixmap a la nouvelle taille pour eviter une image floue au resize
            if (!photoPath.isEmpty() && QFileInfo::exists(photoPath)) {
                QPixmap pix(photoPath);
                if (!pix.isNull()) {
                    label->setPixmap(pix.scaled(photoSize, photoSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
                }
            }
        }

        if (label->property("studentName").toBool()) {
            label->setStyleSheet(QString("font-size: %1px; font-weight: 700; color: #1f2d3d;").arg(nameSize));
        } else if (label->property("studentMeta").toBool()) {
            label->setStyleSheet(QString("font-size: %1px; color: #3c4b5d;").arg(textSize));
        }
    }
}

QWidget *etudiantsPage::buildStudentCard(const QString &photoPath,
                                         const QString &nom,
                                         const QString &prenom,
                                         const QString &numCarte,
                                         const QString &ville,
                                         const QString &cours)
{
    QFrame *card = new QFrame(cardsContainer);
    card->setStyleSheet(
        "QFrame {"
        "background: #f7f9fc;"
        "border: 1px solid #d6deea;"
        "border-radius: 10px;"
        "}"
    );

    QHBoxLayout *cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(14, 14, 14, 14);
    cardLayout->setSpacing(14);

    QLabel *photoLabel = new QLabel(card);
    photoLabel->setAlignment(Qt::AlignCenter);

    // ces proprietes servent plus tard dans updateResponsiveUi pour reconnaitre chaque type de label
    photoLabel->setProperty("studentPhoto", true);
    photoLabel->setProperty("originalPhotoPath", photoPath);
    photoLabel->setStyleSheet("background: #ffffff; border: 1px solid #cfd8e6; border-radius: 8px;");

    if (!photoPath.isEmpty() && QFileInfo::exists(photoPath)) {
        QPixmap pix(photoPath);
        if (!pix.isNull()) {
            photoLabel->setPixmap(pix.scaled(106, 106, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        } else {
            photoLabel->setText("Image\ninvalide");
        }
    } else {
        photoLabel->setText("Aucune\nphoto");
    }

    QWidget *details = new QWidget(card);
    QVBoxLayout *detailsLayout = new QVBoxLayout(details);
    detailsLayout->setContentsMargins(0, 0, 0, 0);
    detailsLayout->setSpacing(6);

    QLabel *nameLabel = new QLabel(nom + " " + prenom, details);
    nameLabel->setProperty("studentName", true);

    QLabel *cardLabel = new QLabel("Carte: " + numCarte, details);
    cardLabel->setProperty("studentMeta", true);

    QLabel *villeLabel = new QLabel("Ville: " + ville, details);
    villeLabel->setProperty("studentMeta", true);

    QLabel *coursLabel = new QLabel("Cours: " + (cours.isEmpty() ? "-" : cours), details);
    coursLabel->setProperty("studentMeta", true);
    coursLabel->setWordWrap(true);

    detailsLayout->addWidget(nameLabel);
    detailsLayout->addWidget(cardLabel);
    detailsLayout->addWidget(villeLabel);
    detailsLayout->addWidget(coursLabel);
    detailsLayout->addStretch();

    cardLayout->addWidget(photoLabel, 0, Qt::AlignTop);
    cardLayout->addWidget(details, 1);

    return card;
}

void etudiantsPage::clearCards()
{
    QLayoutItem *item = nullptr;

    // on vide proprement le layout pour reconstruire la liste a chaque reload
    while ((item = cardsLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

void etudiantsPage::loadEtudiants()
{
    clearCards();
    scrollArea->show();
    emptyLabel->hide();

    scolariteManager manager;
    vector<etudiantDetailsData> rows;
    if (!manager.getAllEtudiantsDetails(rows)) {
        scrollArea->hide();
        emptyLabel->setText("Impossible de charger les etudiants (connexion base de donnees).");
        emptyLabel->show();
        return;
    }

    if (rows.empty()) {
        scrollArea->hide();
        emptyLabel->setText("Aucun etudiant trouve.");
        emptyLabel->show();
        return;
    }

    // on cree une carte par etudiant pour une vue plus lisible qu'un tableau classique
    for (int i = 0; i < static_cast<int>(rows.size()); ++i) {
        cardsLayout->addWidget(buildStudentCard(
            rows[i].photoUrl.trimmed(),
            rows[i].nom,
            rows[i].prenom,
            rows[i].numCarte,
            rows[i].ville,
            rows[i].cours
        ));
    }

    cardsLayout->addStretch();
    updateResponsiveUi();
}
