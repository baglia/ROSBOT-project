#include "Scheduling.h"

bool interpreter(Command* _cmd)
{
  static AccelStepper motor1(m1CCW,m1CW);
  static AccelStepper motor2(m2CCW,m2CW);
  bool state;
  switch ((int)_cmd->type)
  {
    case UNRELATED: //esegue una movimentazione da un punto ad un altro senza seguire alcuna traiettoria specifica
        if(isEmergency)
              {
                  emergencyMsg();
                  return false;
              }
        if (!isHomed)
            homing(&motor1,&motor2);
        unrelated(_cmd->point,_cmd->parameter,&motor1,&motor2);
        finishedMsg();
      break;
    case JOINT:
      if(isEmergency)
      {
          emergencyMsg();
          return false;
      }
      motor1.move(_cmd->point.x / 360 * 50000);
      motor1.setMaxSpeed(_cmd->parameter/LENGTH1/alpha.dAngle);
      motor2.move(_cmd->point.y / 360 * 25000);
      motor2.setMaxSpeed(_cmd->parameter/LENGTH2/beta.dAngle);
      while(motor1.isRunning()||motor2.isRunning())
      {
        motor1.run();
        motor2.run();
      }
      alpha.mainAngle += _cmd->point.x / 360 * 50000;
      beta.mainAngle += _cmd->point.y / 360 * 25000;
      calcPoint(alpha.mainAngle, beta.mainAngle, &actualPoint);
      finishedMsg();
      break;
    case PROG:  //invia un impulso al motore Z per eseguire il programma
      if(isEmergency)
      {
        emergencyMsg();
        return false;
      }
      digitalWrite(MZ_PROG,_cmd->parameter);
      delay(1);
      digitalWrite(MZ_TRG,1);
      delay(1);
      digitalWrite(MZ_TRG,0);
      delay(1);
      digitalWrite(MZ_TRG,1);
      delay(1);
      digitalWrite(MZ_TRG,0);
      while(digitalRead(MZ_MOVING));
      finishedMsg();
      break;
    case GRIPPER:   //fa aprire o chiudere il gripper
      if (_cmd->parameter == 0)
      {
        Serial1.print("sO19|u!");
      } else
      {
        Serial1.print("sO19|v!");
      }
      finishedMsg();
      break;
    case HOMING:    //esegue l'homing dei motori
      
        if(isEmergency)
              {
                  emergencyMsg();
                  return false;
              }
      homing(&motor1,&motor2);
      finishedMsg();
      break;
    case NUL:

      break;
    case PAUSE:
         delay(_cmd->parameter);
         finishedMsg();
      break;
    case WAIT:
        while(1)
        {
        Serial1.print("si");
        Serial1.print(_cmd->point.x);
        Serial1.print("!");
        while(!Serial1.available());
        if(!readMsg(&state))
        {
          digitalWrite(M1_RUN,0);
          digitalWrite(M2_RUN,1);
          digitalWrite(MZ_EN,1);
          isEmergency=true;
          isHomed=false;
          emergencyMsg();
          finishedMsg();
          return false;
        }
          if(state)
          {
            break;
          }
        }
        finishedMsg();
      break;
    case SET:
        Serial1.print("sO");
        Serial1.print(_cmd->point.x);
        Serial1.print("|");
        switch((int)_cmd->parameter)
        {
          case 1:
          Serial1.print("v");
          break;
          case 0:
          Serial1.print("u");
          break;
          default:
          Serial1.print("!");
          digitalWrite(M1_RUN,0);
          digitalWrite(M2_RUN,1);
          digitalWrite(MZ_EN,1);
          isEmergency=true;
          isHomed=false;
          emergencyMsg();
          break;
        }
        Serial1.print("!");
        finishedMsg();
      break;
    case V_MIN:
      minSpeed=_cmd->parameter;
      finishedMsg();
      break;
    case SET_ACC:
      acceleration=_cmd->parameter;
      finishedMsg();
      break;
    case RESET:
      digitalWrite(M1_RUN,1);
      digitalWrite(M2_RUN,0);
      digitalWrite(MZ_EN,1);
      digitalWrite(MZ_RST,1);
      delay(1);
      digitalWrite(MZ_RST,0);
      digitalWrite(MZ_EN,0);
      isEmergency=false;
      finishedMsg();
      break;
    case EMERGENCY:
      digitalWrite(M1_RUN,0);
      digitalWrite(M2_RUN,1);
      digitalWrite(MZ_EN,1);
      isEmergency=true;
      isHomed=false;
      emergencyMsg();
      finishedMsg();
      break;
    default:
      return false;
  }
  return true;
}

bool readMsg(bool* _status)
{
  char msg[3];
  int i=0;
  while(i<3)
  {
    if(Serial1.available())
    {
      msg[i]=Serial1.read();
      i++;
    }
  }
  int c=0;
  if(msg[c]!='s')
    return false;
  c++;
  switch(msg[c])
  {
    case '1':
    *_status=true;
    break;
    case '0':
    *_status=false;
    break;
    default:
    return false;
    break;
  }
  c++;
  if(msg[c]!='!')
    return false;
  return true;
}

