/*
 * GlobalVar.c
 *
 *  Created on: 17 lug 2021
 *      Author: campr
 */

#include "GlobalVar.h"

bool isPointReachable(Point _point)
{
    if(pointDistance(origin,point2D(_point))>(LENGTH1+LENGTH2)||pointDistance(origin,point2D(_point))<(LENGTH1-LENGTH2)||_point.y<0)
        return false;
    if(_point.z<0||_point.z>300)
        return false;
    return true;
}

void emergencyMsg(void)
{
    char msg[]={"|Emg!"};
    sciSend(sciREG1,5,(uint8*)msg);
}

void finishedMsg(void)
{
    char msg[]={"Fns!"};
    sciSend(sciREG1,4,(uint8*)msg);
}

void emergenza(void)
{
    gioSetBit(gioPORTA,0,0);
    gioSetBit(gioPORTA,3,0);
    gioSetBit(gioPORTB,1,0);
    char msg[]={"so11|v!"};
    sciSend(sciREG3,7,(uint8*)msg);
}

void reset(void)
{
    gioSetBit(gioPORTA,0,1);
    gioSetBit(gioPORTA,3,1);
    gioSetBit(gioPORTB,1,1);
    char msg[]={"so11|u!"};
    sciSend(sciREG3,7,(uint8*)msg);
}
