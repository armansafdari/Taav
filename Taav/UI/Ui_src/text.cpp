#include "text.h"
#include <QSpacerItem>
#include <QSizePolicy>
#include <QHBoxLayout>
#include <QLabel>
text::text(QTreeWidget * r,QTreeWidgetItem *parentTree, QString s)
{
    tree = r;
    parentTree->addChild(this);
    QWidget *wid = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    QLabel *l1 = new QLabel(s);
    QSpacerItem *sp = new QSpacerItem(10000,0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Minimum);
    l1->setSizePolicy(sizePolicy);
    layout->addWidget(l1);
    layout->addSpacerItem(sp);
    layout->setMargin(2);
    wid->setLayout(layout);



    tree->setItemWidget(this,0,wid);
}


