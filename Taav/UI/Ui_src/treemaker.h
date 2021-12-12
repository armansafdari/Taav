#ifndef TREEMAKER_H
#define TREEMAKER_H

#include <QObject>
#include <qtreewidget.h>
#include <QDomDocument>
#include <textnode.h>
#include <fieldnode.h>

class treeMaker : public QObject
{
    Q_OBJECT
public:
    explicit treeMaker(QWidget *parent = nullptr);
    textNode * treeEnv(QTreeWidget *tree);
    textNode * treeMaterials(QTreeWidget *tree);
    textNode * treeGeomtry(QTreeWidget *tree);
    textNode * treeSimulation(QTreeWidget *tree);
    textNode * treePost(QTreeWidget *tree);

    QDomElement docElem;
    textNode * nodeMaker(QTreeWidget *tree, QDomNode thisTreeNode);
    void startMaking();

    fieldNode *FieldNodeEditStart;
    fieldNode *FieldNodeEditFinish;
    fieldNode *FieldNodeEditFrequency;
    QLineEdit *lineEditStart;
    QLineEdit *lineEditFinish;
    QLineEdit *lineEditFrequency;
    QLineEdit *WorkingDir;
//    QTreeWidget *treeWidget;

    QVector<QTreeWidget *> WidgetTree;

private:
    QWidget *mw;

signals:


public slots:
    void setEnable(bool check);
};

#endif // TREEMAKER_H
