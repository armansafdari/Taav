#ifndef PATHNODE_H
#define PATHNODE_H

#include <QObject>
#include <node.h>
#include <QWidget>
#include <qstring.h>
#include <QWidget>
#include <qtreewidget.h>
#include <QLabel>

class pathNode : public node
{
    Q_OBJECT
public:
    QLineEdit *lineEdit;

    explicit pathNode(QWidget *parent,QString s1,QString s2);
    void appendChild(node *childNode);
    void openFileDialog();
    QString getData();
    QString getName();
    void pSetData(QString s);

private:
    QLabel *l1;
    QWidget *parentWidget;

signals:

public slots:
};

#endif // PATHNODE_H
