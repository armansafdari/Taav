#include <textnode.h>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QHBoxLayout>



textNode::textNode(QString s)
{
    wid = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    l1 = new QLabel(s);
    QSpacerItem *sp = new QSpacerItem(10000,0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Minimum);

    QFont f( "Arial", 9);
    l1->setFont( f);

    l1->setSizePolicy(sizePolicy);
    layout->addWidget(l1);
    layout->addSpacerItem(sp);
    layout->setMargin(2);
    wid->setLayout(layout);
}


void textNode::appendChild(node *childNode)
{

    childNode->setRootTree(rootTree);
    this->addChild(childNode);
    rootTree->setItemWidget(childNode,0,childNode->getWidget());
    childNodes.append(childNode);

}

QString textNode::getData()
{
    return  "";
}

QString textNode::getName()
{
    QString Str = l1->text();
    Str.remove(' ');
    Str.remove(':');
    Str.replace('+','P');
    Str.replace('-','N');
    return Str;
}

void textNode::pSetData(QString s)
{
    l1->setText(s);
}