void unrelated(Point2D _point, float _vel,AccelStepper* motor1,AccelStepper* motor2)
{
  float velBeta, velAlpha, tAccAlpha, tAccBeta, tTot, tAlpha, tBeta;
  int  stepAlphaTot, stepBetaTot, stepAlpha = 0, stepBeta = 0, deltaBeta, deltaAlpha;
  calcAngles(_point, &deltaAlpha, &deltaBeta);
  deltaAlpha = deltaAlpha - alpha.mainAngle;
  deltaBeta = deltaBeta - beta.mainAngle;
  stepAlphaTot=absolute(deltaAlpha);
  stepBetaTot=absolute(deltaBeta);
  isMovment = true;
  searchTime(_vel,stepAlphaTot,LENGTH1,alpha.dAngle,&tAlpha);
  searchTime(_vel,stepBetaTot,LENGTH2,beta.dAngle,&tBeta);
  
  if(tAlpha>tBeta)
  {
      velAlpha=_vel;
      tTot=tAlpha;
      searchVMax(tTot,stepBetaTot,beta.dAngle,LENGTH2,_vel,&velBeta);
  }else
  {
      velBeta=_vel;
      tTot=tBeta;
      searchVMax(tTot,stepAlphaTot,alpha.dAngle,LENGTH1,_vel,&velAlpha);
  }
  
  if (deltaAlpha < 0)
  {
    alpha.mainAngle-=stepAlphaTot;
    stepAlphaTot*=-1;
  } else
  {
    alpha.mainAngle+=stepAlphaTot;
  }
  if (deltaBeta < 0)
  {
    beta.mainAngle-=stepBetaTot;
    stepBetaTot*=-1;
  } else
  {
    beta.mainAngle+=stepBetaTot;
  }
  motor1->setAcceleration(acceleration/LENGTH1/alpha.dAngle);
  motor1->setMaxSpeed(velAlpha/LENGTH1/alpha.dAngle);
  motor1->move(stepAlphaTot);
  motor2->setAcceleration(acceleration/LENGTH2/beta.dAngle);
  motor2->setMaxSpeed(velBeta/LENGTH2/beta.dAngle);
  motor2->move(stepBetaTot);
  while(motor1->isRunning()||motor2->isRunning())
  {
    motor1->run();
    motor2->run();
  }
  calcPoint(alpha.mainAngle,beta.mainAngle,&actualPoint);
}

void searchTime(float _vMax, unsigned int _stepTot, int _length, float _dAngle, float* _tTot)
{
    float s=_dAngle*(float)_length, tAcc, sAcc;
    unsigned int step=0;
    tAcc=_vMax/acceleration;
    
    sAcc=0.5*acceleration*tAcc*tAcc;
    
    if(2*sAcc<_stepTot*s)
    {
      *_tTot=2*tAcc+(_stepTot*s-2*sAcc)/_vMax;
    }else{
      sAcc=_stepTot*s/2;
      *_tTot=2*sqrt(2*sAcc/acceleration);
    }
}

void searchVMax(float _tTot, unsigned int _stepTot, float _dAngle, int _length, int _vel, float* _vMax)
{
    float a,b,c, s=_dAngle*(float)_length, v1, v2;
    a=1/acceleration;
    b=-_tTot;
    c=_stepTot*s;
    v1=solveQE(a,b,c,true);
    v2=solveQE(a,b,c,false);
    
    if(v1>0&&v1<_vel)
    {
      *_vMax = v1;
    }else
    {
      *_vMax=v2;
    }
}

void homing(AccelStepper* motor1, AccelStepper* motor2)
{
  Serial1.print("sO7|u!");
  delay(10);
  while(digitalRead(MZ_MOVING))
  {
    if(isEmergency)
      return;
  }
  delay(10);
  while(digitalRead(MZ_MOVING))
  {
    if(isEmergency)
      return;
  }
  Serial1.print("sO7|v!");
  homeAsse1(motor1);
  homeAsse2(motor2);
  actualPoint.x = -136.3135;
  actualPoint.y = 113.9765;
  alpha.mainAngle = 25000;
  beta.mainAngle = 1794;
  isHomed = true;
}

void homeAsse1(AccelStepper* motor1)
{
  
   motor1->setMaxSpeed(1250);
  motor1->setSpeed(2500);
  while (!digitalRead(PROXY_M1)){
    motor1->runSpeed();
    if(isEmergency)
      return;
  }
  motor1->move(-250);
  motor1->setAcceleration(10000);
  while(motor1->isRunning())
  {
    motor1->run();
    if(isEmergency)
      return;
  }
  motor1->setSpeed(1250);
  while (!digitalRead(PROXY_M1))
  {
    motor1->runSpeed();
    if(isEmergency)
      return;
  }
  motor1->move(160);
  while(motor1->isRunning())
  {
    motor1->run();
    if(isEmergency)
      return;
  }
}

void homeAsse2(AccelStepper* motor2)
{
  motor2->setMaxSpeed(400);
  motor2->setSpeed(-1000);
  while (!digitalRead(PROXY_M2))
  {
    motor2->runSpeed();
    if(isEmergency)
      return;
  }
  motor2->move(200);
  motor2->setAcceleration(10000);
  while(motor2->isRunning())
  {
    motor2->run();
    if(isEmergency)
      return;
  }
  motor2->setSpeed(-400);
  while (!digitalRead(PROXY_M2))
  {
    motor2->runSpeed();
    if(isEmergency)
      return;
  }
  motor2->move(-140);
  while(motor2->isRunning())
  {
    motor2->run();
    if(isEmergency)
      return;
  }
}
