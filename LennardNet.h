#ifndef LennardNet_H
#define LennardNet_H

#include <QtGui/QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QLabel>

using namespace std;

typedef QPointF Point2D;
typedef QPainter Painter;
typedef QColor Color;

class Pixel //TODO: move to separate header
{
  Point2D pos_;
  Point2D speed_; //per second
  Color color_;
  
public:
  Pixel() : pos_(Point2D(0,0)), speed_(Point2D(0,0)) {};
  Pixel(Point2D p) : pos_(p), speed_(Point2D(0,0)) {};
  Pixel(Point2D p, Point2D s) : pos_(p), speed_(s) {};
  
  void operator=(Point2D p) {pos_ = p;};
  
  void setSpeed(double vx, double vy) {speed_.setX(vx); speed_.setY(vy);};
  void setColor(Color c) {color_ = c;};
  
  Point2D speed() {return speed_;};
  Point2D pos() {return pos_;};
  Color color() {return color_;};
  
  void paint(Painter* painter);
  void proceedInTime(double timeDiff, Point2D acceleration);
};

class LennardNet : public QMainWindow
{
  Q_OBJECT

  virtual void paintEvent(QPaintEvent* pE);
  void initAction();
  void paintPoints(Painter* painter);
  void initPixels();
  void proceedInTime(double arg1);
  void initUpdateInterval();
  void initLabel();
  Point2D calculateForceForPoint(Point2D pos);
  void checkInformMarkerPixelTime();
  Point2D gravityForce(Point2D vector);
  Point2D LJForce(Point2D vector);

  Pixel markerPixel;
  vector<Pixel> pixels;
  QElapsedTimer nanoTimerTotal;
  QElapsedTimer nanoTimer;
  QLabel* label;

public:
  LennardNet();
  virtual ~LennardNet();
};

#endif // LennardNet_H
