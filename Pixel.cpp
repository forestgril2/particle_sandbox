#include "Pixel.h"


void Pixel::paint(Painter* painter)
{
  painter->drawPoint(pos_);
}

void Pixel::proceedInTime(double timeDiff, Point2D acceleration)
{
  pos_ += timeDiff*speed_ + acceleration*timeDiff*timeDiff/2;
  speed_ += acceleration*timeDiff;
}