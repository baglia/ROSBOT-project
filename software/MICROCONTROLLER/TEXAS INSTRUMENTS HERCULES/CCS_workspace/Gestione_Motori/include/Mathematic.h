/*
 * Mathematic.h
 *
 *  Created on: 17 lug 2021
 *      Author: campr
 */

#ifndef MATHEMATIC_H_
#define MATHEMATIC_H_

#define E     2.7182818284590452354   /* e */
#define LOG2E     1.4426950408889634074   /* log 2e */
#define LOG10E    0.43429448190325182765  /* log 10e */
#define LN2       0.69314718055994530942  /* log e2 */
#define LN10      2.30258509299404568402  /* log e10 */
#define PI        3.14159265358979323846  /* pi */
#define PI_2      1.57079632679489661923  /* pi/2 */
#define PI_4      0.78539816339744830962  /* pi/4 */
#define S_1_PI      0.31830988618379067154  /* 1/pi */
#define S_2_PI      0.63661977236758134308  /* 2/pi */
#define S_2_SQRTPI  1.12837916709551257390  /* 2/sqrt(pi) */
#define SQRT2     1.41421356237309504880  /* sqrt(2) */
#define SQRT1_2   0.70710678118654752440  /* 1/sqrt(2) */

float micronRound(float _number, int _rounder);
float setAngle(float _angle);
float fitAngle(float _angle);
float absolute(float _number);

#endif /* INCLUDE_MATHEMATIC_H_ */
