#include "LennardNet.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QPainter>
#include <QTimer>

#include <iostream>

const unsigned int MSEC_PER_SEC = 1000;
const double TIME_INTERVAL = 0.1;

LennardNet::LennardNet()
{
  setGeometry(200, 200, 500, 500);
  initPixels();
  initAction();
  
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(TIME_INTERVAL * MSEC_PER_SEC);
}

void LennardNet::initPixels()
{
  pixels.resize(2);
  pixels[0] = Point2D(width()/2, height()/2);
  pixels[0].setSpeed(1,1);
  pixels[0].setColor(Color(Qt::red));
  pixels[1] = Point2D(width()/2 + 5, height()/2 + 5);
  pixels[1].setSpeed(-1,-1);
  pixels[1].setColor(Color(Qt::green));
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
  proceedInTime(TIME_INTERVAL);
}

void LennardNet::paintPoints(Painter* painter)
{
  for (auto p : pixels)
  {
    painter->setPen(QPen(p.color(), 3, Qt::SolidLine, Qt::RoundCap));
    p.paint(painter);
  }
}

void LennardNet::proceedInTime(double timeDiff)
{
  for (auto &p : pixels)
  {
    p.proceedInTime(timeDiff);
  }
}

void Pixel::proceedInTime(double timeDiff)
{
  pos_ += timeDiff*speed_;
}


#include "LennardNet.moc"
