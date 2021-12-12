#include "treemaker.h"
#include <qpushbutton.h>
#include <QStringList>

#include <QFile>
#include <QDebug>
#include <QStack>

#include <choosenode.h>
#include <fieldnode.h>
#include <typeinfo>
#include <pathnode.h>
#include <workfoldernode.h>

treeMaker::treeMaker(QWidget *parent) : QObject(parent)
{
    mw = parent;

}

textNode * treeMaker::treeEnv(QTreeWidget *tree)
{
    return nodeMaker(tree,docElem.childNodes().at(0));


}

textNode * treeMaker::treeMaterials(QTreeWidget *tree)
{
    return nodeMaker(tree,docElem.childNodes().at(1));
}

textNode * treeMaker::treeGeomtry(QTreeWidget *tree)
{
    return nodeMaker(tree,docElem.childNodes().at(2));
}

textNode * treeMaker::treeSimulation(QTreeWidget *tree)
{
    return nodeMaker(tree,docElem.childNodes().at(3));
}

textNode * treeMaker::treePost(QTreeWidget *tree)
{
    return nodeMaker(tree,docElem.childNodes().at(4));
}

textNode * treeMaker::nodeMaker(QTreeWidget *tree,QDomNode thisTreeNode)
{

    tree->setColumnCount(1);
    tree->setHeaderHidden(true);


    textNode *rootText = new textNode(thisTreeNode.toElement().attribute("name"));
    tree->insertTopLevelItem(0,rootText);

    //rootText->setText(0,thisTreeNode.toElement().attribute("name"));
    rootText->setHidden(false);
    rootText->setExpanded(true);
    rootText->setRootTree(tree);



    tree->setItemWidget(rootText,0,rootText->getWidget());

    WidgetTree.append(tree);

//    setEnable(0);

//    tree->setEnabled(false);






    //________________________________________________________________________________________
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //----------------------------------------------------------------------------------------


    QStack<QDomNode> stackXml;
    QDomNode XMLnode ;

    QStack<node*> stackNode;
    node *parentNode = nullptr;
    QStack<chooseNode*> lastchooseNode ;
    chooseNode *laschooseNodeCurrent = nullptr;



        qDebug() << thisTreeNode.toElement().attribute("name");
        XMLnode = thisTreeNode; //docElem.firstChild();
        stackXml.push(XMLnode);
        stackNode.push(rootText);
        int test =0;

        while(!stackXml.isEmpty())
        {
            XMLnode = stackXml.pop();
            parentNode = stackNode.pop();
//            qDebug() << typeid (*parentNode).name();

//            if(!strncmp(typeid (*parentNode).name(),"class chooseNode",30)) msvc
            if(!strncmp(typeid (*parentNode).name(),"10chooseNode",30))
               laschooseNodeCurrent = lastchooseNode.pop();

            for(int i =0; i <XMLnode.childNodes().count()  ;i++)
            {


                QDomNode workerXml = XMLnode.childNodes().at(i);
//                qDebug() << workerXml.toElement().attribute("name");
//                qDebug() << workerXml.toElement().tagName();


            /////////////////////////////////
                if (workerXml.toElement().attribute("name")=="Fluid name:"|| test==1){
                    test = 1;
                    qDebug()  << stackXml.size();
                    qDebug() << workerXml.toElement().tagName();
                    qDebug() << "XMLnode.childNodes().count() = " << XMLnode.childNodes().count();
}

                //////////////////////////////////


                if(workerXml.toElement().tagName() == "text")
                {
                    textNode *textnode = new textNode(workerXml.toElement().attribute("name"));

                    if(!strncmp(typeid (*parentNode).name(),"10chooseNode",30))
                    {
                        auto x = workerXml.toElement().attribute("item").toInt();
                        parentNode->appendChild(textnode,x);
                        parentNode->setExpanded(true);

                    }
                    else{
                        parentNode->appendChild(textnode);
                        parentNode->setExpanded(true);
                    }
                    stackNode.push(textnode);
                    stackXml.push(XMLnode.childNodes().at(i));

                }
                else if(workerXml.toElement().tagName() == "field"|| workerXml.toElement().tagName() == "shortfield"
                        ||workerXml.toElement().tagName() == "longfield"||workerXml.toElement().tagName() == "shortshortfield")
                {
                    uint fieldtype = 1;
                    if (workerXml.toElement().tagName() == "shortfield") {fieldtype=2;}
                    if (workerXml.toElement().tagName() == "longfield") {fieldtype=3;}
                    if (workerXml.toElement().tagName() == "shortshortfield") {fieldtype=4;}

                    fieldNode *fieldnode = new fieldNode(workerXml.toElement().attribute("name"),
                                             workerXml.toElement().attribute("input"),
                                             workerXml.toElement().attribute("unit"),
                                             workerXml.toElement().attribute("type"),
                                             fieldtype);

                    //----------------------------get neded fileds------------------------

                    if(workerXml.toElement().attribute("name")=="Start:") {
                        qDebug() <<"found start:";
                        lineEditStart = fieldnode->getLineEdit();
                    }
                    else if (workerXml.toElement().attribute("name")=="Finish:") {
                        qDebug() <<"found stop:";
                        lineEditFinish = fieldnode->getLineEdit();
                    }
                    else if (workerXml.toElement().attribute("name")=="Image frequency:") {
                        qDebug() <<"found frequency:";
                        lineEditFrequency = fieldnode->getLineEdit();
                    }

                    //-------------------------------------------------------------------

                    if(!strncmp(typeid (*parentNode).name(),"10chooseNode",30))
                    {
                        auto x = workerXml.toElement().attribute("item").toInt();
                        parentNode->appendChild(fieldnode,x);
                        parentNode->setExpanded(true);
                    }
                    else{
                        parentNode->appendChild(fieldnode);
                        parentNode->setExpanded(true);

                    }

                    stackNode.push(fieldnode);
                    stackXml.push(XMLnode.childNodes().at(i));

                }



                else if(workerXml.toElement().tagName() == "path")
                {
                    pathNode *pathnode = new pathNode(mw,workerXml.toElement().attribute("name"),
                                                      workerXml.toElement().attribute("path"));


                    if(!strncmp(typeid (*parentNode).name(),"10chooseNode",30))
                    {
                        auto x = workerXml.toElement().attribute("item").toInt();
                        parentNode->appendChild(pathnode,x);
                        parentNode->setExpanded(true);
                    }
                    else{
                        parentNode->appendChild(pathnode);
                        parentNode->setExpanded(true);
                    }

                    stackNode.push(pathnode);
                    stackXml.push(XMLnode.childNodes().at(i));
                }
                //----------------------------------------------------------------------------
                else if(workerXml.toElement().tagName() == "uneditable")
                {
                    workFolderNode *workfolderNode = new workFolderNode(workerXml.toElement().attribute("name"),
                                                      workerXml.toElement().attribute("path"));



                    if (workerXml.toElement().attribute("name")=="Work Folder:") {
                        qDebug() <<"found Path:";
                        WorkingDir = workfolderNode->getLineEdit();
                    }




                    if(!strncmp(typeid (*parentNode).name(),"10chooseNode",30))
                    {
                        auto x = workerXml.toElement().attribute("item").toInt();
                        parentNode->appendChild(workfolderNode,x);
                    }
                    else
                        parentNode->appendChild(workfolderNode);

                    stackNode.push(workfolderNode);
                    stackXml.push(XMLnode.childNodes().at(i));
                }

                else if(workerXml.toElement().tagName() == "choose")
                {
                    chooseNode *choosenode = new chooseNode(workerXml.toElement().attribute("name"),{});
                    lastchooseNode.push(choosenode);

                    if(!strncmp(typeid (*parentNode).name(),"10chooseNode",30))
                    {
                        auto x = workerXml.toElement().attribute("item").toInt();
                        parentNode->appendChild(choosenode,x);
                        parentNode->setExpanded(true);
                    }
                    else
                    {
                        parentNode->appendChild(choosenode);
                        parentNode->setExpanded(true);
                    }


                    stackNode.push(choosenode);
                    stackXml.push(XMLnode.childNodes().at(i));

                }
                else if(workerXml.toElement().tagName() == "item")
                {
                    laschooseNodeCurrent->appendItem(workerXml.toElement().attribute("name"));
                }
            }

        }


    //________________________________________________________________________________________

        return rootText;
}

void treeMaker::startMaking()
{
    QFile file("../../Ui_src/data.xml");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cant read file";
        return;
    }

    QDomDocument doc;

    if (!doc.setContent(&file)) {
        qDebug() << "cant setContetn";
        file.close();
        return;
    }

    docElem = doc.documentElement();

    if(docElem.tagName()=="root")
    {
        qDebug() << "root found";
    }



//    QFile savefile("../../data_savefile.xml");
//    if (!savefile.open(QIODevice::WriteOnly | QIODevice::Text))
//    {
//        qDebug() << "Cant read file";
//        return;
//    }
//    else {

//        QTextStream stream( &savefile );
//        stream << doc.toString();
//        savefile.close();
//    }
}

void treeMaker::setEnable(bool check){
    if (check){
        qDebug() << "setEnable is true ";
        for(int i=0;i<WidgetTree.size();i++){
          WidgetTree[i]->setEnabled(true);
        }
    }
    else {
        for(int i=0;i<WidgetTree.size();i++){
          WidgetTree[i]->setEnabled(false);
        }
    }

}

