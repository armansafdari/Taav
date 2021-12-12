#include "graphic_view_zoom.h"
#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>
#include <qmath.h>
#include <QKeyEvent>
#include <QDebug>


Graphics_view_zoom::Graphics_view_zoom(QGraphicsView* view)
  : QObject(view), _view(view)
{
  _view->viewport()->installEventFilter(this);
  _view->setMouseTracking(true);
  _modifiers = Qt::ControlModifier;
  _zoom_factor_base = 1.0015;
}


//Graphics_view_zoom::Graphics_view_zoom(QGraphicsView* view,QObject *keyObj, QObject *mouseObj, QObject *parent /*= NULL*/ ) :
//   QObject(view), _view(view), keyObj(keyObj), mouseObj(mouseObj){

//  _view->viewport()->installEventFilter(this);
//  _view->setMouseTracking(true);
//  _modifiers = Qt::ControlModifier;
//  _zoom_factor_base = 1.0015;
//}

//Graphics_view_zoom::MyFilter(QObject *obj, QEvent *e){

//    if (obj == keyObj && e->type() == QEvent::KeyPress)
//    {
//        qDebug()<<"Key Event recieved by MyFilter";
//    }
//    else if (obj == mouseObj && e->type() == QEvent::MouseButtonPress)
//    {
//        qDebug()<<"Mouse Event recieved by MyFilter";
//    }
//}

void Graphics_view_zoom::gentle_zoom(double factor) {
  _view->scale(factor, factor);
  _view->centerOn(target_scene_pos);
  QPointF delta_viewport_pos = target_viewport_pos - QPointF(_view->viewport()->width() / 2.0,
                                                             _view->viewport()->height() / 2.0);
  QPointF viewport_center = _view->mapFromScene(target_scene_pos) - delta_viewport_pos;
  _view->centerOn(_view->mapToScene(viewport_center.toPoint()));
  emit zoomed();
}

void Graphics_view_zoom::set_modifiers(Qt::KeyboardModifiers modifiers) {
  _modifiers = modifiers;

}

void Graphics_view_zoom::set_zoom_factor_base(double value) {
  _zoom_factor_base = value;
}

bool Graphics_view_zoom::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::MouseMove) {
    QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
    QPointF delta = target_viewport_pos - mouse_event->pos();
    if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5) {
      target_viewport_pos = mouse_event->pos();
      target_scene_pos = _view->mapToScene(mouse_event->pos());
    }
  } else if (event->type() == QEvent::Wheel) {
    QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
    if (QApplication::keyboardModifiers() == _modifiers) {
      if (wheel_event->orientation() == Qt::Vertical) {
        double angle = wheel_event->angleDelta().y();
        double factor = qPow(_zoom_factor_base, angle);
        gentle_zoom(factor);
        return true;
      }
    }
  }
  if (event->type() == QEvent::KeyPress)
      qDebug() << event ;
//      return QObject::eventFilter(obj, event);

//  QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//  keyEvent->
//  switch(keyEventkey()) {
//  case Qt::Key_Space: { qDebug() << "Space"; break; }
//  case Qt::Key_Left: { qDebug() << "Left"; break; }
//  case Qt::Key_Right: { qDebug() << "Right"; break; }
//  default: { qDebug() << "Unhandled"; break; }
//  }

//  qDebug() << event ;
  Q_UNUSED(object)
  return false;
}
