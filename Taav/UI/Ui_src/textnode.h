#ifndef TEXTNODE_H
#define TEXTNODE_H
#include <qtreewidget.h>
#include <QObject>
#include <choosenode.h>
#include <node.h>
#include <QLabel>

class textNode : public node
{
    Q_OBJECT
public:
    explicit textNode(QString s);
    void appendChild(node *childNode);
    QString getData();
    QString getName();
//    void getWidget();

    void pSetData(QString);

private:
    QLabel *l1;
//    QWidget *wid;
signals:

public slots:
};

#endif // TEXTNODE_H
