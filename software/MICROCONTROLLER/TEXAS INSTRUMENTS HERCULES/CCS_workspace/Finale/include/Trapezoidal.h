/*
 * Trapezoidal.h
 *
 *  Created on: 18 ago 2021
 *      Author: campr
 */

#ifndef INCLUDE_TRAPEZOIDAL_H_
#define INCLUDE_TRAPEZOIDAL_H_

#include "GlobalVar.h"

typedef struct
{
    float tAcc;
    float maxSpeed;
    float time;
}Trapezoidal;

bool setTrapezoidal(float _maxSpeed, float _time, float _tAcc, Trapezoidal* _law);
float getTrapezoidalSpeed(float _time, Trapezoidal _law);

#endif /* INCLUDE_TRAPEZOIDAL_H_ */
