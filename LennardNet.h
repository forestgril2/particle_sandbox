#ifndef LennardNet_H
#define LennardNet_H

#include <QtGui/QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QLabel>
#include <QPushButton>

using namespace std;

typedef QPointF Point2D;
typedef QPainter Painter;
typedef QColor Color;
typedef QRectF Rectangle;

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
  void addPixelsSquareNet(double squareSide, Rectangle R, Color color);
  void proceedInTime(double timeDiff);
  void initTimers();
  void initLabel();
  Point2D calculateForceForPoint(Point2D pos);
  void checkInformMarkerPixelTime();
  void initMarkerPixel();
  void initStartButton();

  Pixel markerPixel;
  vector<Pixel> pixels;
  QElapsedTimer nanoTimerTotal;
  QElapsedTimer nanoTimer;
  QLabel* label;
  QTimer *canvasUpdateTimer;
  bool startedUpdates;
  
private slots:
  void startTimers();
  void startButton(const char* arg1);

public:
  LennardNet();
  virtual ~LennardNet();
};

#endif // LennardNet_H
