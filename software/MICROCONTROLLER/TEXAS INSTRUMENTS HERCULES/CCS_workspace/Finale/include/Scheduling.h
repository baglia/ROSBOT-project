#ifndef SCHEDULING_H
#define SCHEDULING_H 

#include "Library.h"
#include "GlobalVar.h"
#include "LinearMovment.h"
#include "HL_het.h"
#include "HL_sys_core.h"
#include "HL_gio.h"
#include "Cinematica.h"
#include "Trapezoidal.h"

bool interpreter(Queue* _cmd, Queue* _queue1, Queue* _queue2, Queue* _queue3);
bool startCalcLoop(Command* _cmd, Queue* _queue1, Queue* _queue2);
bool calcLoopLinear(ParameterLinearMovment _movment, Queue* _queue1, Queue* _queue2);
bool calcLoopCircular(ParameterLinearMovment _movment, Queue* _queue1, Queue* _queue2);
void homing(void);
int homeAsse1(void);
int homeAsse2(void);
void asse1Step(int _step, int _velMax, Queue* _periods);
void asse2Step(int _step, int _velMax, Queue* _periods);
void unrelated(Point2D _point, float _vel, Queue* _periods1, Queue* _periods2);
void calcTrapezoidal(float _maxSpeed, float _tTot,float _sTot, Trapezoidal* _low);
void searchTime(float _vMax, unsigned int _stepTot, int _length, float _dAngle, float* _tAcc, float* _tTot, float* _velReal);
void searchVMax(float _tTot, unsigned int _stepTot, float _dAngle, int _length, float* _vMax, float* _tAcc);
#endif
