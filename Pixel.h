#ifndef Pixel_H
#define Pixel_H

#include "types_includes.h"

class Pixel //TODO: move to separate header
{
  Point2D pos_;
  Point2D speed_; //per second
  Color color_;
  double mass_;
  
public:
  Pixel() : pos_(Point2D(0,0)), speed_(Point2D(0,0)) {};
  Pixel(Point2D p) : pos_(p), speed_(Point2D(0,0)), mass_(1) {};
  Pixel(Point2D p, Point2D s) : pos_(p), speed_(s), mass_(1) {};
  Pixel(Point2D p, Point2D s, double m) : pos_(p), speed_(s), mass_(m) {};
  
  void operator=(Point2D p) {pos_ = p;};
  
  void setSpeed(double vx, double vy) {speed_.setX(vx); speed_.setY(vy);};
  void setColor(Color c) {color_ = c;};
  void setMass(double m) {mass_= m;};
  
  Point2D speed() {return speed_;};
  Point2D pos() {return pos_;};
  Color color() {return color_;};
  double mass() {return mass_;};
  
  void paint(Painter* painter);
  void proceedInTime(double timeDiff, Point2D acceleration);
};


#endif