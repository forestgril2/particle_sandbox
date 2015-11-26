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
const unsigned numPointsMax = 1000;
const double maxSpeed = 0;

double netWidth = 410; 
double netHeight = 300;
Rectangle netShape((canvasWidth - netWidth)/2, (canvasHeight - netHeight)/2, netWidth, netHeight);


LennardNet::LennardNet() : startedUpdates(false)
{
  setGeometry(200, 200, 100, 40);
  
  addPixelsSquareNet(20, Rectangle(300, 300, 201, 201), Qt::red);
  addPixelsSquareNet(20, Rectangle(500, 500, 201, 201), Qt::green);
  addPixelsSquareNet(20, Rectangle(500, 300, 201, 201), Qt::blue);
  addPixelsSquareNet(20, Rectangle(300, 500, 201, 201), Qt::yellow);
  //addPixel(widht()/2 -100, Qt::yellow);
  
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
  connect(canvasUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));
}

void LennardNet::startStop()
{
  if (true == startedUpdates)
  {
    startStopButton -> setText("START");
    canvasUpdateTimer-> stop();
    nanoTimer.invalidate();
    nanoTimerTotal.invalidate();
    startedUpdates = false;
  }
  else
  {
    startStopButton -> setText("STOP");
    canvasUpdateTimer->start(TIME_INTERVAL * MSEC_PER_SEC);
    nanoTimer.start();
    nanoTimerTotal.start();
    startedUpdates = true;
  }
}

void LennardNet::addPixelsSquareNet(double squareSide, Rectangle R, Color color)
{
  auto numPoints = pixels.size();
  
  for (auto x = R.left(); x <= R.left() + R.width(); x += squareSide)
  {
    for (auto y = R.top(); y <= R.top() + R.height(); y += squareSide)
    {
      pixels.resize(++numPoints);
    
      pixels[numPoints -1] = Point2D(x, y);

      pixels[numPoints -1].setSpeed(randd(maxSpeed) - maxSpeed/2, randd(maxSpeed) - maxSpeed/2);
      pixels[numPoints -1].setColor(color);
      
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
    qint64 nanoSec = nanoTimer.nsecsElapsed();
    nanoTimer.restart();
    proceedInTime(static_cast<double>(nanoSec)/1000000000.);
  }
}

void LennardNet::paintPoints(Painter* painter)
{
  for (auto p : pixels)
  {
    painter->setPen(QPen(p.color(), 7, Qt::SolidLine, Qt::RoundCap));
    p.paint(painter);
  }
}

void LennardNet::proceedInTime(double timeDiff)
{
  Point2D acceleration;
  
  startTimeMeasurement("All pixel calculations.");
  for (auto &p : pixels)
  {
    acceleration = calculateForceForPoint(p.pos())/pixelMass;
    p.proceedInTime(timeDiff, acceleration);
  }
  printTimes();
}

void LennardNet::startTimeMeasurement(string name)
{
  //addTimer(name);
  //startTimer(name);
}

void LennardNet::printTimes()
{
  static bool elapsed = false;
  static long unsigned timeElapsed = 0;
  
  if (false == elapsed) 
  {
    timeElapsed = nanoTimerTotal.nsecsElapsed()/1000000;
  }
  
  label->setText("Time: " + QString::number(timeElapsed));
}

Point2D LennardNet::calculateForceForPoint(Point2D pos)
{
  Point2D force(0,0);
  Point2D vector;

  for (auto &p : pixels)
  {
    vector = p.pos() - pos;
    force += gravityForce(vector);
    //force += LJForce(vector);
  }
  return force;
}

#include "LennardNet.moc"
