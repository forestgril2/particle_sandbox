#include "LennardNet.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QPainter>

#include <iostream>

const unsigned pixelMass = 1;
const unsigned int MSEC_PER_SEC = 1000;
const double TIME_INTERVAL = 0.02;
const double gravityConstant = 1;
const unsigned numberOfMassPoints = 200;
const double LJConstant = 0.0001;
const double LJDistance = 500/sqrt(numberOfMassPoints);
const double squareLJDist = LJDistance*LJDistance;
const double maxSpeed = 1;

double randd(double max)
{
  return ((double)rand()/(double)RAND_MAX)*max;
};

LennardNet::LennardNet()
{
  setGeometry(200, 200, 500, 500);
  initPixels();
  initAction();
  initLabel();
  initUpdateInterval();
}

void LennardNet::initLabel()
{
  label = new QLabel( this );
  label->setStyleSheet("QLabel { background-color : red; color : blue; }");
  label->setText( "Time: " );
  label->move(250, 20);
}

void LennardNet::initUpdateInterval()
{
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(TIME_INTERVAL * MSEC_PER_SEC);
  nanoTimer.start();
  nanoTimerTotal.start();
}


void LennardNet::initPixels()
{
  pixels.resize(numberOfMassPoints);
  
  for (auto &p : pixels)
  {
    p = Point2D(randd(width()), randd(height()));
    p.setSpeed(randd(maxSpeed) - maxSpeed/2, randd(maxSpeed) - maxSpeed/2);
    p.setColor(Color(Qt::red));
  }
  
  markerPixel = Point2D(width()/2, height()/2);
  markerPixel.setSpeed(-50,50);
  markerPixel.setColor(Qt::green);
}

void LennardNet::initAction()
{
  QAction* action = new QAction(this);
  action->setText( "Quit" );
  connect(action, SIGNAL(triggered()), SLOT(close()) );
  menuBar()->addMenu( "File" )->addAction( action );
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
    painter->setPen(QPen(p.color(), 3, Qt::SolidLine, Qt::RoundCap));
    p.paint(painter);
  }
  painter->setPen(QPen(markerPixel.color(), 6, Qt::SolidLine, Qt::RoundCap));
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
  if (markerPixel.pos().x() < 50 && false == elapsed)
  {
    elapsed = true;
    label->setText("Time: " + QString::number(nanoTimerTotal.nsecsElapsed()/1000000));
  }
}


Point2D LennardNet::calculateForceForPoint(Point2D pos)
{
  Point2D force(0,0);
  Point2D vector;

  for (auto &p : pixels)
  {
    vector = p.pos() - pos;
    //force += gravityForce(vector);
    force += LJForce(vector);
  }
  return force;
}

Point2D LennardNet::LJForce(Point2D vector)
{
  double sqrDist = vector.rx()*vector.rx() + vector.ry()*vector.ry();
  if (sqrDist < 100) return Point2D(0, 0);
  
  double r = sqrt(sqrDist);
  
  double Y = squareLJDist / sqrDist;
  double X = Y*Y*Y;
  
  //(12 LJsigma^6)/r^7-(12 LJsigma^12)/r^13

  return LJConstant * ((12/r) * X * (1 - X) ) * vector/(r);
}

Point2D LennardNet::gravityForce(Point2D vector)
{
  double sqrDist = sqrt(vector.rx()*vector.rx() + vector.ry()*vector.ry());
  if (sqrDist == 0) return Point2D(0, 0);
  return gravityConstant * vector / sqrDist;
}


void Pixel::proceedInTime(double timeDiff, Point2D acceleration)
{
  pos_ += timeDiff*speed_ + acceleration*timeDiff*timeDiff/2;
  speed_ += acceleration*timeDiff;
}


#include "LennardNet.moc"
