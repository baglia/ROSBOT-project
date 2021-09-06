#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
 #include "stdlib.h"
 #include "wiring.h"
#endif

#include "Point.h"
#include <MultiStepper.h>
#include <AccelStepper.h>

#define M1_CCW 3
#define M1_CW 2
#define M2_CCW 4
#define M2_CW 5

#define PROXY_M1 6
#define PROXY_M2 7

#define LENGTH1 370
#define LENGTH2 260

#define M1_RUN 8
#define M2_RUN 13
#define MZ_EN 10

#define EMG_CHN 11

#define MZ_MOVING 38
#define MZ_TRG 28
#define MZ_RST 32
#define MZ_PROG 30

#define M1_ALM 22
#define M2_ALM 24
#define MZ_ALM 26


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


extern int tolerance, epromPosition[];
extern MotorAngle alpha, beta;
extern MotorAngle betaAbs;  //rappresenta l'angolo del secondo asse riferito rispetto al sistema di riferimento fisso. alpha -> mainAngle ;  beta -> secondAngle.
extern Point2D origin, actualPoint;
extern float maxSpeed, maxAccel, minSpeed, acceleration;
extern bool isMovment, isFinished;
extern volatile bool isHomed, isEmergency;



bool isPointReachable(Point2D _point);
void emergencyMsg(void);
void finishedMsg(void);
void errorMsg(void);
void okMsg(void);
void unreachbleMsg(void);
void fullMsg(void);
void m1CW(void);
void m1CCW(void);
void m2CW(void);
void m2CCW(void);
#endif
