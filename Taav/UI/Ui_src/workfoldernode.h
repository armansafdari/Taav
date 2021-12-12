#ifndef WORKFOLDERNODE_H
#define WORKFOLDERNODE_H

#include <QObject>
#include <node.h>
#include <QWidget>
#include <qstring.h>
#include <QWidget>
#include <qtreewidget.h>
#include <QLabel>

class workFolderNode: public node
{
    Q_OBJECT
public:
    QLineEdit *lineEdit;

    explicit workFolderNode(QString s1,QString s2);
    void appendChild(node *childNode);

    QString getData();
    QString getName();
    void pSetData(QString s);
    QLineEdit* getLineEdit();
    QLineEdit* setLineEdit(QString str);
private:
    QLabel *l1;

signals:

public slots:
};
#endif // WORKFOLDERNODE_H
