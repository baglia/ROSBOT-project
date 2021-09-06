#ifndef SCHEDULING_H
#define SCHEDULING_H 

#include "Library.h"
#include "GlobalVar.h"
#include "LinearMovment.h"
#include "HL_het.h"
#include "HL_sys_core.h"
#include "HL_gio.h"
#include "Cinematica.h"

bool interpreter(Queue* _cmd, Queue* _queue1, Queue* _queue2, Queue* _queue3);
bool startCalcLoop(Command* _cmd, Queue* _queue1, Queue* _queue2);
bool calcLoopLinear(ParameterLinearMovment _movment, Queue* _queue1, Queue* _queue2);
bool calcLoopCircular(ParameterLinearMovment _movment, Queue* _queue1, Queue* _queue2);
void homing(void);
void homeAsse1(void);
void homeAsse2(void);
void asse1Step(int _step, unsigned int period);
void asse2Step(int _step, unsigned int period);
void unrelated(Point2D _point, float _vel, Queue* _periods1, Queue* _periods2);
#endif
