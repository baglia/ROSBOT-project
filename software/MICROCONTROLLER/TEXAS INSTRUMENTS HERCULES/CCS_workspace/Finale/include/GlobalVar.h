#ifndef GLOBALVAR_H
#define GLOBALVAR_H


#include "Point.h"
#include <stdbool.h>
#include "HL_sci.h"
#include "HL_gio.h"

#define M1_CCW pwm0
#define M1_CW pwm1
#define M2_CCW pwm3
#define M2_CW pwm2

#define PROXY_M1 7
#define PROXY_M2 6

#define LENGTH1 370
#define LENGTH2 260

typedef struct
{
  int mainAngle;
  int secondAngle;
  float dAngle;
} MotorAngle;

typedef struct
{
  unsigned int period;
  bool isPeriod;
  bool isEnd;
  bool isCW;
} Period;

extern int tolerance, iteration;
extern MotorAngle alpha, beta;
extern MotorAngle betaAbs;  //rappresenta l'angolo del secondo asse riferito rispetto al sistema di riferimento fisso. alpha -> mainAngle ;  beta -> secondAngle.
extern Point2D origin, actualPoint;
extern float maxSpeed, maxAccel, minSpeed, acceleration;
extern bool isMovment,isHomed, isFinished, isEmergency;


bool isPointReachable(Point _point);
void emergencyMsg(void);
void finishedMsg(void);
void emergenza(void);
void reset(void);
#endif
