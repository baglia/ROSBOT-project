#include "Geometry.h"

float calcModule(Point2D _point)
{
  return sqrt(pow(_point.x, 2) + pow(_point.y, 2));
}

float calcPhase(Point2D _point)
{
  return atan2(_point.y, _point.x);
}
