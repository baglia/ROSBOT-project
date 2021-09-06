#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "GlobalVar.h"
#include <stdbool.h>
#include "math.h"
#include "Point.h"
#include "Mathematic.h"

typedef struct
{
  float module;
  float phase;
} Vector2D;

float calcModule(Point2D _point);
float calcPhase(Point2D _point);

#endif
