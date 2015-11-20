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
const double forceFactor = 100;

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
  pixels.resize(300);
	
	for (auto &p : pixels)
  {
    p = Point2D(randd(width()), randd(height()));
    p.setSpeed(randd(width()/10) - width()/20, randd(height()/10) - height()/20);
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
  static bool elapsed = false;
  Point2D acceleration;
  
  for (auto &p : pixels)
  {
    acceleration = calculateForceForPoint(p.pos())/pixelMass;
    p.proceedInTime(timeDiff, acceleration);
  }
  
  markerPixel.proceedInTime(timeDiff, Point2D(0,0));
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
	double dist;
	double invSquareDist;
	
	for (auto &p : pixels)
	{
		vector = p.pos() - pos;
    invSquareDist = (vector.rx()*vector.rx()) + (vector.ry()*vector.ry());
		if (invSquareDist == 0) continue;
		dist = sqrt(invSquareDist);
		force += forceFactor*vector/invSquareDist;
	}
	
	return force;
}


void Pixel::proceedInTime(double timeDiff, Point2D acceleration)
{
  pos_ += timeDiff*speed_ + acceleration*timeDiff*timeDiff/2;
	speed_ += acceleration*timeDiff;
}


#include "LennardNet.moc"
