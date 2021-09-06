/*
 * GlobalVar.c
 *
 *  Created on: 17 lug 2021
 *      Author: campr
 */

#include "GlobalVar.h"

bool isPointReachable(Point2D _point)
{
  if(pointDistance(origin,_point)>(LENGTH1+LENGTH2)||pointDistance(origin,_point)<(LENGTH1-LENGTH2)||_point.y<0)
    return false;
  return true;
}

void emergencyMsg(void)
{
    Serial.print("|Emg!");
}

void finishedMsg(void)
{
    Serial.print("Fns!");
}

void errorMsg(void)
{
    Serial.print("Err!");
}

void okMsg(void)
{
    Serial.print("Ok!");
}

void unreachbleMsg(void)
{
    Serial.print("Un!");
}

void fullMsg(void)
{
    Serial.print("Fll!");
}

void m1CW(void)
{
    digitalWrite(M1_CW,1);
    delayMicroseconds(1);
    digitalWrite(M1_CW,0);
}

void m1CCW(void)
{
    digitalWrite(M1_CCW,1);
    delayMicroseconds(1);
    digitalWrite(M1_CCW,0);
}

void m2CW(void)
{
    digitalWrite(M2_CW,1);
    delayMicroseconds(1);
    digitalWrite(M2_CW,0);
}

void m2CCW(void)
{
    digitalWrite(M2_CCW,1);
    delayMicroseconds(1);
    digitalWrite(M2_CCW,0);
}
