#include "workfoldernode.h"
#include "QHBoxLayout"

#include <QLineEdit>
#include <QSpacerItem>
#include <QPushButton>
#include <QSizePolicy>
#include <QFileDialog>

workFolderNode::workFolderNode(QString s1,QString s2)
{
    wid = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    l1 = new QLabel(s1);

    lineEdit = new QLineEdit(s2);
    lineEdit->setReadOnly(true);

    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Minimum);


    QFont f( "Arial", 8);
    l1->setFont( f);


    l1->setFixedSize(65,20);
    QSizePolicy sizePolicy2;
    sizePolicy2.setHorizontalPolicy(QSizePolicy::Expanding);
    lineEdit->setSizePolicy(sizePolicy2);
    lineEdit->setFont(QFont ("Arial", 8));

    layout->addWidget(l1);
    layout->addWidget(lineEdit);


    //layout->addSpacerItem(sp);

    layout->setMargin(2);
    wid->setLayout(layout);
}
void workFolderNode::appendChild(node *childNode)
{
    childNode->setRootTree(rootTree);
    this->addChild(childNode);
    rootTree->setItemWidget(childNode,0,childNode->getWidget());
    childNodes.append(childNode);
}



QString workFolderNode::getData()
{
    return  lineEdit->text();
}

QString workFolderNode::getName()
{
    QString Str = l1->text();
    Str.remove(' ');
    Str.remove(':');
    Str.replace('+','P');
    Str.replace('-','N');
    return Str;
}

void workFolderNode::pSetData(QString s)
{
    lineEdit->setText(s);
}

QLineEdit* workFolderNode::getLineEdit()
{
    return lineEdit;
}

QLineEdit* workFolderNode::setLineEdit(QString str)
{
    lineEdit->setText(str);
}
