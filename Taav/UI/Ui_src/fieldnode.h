#ifndef FIELDNODE_H
#define FIELDNODE_H

#include <QObject>
#include <node.h>
#include <QWidget>
#include <qstring.h>
#include <QWidget>
#include <qtreewidget.h>
#include <QLabel>

class fieldNode : public node
{
    Q_OBJECT
public:
    explicit fieldNode(QString s1, QString i1, QString s2,QString type,uint fieldtype=1);
    void appendChild(node *childNode);

    QString getData();
    QString getName();
    void pSetData(QString s);
    QLineEdit* getLineEdit();

private:
    QLabel *l1;
    QLineEdit *lineEdit;
    uint fieldtype;
signals:

public slots:
};


#endif // FIELDNODE_H
