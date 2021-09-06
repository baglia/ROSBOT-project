/*
 * Mathematic.c
 *
 *  Created on: 17 lug 2021
 *      Author: campr
 */

#include "Mathematic.h"

float micronRound(float _number, int _rounder)
{
  if ((int)(_number * _rounder) % 10 < 5)
  {
    return (float)((int)(_number * _rounder)) / _rounder;
  }
  else
  {
    return (float)((int)(_number * _rounder + 1)) / _rounder;
  }
}

float setAngle(float _angle)
{
    return micronRound(_angle,1000000);
}

float fitAngle(float _angle)
{
  int divisor = 1000000;
  if (_angle > 0)
  {
    return (float)((int)(_angle*divisor) % (int)(2 * PI*divisor))/divisor;
  }
  else
  {
    return 2 * PI + (float)((int)(_angle*divisor) % (int)(2 * PI*divisor))/divisor;
  }
}

float absolute(float _number)
{
    if(_number<0)
        return -_number;
    return _number;
}
