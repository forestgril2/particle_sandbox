#ifndef LennardNet_H
#define LennardNet_H

#include <QtGui/QMainWindow>

class Vector
{
  Vector(QPointF p) : p_(p) {};
  QPointF p_;
  
public:
  void paint(QPainter* painter);
};

class LennardNet : public QMainWindow
{
  Q_OBJECT

  void paintPointA(QPainter* painter);
  virtual void paintEvent(QPaintEvent* pE);
    void initAction();
    void paintPoints(QPainter* painter);

  Vector pointA;
  Vector pointB;

public:
  LennardNet();
  virtual ~LennardNet();
};

#endif // LennardNet_H
