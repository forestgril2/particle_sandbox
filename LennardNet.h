#ifndef LennardNet_H
#define LennardNet_H

#include <QtGui/QMainWindow>

class Pixel
{
  QPointF p_;
  
public:
  void operator=(QPoint p) {p_ = p;};
  Pixel() : p_(QPointF(0,0)) {};
  Pixel(QPointF p) : p_(p) {};
  void paint(QPainter* painter);
};

class LennardNet : public QMainWindow
{
  Q_OBJECT

  void paintPointA(QPainter* painter);
  virtual void paintEvent(QPaintEvent* pE);
  void initAction();
  void paintPoints(QPainter* painter);

  Pixel *pixels;

public:
  LennardNet();
  virtual ~LennardNet();
};

#endif // LennardNet_H
