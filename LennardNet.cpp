#include "LennardNet.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QPainter>

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


LennardNet::LennardNet()
{
  setGeometry(200, 200, canvasWidth, canvasHeight);
  
  addPixelsSquareNet(20, Rectangle(300, 300, 201, 201), Qt::red);
  addPixelsSquareNet(20, Rectangle(500, 500, 201, 201), Qt::green);
  addPixelsSquareNet(20, Rectangle(500, 300, 201, 201), Qt::blue);
  addPixelsSquareNet(20, Rectangle(300, 500, 201, 201), Qt::yellow);
  
  initMarkerPixel();
  initAction();
  initLabel();
  initStartButton();
  initTimers();
}

void LennardNet::initStartButton()
{
  QPushButton* startButton = new QPushButton("START", this);
  startButton -> setGeometry(width() - 70, height() - 40, 50, 20);
  connect(startButton, SIGNAL(pressed()), SLOT(startTimers()));
  startButton -> show();
}

void LennardNet::initMarkerPixel()
{
  markerPixel = Point2D(width()/2, height()/2);
  markerPixel.setSpeed(-50, 50);
  markerPixel.setColor(Qt::green);
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
  label->move(20, 20);
}

void LennardNet::initTimers()
{
  canvasUpdateTimer = new QTimer(this);
  connect(canvasUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));
  nanoTimer.start();
  nanoTimerTotal.start();
}

void LennardNet::startTimers()
{
  canvasUpdateTimer->start(TIME_INTERVAL * MSEC_PER_SEC);
}
// Maths and physics
double randd(double max)
{
  return ((double)rand()/(double)RAND_MAX)*max;
};

Point2D LJForce(Point2D vector)
{
  static const double LJConstant = 10;
  static const double LJDistance = 10;
  static const double squareLJDist = LJDistance*LJDistance;
  static const double lowerLJCutoff = 0.75*LJDistance;
  static const double upperLJCutoff = 2.5*LJDistance;
  
  double sqrDist = vector.rx()*vector.rx() + vector.ry()*vector.ry();
  double r = sqrt(sqrDist);
  
  if (r < lowerLJCutoff || r > upperLJCutoff) return Point2D(0, 0);
  
  double Y = squareLJDist / sqrDist;
  double X = Y*Y*Y;
  
  //(12 LJsigma^6)/r^7-(12 LJsigma^12)/r^13
  
  return LJConstant * ((12/r) * X * (1 - X) ) * vector/(r);
}

Point2D gravityForce(Point2D vector)
{
  static const double gravityConstant = 0.3;
  static const double lowerGravityCutoff = 2;
  static const double upperGravityCutoff = 5000;
  
  double sqrDist = sqrt(vector.rx()*vector.rx() + vector.ry()*vector.ry());
  double dist = sqrt(sqrDist); 
  if (dist < lowerGravityCutoff || dist > upperGravityCutoff) return Point2D(0, 0);
  return gravityConstant * vector / sqrDist;
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

void Pixel::paint(Painter* painter)
{
  painter->drawPoint(pos_);
}

void LennardNet::paintEvent(QPaintEvent* pE)
{
  QPainter painter(this);
  
  painter.fillRect(0, 0, width(), height(), Qt::black);
  paintPoints(&painter);
  
  qint64 nanoSec = nanoTimer.nsecsElapsed();
  nanoTimer.restart();
  proceedInTime(static_cast<double>(nanoSec)/1000000000.);
}

void LennardNet::paintPoints(Painter* painter)
{
  for (auto p : pixels)
  {
    painter->setPen(QPen(p.color(), 7, Qt::SolidLine, Qt::RoundCap));
    p.paint(painter);
  }
  painter->setPen(QPen(markerPixel.color(), 7, Qt::SolidLine, Qt::RoundCap));
  markerPixel.paint(painter);
}

void LennardNet::proceedInTime(double timeDiff)
{
  Point2D acceleration;
  
  for (auto &p : pixels)
  {
    acceleration = calculateForceForPoint(p.pos())/pixelMass;
    p.proceedInTime(timeDiff, acceleration);
  }
  
  markerPixel.proceedInTime(timeDiff, Point2D(0,0));
  checkInformMarkerPixelTime();
}

void LennardNet::checkInformMarkerPixelTime()
{
  static bool elapsed = false;
  static long unsigned timeElapsed = 0;
  
  if (false == elapsed) 
  {
    timeElapsed = nanoTimerTotal.nsecsElapsed()/1000000;
  }
  
  if (markerPixel.pos().x() < 50)
  {
    elapsed = true;
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

void Pixel::proceedInTime(double timeDiff, Point2D acceleration)
{
  pos_ += timeDiff*speed_ + acceleration*timeDiff*timeDiff/2;
  speed_ += acceleration*timeDiff;
}


#include "LennardNet.moc"
