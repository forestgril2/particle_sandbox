#include "LennardNet.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QPainter>

LennardNet::LennardNet()
{
  setGeometry(200, 200, 500, 500);
  
  pixels.resize(2);
  pixels[0] = Point2D(width()/2, height()/2);
  pixels[0].setColor(Color(Qt::red));
  pixels[1] = Point2D(width()/2 + 5, height()/2 + 5);
  pixels[1].setColor(Color(Qt::green));
  
  initAction();
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
  painter->drawPoint(p_);
}

void LennardNet::paintEvent(QPaintEvent* pE)
{
  QPainter painter(this);
  
  painter.fillRect(0, 0, width(), height(), Qt::black);
  paintPoints(&painter);
}

void LennardNet::paintPoints(Painter* painter)
{
  for (auto p : pixels)
  {
    painter->setPen(QPen(p.color(), 3, Qt::SolidLine, Qt::RoundCap));
    p.paint(painter);
  }
}

#include "LennardNet.moc"
