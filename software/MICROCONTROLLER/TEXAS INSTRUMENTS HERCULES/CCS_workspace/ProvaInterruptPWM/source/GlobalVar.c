/*
 * GlobalVar.c
 *
 *  Created on: 17 lug 2021
 *      Author: campr
 */

#include "GlobalVar.h"

bool isPointReachable(Point _point)
{
    if(pointDistance(origin,point2D(_point))>(length1+length2)||pointDistance(origin,point2D(_point))<(length1-length2))
        return false;
    if(_point.z<0||_point.z>300)
        return false;
    return true;
}
