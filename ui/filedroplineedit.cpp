#include "filedroplineedit.h"

#include <QDragEnterEvent>
#include <QMimeData>
#include <QUrl>

fileDropLineEdit::fileDropLineEdit(QWidget *parent) : QLineEdit(parent)
{
    setAcceptDrops(true);
}

void fileDropLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    // on accepte uniquement les drops qui contiennent des urls
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
        return;
    }

    QLineEdit::dragEnterEvent(event);
}

void fileDropLineEdit::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        const QList<QUrl> urls = event->mimeData()->urls();

        // on prend juste le premier fichier local et on met son chemin dans le line edit
        if (!urls.isEmpty() && urls.first().isLocalFile()) {
            setText(urls.first().toLocalFile());
            event->acceptProposedAction();
            return;
        }
    }

    QLineEdit::dropEvent(event);
}
