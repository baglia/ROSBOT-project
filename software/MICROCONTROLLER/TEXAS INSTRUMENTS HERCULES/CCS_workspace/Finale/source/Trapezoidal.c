/*
 * Trapezoidal.c
 *
 *  Created on: 18 ago 2021
 *      Author: campr
 */

#include "Trapezoidal.h"

bool setTrapezoidal(float _maxSpeed, float _time, float _tAcc, Trapezoidal* _law)
{
    _law->maxSpeed=_maxSpeed;
    _law->time=_time;
    _law->tAcc=_tAcc;
    return true;
}

float getTrapezoidalSpeed(float _time, Trapezoidal _law)
{
    if(_time<=_law.tAcc)
    {
        return acceleration*_time+minSpeed;
    }else if(_time<=_law.time-_low.tAcc)
    {
        return _law.maxSpeed;
    }else
    {
        return -acceleration*(_time-_law.time+_law.tAcc)+_law.maxSpeed;
    }
}
