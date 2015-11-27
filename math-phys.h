#ifndef mathphys_H
#define mathphys_H
#include "types_includes.h"

double randd(double max);
Point2D LJForce(Point2D vector);
Point2D gravityForce(Point2D vector);
Point2D springForce(Point2D vector);

#endif