#include "pdockwidget.h"

PdockWidget::PdockWidget(const QString & title, QWidget * parent) : QDockWidget(title,parent)
{

}

void PdockWidget::resizeEvent(QResizeEvent *event)
{
    emit sizeChanged();
}


