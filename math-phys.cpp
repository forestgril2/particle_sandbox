#include "math-phys.h"

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
  static const double gravityConstant = 0.03;
  static const double lowerGravityCutoff = 2;
  static const double upperGravityCutoff = 5000;
  
  double sqrDist = sqrt(vector.rx()*vector.rx() + vector.ry()*vector.ry());
  double dist = sqrt(sqrDist); 
  if (dist < lowerGravityCutoff || dist > upperGravityCutoff) return Point2D(0, 0);
  return gravityConstant * vector / sqrDist;
}

Point2D springForce(Point2D vector)
{
  static const double minDist = 7;
  static const double maxDist = 13;
  static const double neutralDist = 10.1;
  static const double k = 1;
  double dist = sqrt(vector.rx()*vector.rx() + vector.ry()*vector.ry());
  
  if (dist > maxDist || dist < minDist) return Point2D(0, 0);
  
  return k * vector * pow(dist,-1) * (dist - neutralDist);
}
