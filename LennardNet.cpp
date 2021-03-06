#include "LennardNet.h"
#include "math-phys.h"
#include "types_includes.h"

#include <iostream>


// Constants
const unsigned canvasWidth = 1000; 
const unsigned canvasHeight = 1000;

const unsigned pixelMass = 1;
const unsigned int MSEC_PER_SEC = 1000;
const double TIME_INTERVAL = 0.05;
const unsigned numPointsMax = 5000;
const double randomSpeedComponent = 0;

LennardNet::LennardNet() : startedUpdates(false)
{
  setGeometry(200, 200, 1000, 1000);
  
  addPixelsSquareNet(12, Rectangle(250, 200, 200, 200), Point2D(70, 0), Qt::red);
  //addPixelsSquareNet(20, Rectangle(500, 500, 201, 201), Qt::green);
  //addPixelsSquareNet(20, Rectangle(500, 300, 201, 201), Qt::blue);
  //addPixelsSquareNet(20, Rectangle(300, 500, 201, 201), Qt::yellow);
  addPixel(width()/2 -100, height()/2, Qt::yellow, 700);
  //pixels[pixels.size() -1].setSpeed(10,-10);
  //addPixel(width()/2 +100, height()/2, Qt::green);
  
  initAction();
  initLabel();
  initStartStopButton();
  initCanvasUpdateTimer();
}

void LennardNet::initStartStopButton()
{
  startStopButton = new QPushButton("START", this);
  startStopButton -> setGeometry(0, 20, 50, 20);
  connect(startStopButton, SIGNAL(pressed()), SLOT(startStop()));
  startStopButton -> show();
}

void LennardNet::initAction()
{
  QAction* action = new QAction(this);
  action->setText( "Quit" );
  connect(action, SIGNAL(triggered()), SLOT(close()) );
  menuBar()->addMenu( "File" )->addAction( action );
}

void LennardNet::initLabel()
{
  label = new QLabel( this );
  label->setStyleSheet("QLabel { background-color : black; color : white; }");
  label->setText( "Time: " );
  label->move(0, 0);
}

void LennardNet::initCanvasUpdateTimer()
{
  canvasUpdateTimer = new QTimer(this);
 // connect(canvasUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));
}

void LennardNet::startStop()
{
  if (true == startedUpdates)
  {
    startStopButton -> setText("START");
    canvasUpdateTimer-> stop();
    paintEventNanoTimer.invalidate();
    startedUpdates = false;
  }
  else
  {
    startStopButton -> setText("STOP");
    canvasUpdateTimer->start(TIME_INTERVAL * MSEC_PER_SEC);
    paintEventNanoTimer.start();
    startedUpdates = true;
  }
}

void LennardNet::addPixel(double x, double y, Color color)
{
  auto numPoints = pixels.size();
  pixels.resize(++numPoints);
  pixels[numPoints -1] = Point2D(x, y);
  pixels[numPoints -1].setColor(color);
  pixels[numPoints -1].setMass(1);
}

void LennardNet::addPixel(double x, double y, Color color, double mass)
{
  auto numPoints = pixels.size();
  pixels.resize(++numPoints);
  pixels[numPoints -1] = Point2D(x, y);
  pixels[numPoints -1].setColor(color);
  pixels[numPoints -1].setMass(mass);
}

void LennardNet::addPixelsSquareNet(double squareSide, Rectangle R, Point2D speed, Color color)
{
  unsigned numPoints;
  
  for (double x = R.left(); x <= R.left() + R.width(); x += squareSide)
  {
    for (double y = R.top(); y <= R.top() + R.height(); y += squareSide)
    {
      addPixel(x, y, color);

      numPoints = pixels.size();
      pixels[numPoints -1].setSpeed(speed.x() + randd(randomSpeedComponent), speed.y() + randd(randomSpeedComponent));
      if (numPoints > numPointsMax) break;
    }
    if (numPoints > numPointsMax) break;
  }
}

LennardNet::~LennardNet()
{}

void LennardNet::paintEvent(QPaintEvent* pE)
{
  QPainter painter(this);
  
  painter.fillRect(0, 0, width(), height(), Qt::black);
  paintPoints(&painter);
  
  if (true == startedUpdates)
  {
    qint64 nanoSec = paintEventNanoTimer.nsecsElapsed();
    paintEventNanoTimer.restart();
    proceedInTime(static_cast<double>(nanoSec)/1000000000.);
  }
}

void LennardNet::paintPoints(Painter* painter)
{
  for (auto p : pixels)
  {
    painter->setPen(QPen(p.color(), pixelSizeForUnitMass * pow(p.mass(), 0.333333333333333333333), Qt::SolidLine, Qt::RoundCap));
    p.paint(painter);
  }
}

void LennardNet::proceedInTime(double timeDiff)
{
  Point2D* accelerations = new Point2D[pixels.size()];
  unsigned size = pixels.size();

  calculationNanoTimer.start();
  for (auto p = 0; p < size; p++)
  {
    accelerations[p] = calculateForceForPoint(pixels[p].pos())/pixels[p].mass();
  }
  
  for (auto p = 0; p < size; p++)
  {
    pixels[p].proceedInTime(timeDiff, accelerations[p]);
  }
  printCalculationTime();
  update();
}

void LennardNet::startTimeMeasurement(string name)
{
  paintEventNanoTimer.start();
}

void LennardNet::printCalculationTime()
{
  long unsigned timeElapsed= calculationNanoTimer.nsecsElapsed()/1000000;
  label->setText("Time: " + QString::number(timeElapsed));
}

Point2D LennardNet::calculateForceForPoint(Point2D pos)
{
  Point2D force(0,0);
  Point2D vector;
  
  unsigned size = pixels.size();

  for (auto p = 0; p < size; p++)
  {
    vector = pixels[p].pos() - pos;
    force += pixels[p].mass() * gravityForce(vector);
    //force += LJForce(vector);
    //force += springForce(vector);
  }
  return force;
}

#include "LennardNet.moc"
