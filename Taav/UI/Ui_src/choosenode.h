#ifndef CHOOSENODE_H
#define CHOOSENODE_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QList>
#include <qtreewidget.h>
#include <QComboBox>
#include <node.h>
#include <QLabel>

class chooseNode : public node
{
    Q_OBJECT
public:
    explicit chooseNode(QString s1, QStringList list);
    void appendChild(node *childNode, int selectId);
    QVector <QVector<node*>> *vector;
    QStringList Items;
    void appendItem(QString s);
    QString getData();
    QString getName();
    void pSetData(QString s);


private:
    QComboBox *combo;
    int currentComboIndex;
    QLabel *l1;

signals:

public slots:
   void comboChanged(int index);
};

#endif // CHOOSENODE_H
