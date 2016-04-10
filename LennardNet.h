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
  void addPixel(double x, double y, Color color);
  void addPixel(double x, double y, Color color, double mass);
  void addPixelsSquareNet(double squareSide, Rectangle R,  Point2D speed, Color color);
  void proceedInTime(double timeDiff);
  void initCanvasUpdateTimer();
  void initLabel();
  Point2D calculateForceForPoint(Point2D pos);
  double totalEnergy();
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
  const double pixelSizeForUnitMass = 7;
  
private slots:
  void startStop();
  void startTimeMeasurement(string name);
  //void addTimer(string name);

public:
  LennardNet();
  virtual ~LennardNet();
};

//const double LennardNet::pixelSizeForUnitMass = 7;

#endif // LennardNet_H
