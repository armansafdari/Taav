#ifndef TEXT_H
#define TEXT_H

#include <QObject>
#include <qtreewidget.h>
#include <qstring.h>
#include <QWidget>
class text : public QObject,public QTreeWidgetItem
{
    Q_OBJECT
public:
    explicit text(QTreeWidget * r,QTreeWidgetItem*,QString s);
//    QTreeWidgetItem * make(QTreeWidgetItem*,QString s);
    QTreeWidget * tree;
signals:

public slots:
};

#endif // TEXT_H
