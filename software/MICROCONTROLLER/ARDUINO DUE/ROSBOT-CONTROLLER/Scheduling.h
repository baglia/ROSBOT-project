#ifndef SCHEDULING_H
#define SCHEDULING_H 

#include "Library.h"
#include "GlobalVar.h"
#include "Cinematica.h"
#include <MultiStepper.h>
#include <AccelStepper.h>

bool readMsg(bool* _status);
bool interpreter(Command* _cmd);
void homing(AccelStepper* motor1,AccelStepper* motor2);
void homeAsse1(AccelStepper* motor1);
void homeAsse2(AccelStepper* motor2);
void asse1Step(int _step, unsigned int period);
void asse2Step(int _step, unsigned int period);
void unrelated(Point2D _point, float _vel,AccelStepper* motor1,AccelStepper* motor2);
void searchVMax(float _tTot, unsigned int _stepTot, float _dAngle, int _length, int _vel, float* _vMax);
void searchTime(float _vMax, unsigned int _stepTot, int _length, float _dAngle, float* _tTot);
#endif
