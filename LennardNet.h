#ifndef LennardNet_H
#define LennardNet_H

#include <QtGui/QMainWindow>

class LennardNet : public QMainWindow
{
  Q_OBJECT

  void paintPointA(QPainter* painter);
  virtual void paintEvent(QPaintEvent* pE);
    void initAction();
    void paintPoints(QPainter* painter);

  QPoint pointA;
  QPoint pointB;

public:
  LennardNet();
  virtual ~LennardNet();
};

#endif // LennardNet_H
