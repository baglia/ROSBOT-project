#include "LinearMovment.h"

bool nextPeriodA(Period* _period, ParameterLinearMovment _settings, bool _isNew)
{
  static Point2D point;
  static Vector2D velocity;
  float omega;
  if (_isNew)
  {
    point2DCopy(_settings.firstPoint, &point);
    calcMotorAngles(point, true);
    velocity.phase = atan2(_settings.lastPoint.y - _settings.firstPoint.y, _settings.lastPoint.x - _settings.firstPoint.x);
  }
  velocity.module = getCicloydalSpeed(pointDistance(_settings.firstPoint, point), _settings.cicloydal);
  omega = calcJointSpeed(velocity, point, true);
  if (omega > 0)
  {
    calcBeta(fitAngle(alpha.mainAngle + alpha.dAngle), _settings.targetLine, point, _settings.lastPoint, true);
  } else
  {
    calcBeta(fitAngle(alpha.mainAngle - alpha.dAngle), _settings.targetLine, point, _settings.lastPoint, true);
  }
  calcPoint(alpha.mainAngle, betaAbs.mainAngle, &point);
  if (pointDistance(point, _settings.lastPoint) < tolerance)
  {
    _period->isPeriod = true;
    _period->isEnd = true;
    return true;
  }
  if(omega<0)
  {
      _period->period = 20 / (-omega);
      _period->isCW=true;
  }else
  {
      _period->period = 20 / omega;
      _period->isCW=false;
  }
  _period->isPeriod = true;
  _period->isEnd = false;
  return true;
}

bool nextPeriodB(Period* _period, ParameterLinearMovment _settings, bool _isNew)
{
  static Point2D point;
  static Vector2D velocity;
  float omega;
  if (_isNew)
  {
    point2DCopy(_settings.firstPoint, &point);
    calcMotorAngles(point, false);
    velocity.phase = atan2(_settings.lastPoint.y - _settings.firstPoint.y, _settings.lastPoint.x - _settings.firstPoint.x);
  }
  velocity.module = getCicloydalSpeed(pointDistance(_settings.firstPoint, point), _settings.cicloydal);
  omega = calcJointSpeed(velocity, point, false);
  if (omega > 0)
  {
    calcAlpha(fitAngle(beta.mainAngle + beta.dAngle), _settings.targetLine, point, _settings.lastPoint, false);
  } else
  {
    calcAlpha(fitAngle(beta.mainAngle - beta.dAngle), _settings.targetLine, point, _settings.lastPoint, false);
  }
  calcPoint(beta.secondAngle, betaAbs.secondAngle, &point);
  if (pointDistance(point, _settings.lastPoint) < tolerance)
  {
    _period->isPeriod = true;
    _period->isEnd = true;
    return true;
  }
  if(omega<0)
    {
        _period->period = 40 / (-omega);
        _period->isCW=true;
    }else
    {
        _period->period = 40 / omega;
        _period->isCW=false;
    }
  _period->isPeriod = true;
  _period->isEnd = false;
  return true;
}

bool setLinMovment(Point2D _firstPoint, Point2D _lastPoint, Cicloydal _low, ParameterLinearMovment* _settings)
{
  if (isPoint2DEq(_firstPoint, _lastPoint))
    return false;
  point2DCopy(_firstPoint, &_settings->firstPoint);
  point2DCopy(_lastPoint, &_settings->lastPoint);
  copyCicloydal(_low, &_settings->cicloydal);
  setLine2Point(_firstPoint, _lastPoint, &_settings->targetLine);
  return true;
}
