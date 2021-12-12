#ifndef PDOCKWIDGET_H
#define PDOCKWIDGET_H

#include <QObject>
#include <qdockwidget.h>

class PdockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit PdockWidget(const QString & title, QWidget * parent = nullptr);
    void resizeEvent(QResizeEvent * event);

signals:

    void sizeChanged();


public slots:
};

#endif // PDOCKWIDGET_H
