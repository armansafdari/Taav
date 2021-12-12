#include "node.h"


node::node()
{

}


void node::setRootTree(QTreeWidget *root)
{
    rootTree = root;
}

void node::appendToParent(QTreeWidgetItem *parentTree)
{
    parentTree->addChild(this);
    rootTree->setItemWidget(this,0,wid);
}

void node::appendChild(node *childNode)
{

}

void node::appendChild(node *, int)
{

}

QString node::getData()
{

}

void node::pSetData(QString)
{

}

QString node::getName()
{

}






QWidget *node::getWidget()
{
    return wid;
}
