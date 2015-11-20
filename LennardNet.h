#ifndef LennardNet_H
#define LennardNet_H

#include <QtGui/QMainWindow>

using namespace std;

typedef QPointF Point2D;
typedef QPainter Painter;
typedef QColor Color;

class Pixel
{
  Point2D p_;
  Color c_;
  
public:
  void operator=(Point2D p) {p_ = p;};
  Pixel() : p_(Point2D(0,0)) {};
  Pixel(Point2D p) : p_(p) {};
  void paint(Painter* painter);
  void setColor(Color c) {c_ = c;};
  Color color() {return c_;};
};

class LennardNet : public QMainWindow
{
  Q_OBJECT

  virtual void paintEvent(QPaintEvent* pE);
  void initAction();
  void paintPoints(Painter* painter);

  vector<Pixel> pixels;

public:
  LennardNet();
  virtual ~LennardNet();
};

#endif // LennardNet_H
