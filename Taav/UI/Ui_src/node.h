#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <qtreewidget.h>

class node : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    explicit node();
    void setRootTree(QTreeWidget * root);
    void appendToParent(QTreeWidgetItem *parentTree);
    virtual void appendChild(node *childNode);
    virtual void appendChild(node *childNode,int);
    virtual QString getData();
    virtual void pSetData(QString);
    virtual QString getName();
    QWidget *getWidget();


    QWidget *wid;
    QTreeWidget * rootTree;
    QList<node *> childNodes;

signals:

public slots:
};

#endif // NODE_H
