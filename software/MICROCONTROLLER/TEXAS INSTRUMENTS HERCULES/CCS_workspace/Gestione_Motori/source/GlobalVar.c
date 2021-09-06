/*
 * GlobalVar.c
 *
 *  Created on: 17 lug 2021
 *      Author: campr
 */

#include "GlobalVar.h"

bool isPointReachable(Point _point)
{
    if(pointDistance(origin,point2D(_point))>(LENGTH1+LENGTH2)||pointDistance(origin,point2D(_point))<(LENGTH1-LENGTH2))
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
