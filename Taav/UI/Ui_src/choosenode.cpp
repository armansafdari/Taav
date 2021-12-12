#include "choosenode.h"
#include "QHBoxLayout"

#include <QDebug>
#include <QSizePolicy>


chooseNode::chooseNode(QString s1, QStringList list)
{
    wid = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    l1 = new QLabel(s1);
    combo = new QComboBox;

    QFont f( "Arial", 9);
    l1->setFont( f);

    QFont font = combo->font();
    font.setPointSize(8);
    combo->setFont(font);

    combo->addItems(list);

    connect(combo,QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,&chooseNode::comboChanged);
    QSpacerItem *sp = new QSpacerItem(10000,0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Minimum);

    l1->setSizePolicy(sizePolicy);
    combo->setSizePolicy(sizePolicy);

    layout->addWidget(l1);
    layout->addWidget(combo);
    layout->addSpacerItem(sp);
    layout->setMargin(2);
    wid->setLayout(layout);


    vector =new QVector< QVector< node *> >;
    for(int i = 0; i< 10; i++)
    {
        QVector<node*> fill_vector;
        vector->append(fill_vector);
    }



}



void chooseNode::appendChild(node *childNode,int selectId)
{



    childNode->setRootTree(rootTree);


    this->addChild(childNode);
    rootTree->setItemWidget(childNode,0,childNode->getWidget());
    childNode->setHidden(true);
    vector[0][selectId].append(childNode);


    for(int i = 0;i<vector->at(0).size();i++)
    {

        node *sampleNode = vector->at(0).at(i);
        sampleNode->setHidden(false);

    }
    childNodes.append(childNode);


}

void chooseNode::appendItem(QString s)
{
    combo->addItem(s);
}

QString chooseNode::getData()
{
    return combo->itemText(currentComboIndex);
}

QString chooseNode::getName()
{
    QString Str = l1->text();
    Str.remove(' ');
    Str.remove(':');
    Str.replace('+','P');
    Str.replace('-','N');
    return Str;
}

void chooseNode::pSetData(QString s)
{
    for(int i = 0;i<combo->count();i++)
    {
        QString ss =  combo->itemText(i);
        if(!QString::compare(ss,s,Qt::CaseInsensitive))
        {
            //qDebug() << "found:" + s;
            combo->setCurrentIndex(i);
            break;
        }
    }
}


void chooseNode::comboChanged(int index)
{

    currentComboIndex = index;
    //qDebug() << "combo changed:" << index;
    for(int j = 0;j<10;j++)
    {
        for(int i = 0;i<vector->at(j).size();i++)
        {
            node *sampleNode = vector->at(j).at(i);
            sampleNode->setHidden(true);

        }
    }

    for(int i = 0;i<vector->at(index).size();i++)
    {
        node *sampleNode = vector->at(index).at(i);
        sampleNode->setHidden(false);

    }


}
