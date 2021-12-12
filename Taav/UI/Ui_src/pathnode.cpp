#include "pathnode.h"
#include "QHBoxLayout"

#include <QLineEdit>
#include <QSpacerItem>
#include <QPushButton>
#include <QSizePolicy>
#include <QFileDialog>


pathNode::pathNode(QWidget *parent,QString s1,QString s2)
{
    parentWidget = parent;
    wid = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    l1 = new QLabel(s1);
    QPushButton *bt = new QPushButton("...");

    connect(bt,&QPushButton::clicked,this,&pathNode::openFileDialog);


    lineEdit = new QLineEdit(s2);
    //QSpacerItem *sp = new QSpacerItem(0,0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Minimum);

    QFont f( "Arial", 9);
    l1->setFont( f);

    QFont font = bt->font();
    font.setPointSize(6);
    bt->setFont(font);
    bt->setSizePolicy(sizePolicy);
    bt->setFixedSize(20,22);


    l1->setFixedSize(80,20);
    QSizePolicy sizePolicy2;
    sizePolicy2.setHorizontalPolicy(QSizePolicy::Expanding);
    lineEdit->setSizePolicy(sizePolicy2);


    layout->addWidget(l1);
    layout->addWidget(lineEdit);
    layout->addWidget(bt);

    //layout->addSpacerItem(sp);

    layout->setMargin(2);
    wid->setLayout(layout);

}
void pathNode::appendChild(node *childNode)
{
    childNode->setRootTree(rootTree);
    this->addChild(childNode);
    rootTree->setItemWidget(childNode,0,childNode->getWidget());
    childNodes.append(childNode);
}

void pathNode::openFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(parentWidget,tr("Open File"));
    if(fileName.isEmpty()==0){
    lineEdit->setText(fileName);
    }
}

QString pathNode::getData()
{
    return  lineEdit->text();
}

QString pathNode::getName()
{
    QString Str = l1->text();
    Str.remove(' ');
    Str.remove(':');
    return Str;
}

void pathNode::pSetData(QString s)
{
    lineEdit->setText(s);
}
