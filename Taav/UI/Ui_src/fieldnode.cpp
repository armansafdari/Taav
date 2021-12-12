#include <fieldnode.h>
#include "QHBoxLayout"

#include <QLineEdit>
#include <QSpacerItem>
#include <QDebug>
#include <QSizePolicy>

fieldNode::fieldNode(QString s1, QString i1, QString s2,QString type,uint fieldtype)
{
        wid = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout;

        l1 = new QLabel(s1);
        lineEdit = new QLineEdit(i1);

        lineEdit->setPlaceholderText(i1);

        if(!QString::compare(type,"int"))
        {
            lineEdit->setValidator( new QIntValidator(0, 1000000000, this) );
        }
        else if(!QString::compare(type,"float"))
        {
            lineEdit->setValidator( new QDoubleValidator(0, 1000000000, 16, this) );
        }


        QLabel *l2 = new QLabel(s2);
        QSpacerItem *sp = new QSpacerItem(10000,0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        if (fieldtype==1){
            QFont f( "Arial", 9);
            l1->setFont( f);
            l2->setFont( f);
            l1->setFixedSize(100,20);

            QFont f1( "Arial", 8);
            lineEdit->setFont(f1);
            lineEdit->setFixedSize(QSize(200,20));
        }
        //// short
        if (fieldtype==2){
            QFont f( "Arial", 9);
            l1->setFont( f);
            l2->setFont( f);
            l1->setFixedSize(50,20);

            QFont f1( "Arial", 8);
            lineEdit->setFont(f1);
            lineEdit->setFixedSize(QSize(50,20));
        }
        /// long
        if (fieldtype==3){
            QFont f( "Arial", 9);
            l1->setFont( f);
            l2->setFont( f);
            l1->setFixedSize(120,20);

            QFont f1( "Arial", 8);
            lineEdit->setFont(f1);
            lineEdit->setFixedSize(QSize(50,20));
        }
        //// short short
        if (fieldtype==4){
            QFont f( "Arial", 9);
            l1->setFont( f);
            l2->setFont( f);
            l1->setFixedSize(20,20);

            QFont f1( "Arial", 8);
            lineEdit->setFont(f1);
            lineEdit->setFixedSize(QSize(250,20));
        }

        QSizePolicy sizePolicy;
        sizePolicy.setHorizontalPolicy(QSizePolicy::Minimum);
        l1->setSizePolicy(sizePolicy);
        l2->setSizePolicy(sizePolicy);

        layout->addWidget(l1);
        layout->addWidget(lineEdit);
        layout->addWidget(l2);
        layout->addSpacerItem(sp);


        layout->setMargin(2);
        wid->setLayout(layout);


}

void fieldNode::appendChild(node *childNode)
{
    childNode->setRootTree(rootTree);
    this->addChild(childNode);
    rootTree->setItemWidget(childNode,0,childNode->getWidget());
    childNodes.append(childNode);
}

QString fieldNode::getData()
{
    QString Str = lineEdit->text();
//    qDebug() <<  Str ;
//    Str.remove(' ');
//    Str.replace("&gt;",">");
//    qDebug() <<  Str ;
//    Str.replace("<","&lt;");
    return  Str;
}

QString fieldNode::getName()
{
    QString Str = l1->text();
    Str.remove(' ');
    Str.remove(':');
    return Str;
}

void fieldNode::pSetData(QString s)
{
    lineEdit->setText(s);
}


QLineEdit* fieldNode::getLineEdit()
{
    return lineEdit;
}
