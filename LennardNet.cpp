#include "LennardNet.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QPainter>

LennardNet::LennardNet()
{
  setGeometry(200, 200, 500, 500);
  pointA = QPoint(width()/2, height()/2);
  pointB = QPoint(width()/2 + 5, height()/2 + 5);

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

void LennardNet::paintEvent(QPaintEvent* pE)
{
  QPainter painter(this);
  
  painter.fillRect(0, 0, width(), height(), Qt::black);
  paintPoints(&painter);
}

void LennardNet::paintPoints(QPainter* painter)
{
  painter->setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap));
  painter->drawPoint(pointA);
  
  painter->setPen(QPen(Qt::green, 3, Qt::SolidLine, Qt::RoundCap));
  painter->drawPoint(pointB);
}



#include "LennardNet.moc"
