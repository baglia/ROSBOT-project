#include "Cinematica.h"

bool calcMotorAngles(const Point2D _point, bool _isAlpha)
{
  if (_isAlpha)
  {
    calcAngles(_point,&alpha.mainAngle,&alpha.secondAngle);
    betaAbs.mainAngle=alpha.mainAngle+alpha.secondAngle-12500;
  }
  else
  {
    calcAngles(_point,&beta.secondAngle,&beta.mainAngle);
    betaAbs.secondAngle=beta.secondAngle+beta.mainAngle-12500;
  }
  return true;
}

bool calcAngles(const Point2D _point, int* _alpha, int* _beta)
{
    *_alpha =setAngle(atan2(_point.y,_point.x)+acos(( pow(LENGTH1, 2) + pow( calcModule(_point), 2) - pow( LENGTH2, 2)) / ( 2 * LENGTH1 * calcModule(_point))))/(2*PI)*50000;
    *_beta = setAngle(acos((pow(LENGTH1, 2) - pow(calcModule(_point), 2) + pow(LENGTH2, 2)) / (2 * (float)LENGTH1 * LENGTH2)))/(2*PI)*25000;
    return true;
}

bool calcJoint1(float _alpha, Point2D* _point)
{
  _point->x = LENGTH1 * cos(_alpha);
  _point->y = LENGTH1 * sin(_alpha);
  return true;
}

float calcJointSpeed(Vector2D _velocity, Point2D _point, bool _isAlpha)
{
  Point2D point12, pointV2;
  float q2, omega, phase;
  pointV2.x = _velocity.module * cos(_velocity.phase);
  pointV2.y = _velocity.module * sin(_velocity.phase);
  if (_isAlpha)
  {
      if(betaAbs.mainAngle!=0){
    q2 = pointV2.y - pointV2.x * tan(betaAbs.mainAngle + PI_2);
    point12.x = q2 / (tan(alpha.mainAngle + PI_2) - tan(betaAbs.mainAngle + PI_2));
    point12.y = tan(alpha.mainAngle + PI_2) * point12.x;
      }else{
          point12.x=pointV2.x;
          point12.y=0;
      }
    omega = pointDistance(origin, point12) / LENGTH1;
    phase = setAngle(atan2(point12.y, point12.x));
    if (micronRound(alpha.mainAngle + PI_2,100000) != micronRound(phase,100000))
      omega *= -1;
  }
  else
  {
      if(betaAbs.secondAngle!=0){
    q2 = pointV2.y - pointV2.x * tan(betaAbs.secondAngle + PI_2);
    point12.x = q2 / (tan(beta.secondAngle + PI_2) - tan(betaAbs.secondAngle + PI_2));
    point12.y = tan(beta.secondAngle + PI_2) * point12.x;
      }else{
          point12.x=pointV2.x;
          point12.y=0;
      }
    omega = pointDistance(point12, pointV2) / LENGTH2;
    phase = setAngle(atan2(pointV2.y - point12.y, pointV2.x - point12.x));
    if (micronRound(alpha.mainAngle + PI_2,1000000) != micronRound(phase,1000000))
      omega *= -1;
  }
  return omega;
}

bool calcAlpha(float _beta, Line _targetLine, Point2D _previousPoint, Point2D _lastPoint, bool _isAlpha)
{
  float radius;
  Point2D point1, point2, point;
  radius = sqrt(pow(LENGTH1,2)+pow(LENGTH2,2)-2*LENGTH1*LENGTH2*cos(_beta));
  point1.x=solveQE(1+pow(_targetLine.slope,2),2*_targetLine.offset*_targetLine.slope,pow(_targetLine.offset,2)-pow(radius,2),true);
  point1.y=lineY(point1.x,_targetLine);
  point2.x=solveQE(1+pow(_targetLine.slope,2),2*_targetLine.offset*_targetLine.slope,pow(_targetLine.offset,2)-pow(radius,2),false);
  point2.y=lineY(point1.x,_targetLine);
  if (pointDistance(_previousPoint, _lastPoint) < pointDistance(_lastPoint, point1) || pointDistance(_previousPoint, _lastPoint) < pointDistance(_previousPoint, point1))
  {
    point2DCopy(point2, &point);
  }
  else if (pointDistance(_previousPoint, _lastPoint) < pointDistance(_lastPoint, point2) || pointDistance(_previousPoint, _lastPoint) < pointDistance(_previousPoint, point2))
  {
    point2DCopy(point1, &point);
  }
  else if (pointDistance(_previousPoint, point1) < pointDistance(_previousPoint, point2))
  {
    point2DCopy(point1, &point);
  }
  else
  {
    point2DCopy(point2, &point);
  }
  return calcMotorAngles(point, _isAlpha);
}

bool calcBeta(float _alpha, Line _targetLine, Point2D _previousPoint, Point2D _lastPoint, bool _isAlpha)
{
  Point2D joint1;
  calcJoint1(_alpha, &joint1);
  Point2D point1, point2, point;
  point1.x = solveQE(1 + pow(_targetLine.slope, 2), -2 * joint1.x + 2 * (_targetLine.offset - joint1.y) * _targetLine.slope, pow(_targetLine.offset - joint1.y, 2) + pow(joint1.x, 2) - pow(LENGTH2, 2), true);
  point1.y = point1.x * _targetLine.slope + _targetLine.offset;
  point2.x = solveQE(1 + pow(_targetLine.slope, 2), -2 * joint1.x + 2 * (_targetLine.offset - joint1.y) * _targetLine.slope, pow(_targetLine.offset - joint1.y, 2) + pow(joint1.x, 2) - pow(LENGTH2, 2), false);
  point1.y = point1.x * _targetLine.slope + _targetLine.offset;
  if (pointDistance(_previousPoint, _lastPoint) < pointDistance(_lastPoint, point1) || pointDistance(_previousPoint, _lastPoint) < pointDistance(_previousPoint, point1))
  {
    point2DCopy(point2, &point);
  }
  else if (pointDistance(_previousPoint, _lastPoint) < pointDistance(_lastPoint, point2) || pointDistance(_previousPoint, _lastPoint) < pointDistance(_previousPoint, point2))
  {
    point2DCopy(point1, &point);
  }
  else if (pointDistance(_previousPoint, point1) < pointDistance(_previousPoint, point2))
  {
    point2DCopy(point1, &point);
  }
  else
  {
    point2DCopy(point2, &point);
  }
  return calcMotorAngles(point, _isAlpha);
}

bool calcPoint(float _alpha, float _beta, Point2D* _point)
{
    if(_alpha<PI/2)
    {
        _point->x = LENGTH1 * cos(_alpha) + LENGTH2 * sin(_beta-(PI/2-_alpha));
        _point->y = LENGTH1 * sin(_alpha) - LENGTH2 * cos(_beta-(PI/2-_alpha));
    }else
    {
        _point->x = LENGTH1 * cos(_alpha) + LENGTH2 * cos(_beta-(PI-_alpha));
        _point->y = LENGTH1 * sin(_alpha) + LENGTH2 * sin(_beta-(PI-_alpha));
    }

  return true;
}
