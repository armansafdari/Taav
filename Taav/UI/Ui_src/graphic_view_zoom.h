#ifndef GRAPHIC_VIEW_ZOOM_H
#define GRAPHIC_VIEW_ZOOM_H


#include <QObject>
#include <QGraphicsView>
#include <QMainWindow>


class Graphics_view_zoom : public QObject {
  Q_OBJECT
public:
  Graphics_view_zoom(QGraphicsView* view);
//  Graphics_view_zoom(QGraphicsView* view, QObject *keyObj, QObject *mouseObj, QObject *parent = NULL);

//  MyFilter(&w, w.viewport(), &w);
  void gentle_zoom(double factor);
  void set_modifiers(Qt::KeyboardModifiers modifiers);
  void set_zoom_factor_base(double value);

//  QObject *keyObj;
//  QObject *mouseObj;

//public:
//    void MyFilter(QObject* object, QEvent* event);

private:
  QGraphicsView* _view;
  Qt::KeyboardModifiers _modifiers;
  double _zoom_factor_base;
  QPointF target_scene_pos, target_viewport_pos;
  bool eventFilter(QObject* object, QEvent* event);

signals:
  void zoomed();


};

#endif // GRAPHIC_VIEW_ZOOM_H
