#ifndef LennardNet_H
#define LennardNet_H

#include <QtGui/QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QLabel>
#include <QPushButton>

#include "Pixel.h"

using namespace std;

class LennardNet : public QMainWindow
{
  Q_OBJECT

  virtual void paintEvent(QPaintEvent* pE);
  void initAction();
  void paintPoints(Painter* painter);
  void addPixelsSquareNet(double squareSide, Rectangle R, Color color);
  void proceedInTime(double timeDiff);
  void initCanvasUpdateTimer();
  void initLabel();
  Point2D calculateForceForPoint(Point2D pos);
  void printCalculationTime();
  void initMarkerPixel();
  void initStartStopButton();

  vector<Pixel> pixels;
  QElapsedTimer calculationNanoTimer;
  QElapsedTimer paintEventNanoTimer;
  QLabel* label;
  QTimer* canvasUpdateTimer;
  QPushButton* startStopButton;
  bool startedUpdates;
  
private slots:
  void startStop();
  void startTimeMeasurement(string name);
  //void addTimer(string name);

public:
  LennardNet();
  virtual ~LennardNet();
};

#endif // LennardNet_H
