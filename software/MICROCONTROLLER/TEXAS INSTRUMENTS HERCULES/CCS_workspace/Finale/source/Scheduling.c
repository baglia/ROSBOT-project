#include "Scheduling.h"

bool interpreter(Queue* _cmds, Queue* _queue1, Queue* _queue2, Queue* _queue3)
{
  Command* _cmd;
  int step1=-12500, step2=7580, errAsse1, errAsse2;
  if (_cmds->index == -1)
    return false;
  _cmd = (Command*)popQueue(_cmds);
  switch ((int)_cmd->type)
  {
    case LINEAR_MOV:    //esegue una movimentazione lineare dal punto attuale a quello desiderato
        if(isEmergency)
        {
            emergencyMsg();
            return false;
        }
        if (!isHomed)
            homing();
      startCalcLoop(_cmd, _queue1, _queue2);
      finishedMsg();
      break;
    case CIRC_OR_MOV:

      break;
    case CIRC_AOR_MOV:

      break;
    case UNRELATED: //esegue una movimentazione da un punto ad un altro senza seguire alcuna traiettoria specifica
        if(isEmergency)
              {
                  emergencyMsg();
                  return false;
              }
        if (!isHomed)
            homing();
        unrelated(point2D(_cmd->point),_cmd->parameter,_queue1,_queue2);
        finishedMsg();
      break;
    case JOINT:
      if(isEmergency)
      {
          emergencyMsg();
          return false;
      }
      asse1Step(_cmd->point.x / 360 * 50000,_cmd->parameter,_queue1);
      asse2Step(_cmd->point.y / 360 * 25000, _cmd->parameter,_queue2);
      calcPoint(alpha.mainAngle, beta.mainAngle, &actualPoint);
      finishedMsg();
      break;
    case PROG:  //invia un impulso al motore Z per eseguire il programma
      pwmStart(hetRAM1, pwm5);
      pwmEnableNotification(hetREG1, pwm5, pwmEND_OF_DUTY);
      finishedMsg();
      break;
    case GRIPPER:   //fa aprire o chiudere il gripper
      if (_cmd->parameter == 0)
      {
        //sciSend(sciREG3, , ""); //manda il messaggio alla ciabatta arduino per attivare l'elettrovalvola
      } else
      {
        //sciSend(sciREG3, , ""); //manda il messaggio alla ciabatta arduino per disattivare l'elettrovalvola
      }
      break;
    case HOMING:    //esegue l'homing dei motori
        if(isEmergency)
              {
                  emergencyMsg();
                  return false;
              }
      homing();
      finishedMsg();
      break;
    case NUL:

      break;
    case PAUSE:

      break;
    case IF:

      break;
    case V_MIN:

      break;
    case RESET:
      reset();
      isEmergency=false;
      finishedMsg();
      break;
    case EMERGENCY:
      emergenza();
      isEmergency=true;
      emergencyMsg();
      finishedMsg();
      break;
    case S_TEST:
        homing();
        asse1Step(step1,100,_queue1);   //velocità posizionamento 50 mm/s
        asse2Step(step2,100,_queue2);
        int i;                  //test asse2
        step2=6250;
        for(i=0;i<50;i++)
        {
            asse2Step(step2,300,_queue2);
            step2*=-1;
        }
        errAsse2=homeAsse2();
        if(7580==-errAsse2)
        {
            char msgAsse2P[]={"asse 2 passato"};
            sciSend(sciREG1,14,(uint8*)msgAsse2P);
        }else
        {
            char msgAsse2NP[]={"asse 2 fallito"};
            sciSend(sciREG1,14,(uint8*)msgAsse2NP);
        }
        asse2Step(10705,50,_queue2);
        step1=6250;
        asse1Step(step1,50,_queue1);
        step1=-12500;
        for(i=0;i<50;i++)       //test asse 1
        {
            asse1Step(step1,300,_queue1);
            step1*=-1;
        }
        errAsse1=homeAsse1();
        if(6250==errAsse1)
        {
            char msgAsse1P[]={"asse 1 passato"};
            sciSend(sciREG1,14,(uint8*)msgAsse1P);
        }else
        {
            char msgAsse1NP[]={"asse 1 fallito"};
            sciSend(sciREG1,14,(uint8*)msgAsse1NP);
        }
        break;
    case SET_ACC:
        acceleration=_cmd->parameter;
        break;
    default:
      free(_cmd);
      return false;
  }
  free(_cmd);
  return true;
}

void unrelated(Point2D _point, float _vel, Queue* _periods1, Queue* _periods2)
{
  float velAlpha, velBeta, tAccAlpha, tAccBeta, tTot, tAlpha, tBeta;
  int  stepAlphaTot, stepBetaTot, stepAlpha = 0, stepBeta = 0, deltaAlpha, deltaBeta;
  Period period1, period2;
  Trapezoidal lowAlpha, lowBeta;
  hetSIGNAL_t signal;
  signal.duty = 40;
  calcAngles(_point, &deltaAlpha, &deltaBeta);
  deltaAlpha = deltaAlpha - alpha.mainAngle;
  deltaBeta = deltaBeta - beta.mainAngle;
  stepAlphaTot=absolute(deltaAlpha);
  stepBetaTot=absolute(deltaBeta);
  isMovment = true;
  searchTime(_vel,stepAlphaTot,LENGTH1,alpha.dAngle,&tAccAlpha,&tAlpha,&velAlpha);
  searchTime(_vel,stepBetaTot,LENGTH2,beta.dAngle,&tAccBeta,&tBeta,&velBeta);
  if(tAlpha>tBeta)
  {
      tTot=tAlpha;
      searchVMax(tTot,stepBetaTot,beta.dAngle,LENGTH2,&velBeta,&tAccBeta);
  }else
  {
      tTot=tBeta;
      searchVMax(tTot,stepAlphaTot,alpha.dAngle,LENGTH1,&velAlpha,&tAccAlpha);
  }
  setTrapezoidal(velAlpha,tTot,tAccAlpha,&lowAlpha);
  setTrapezoidal(velBeta,tTot,tAccBeta,&lowBeta);


  calcPoint(alpha.mainAngle,beta.mainAngle,&actualPoint);
  alpha.mainAngle+=deltaAlpha;
  beta.mainAngle+=deltaBeta;
  if (deltaAlpha < 0)
  {
    period1.isCW = true;
  } else
  {
    period1.isCW = false;
  }
  if (deltaBeta < 0)
  {
    period2.isCW = true;
  } else
  {
    period2.isCW = false;
  }
  period1.isPeriod = true;
  period1.isEnd = false;
  period2.isPeriod = true;
  period2.isEnd = false;
  tAlpha=0;
  tBeta=0;
  period1.period = 1000000*alpha.dAngle*LENGTH1/getTrapezoidalSpeed(tAlpha,lowAlpha);
  period2.period = 1000000*beta.dAngle*LENGTH2/getTrapezoidalSpeed(tBeta,lowBeta);
  stepAlpha++;
  stepBeta++;
  tAlpha+=(float)period1.period/1000000;
  tBeta+=(float)period2.period/1000000;
  signal.period = period1.period;
  if (!period1.isCW)
  {
    pwmSetSignal(hetRAM1, M1_CCW, signal);
    pwmStart(hetRAM1, M1_CCW);
    pwmEnableNotification(hetREG1, M1_CCW, pwmEND_OF_DUTY);
  } else
  {
    pwmSetSignal(hetRAM1, M1_CW, signal);
    pwmStart(hetRAM1, M1_CW);
    pwmEnableNotification(hetREG1, M1_CW, pwmEND_OF_DUTY);
  }
  signal.period = period2.period;
  if (!period2.isCW)
  {
    pwmSetSignal(hetRAM1, M2_CCW, signal);
    pwmStart(hetRAM1, M2_CCW);
    pwmEnableNotification(hetREG1, M2_CCW, pwmEND_OF_DUTY);
  } else
  {
    pwmSetSignal(hetRAM1, M2_CW, signal);
    pwmStart(hetRAM1, M2_CW);
    pwmEnableNotification(hetREG1, M2_CW, pwmEND_OF_DUTY);
  }
  while (1) {
        if(_periods1->index<(int)_periods1->size-1)
        {
      if (stepAlpha < abs(stepAlphaTot))
      {
        period1.period = 1000000*alpha.dAngle*LENGTH1/getTrapezoidalSpeed(tAlpha,lowAlpha);
        pushQueue(_periods1, (int*)&period1);
        stepAlpha++;
        tAlpha+=(float)period1.period/1000000;
      } else
      {
        period1.isEnd = true;
        pushQueue(_periods1, (int*)&period1);
      }
        }
        if(_periods2->index<(int)_periods2->size-1)
      if (stepBeta < abs(stepBetaTot))
      {
        period2.period =1000000*beta.dAngle*LENGTH2/getTrapezoidalSpeed(tBeta,lowBeta);
        pushQueue(_periods2, (int*)&period2);
        stepBeta++;
        tBeta+=(float)period2.period/1000000;
      } else
      {
        period2.isEnd = true;
        pushQueue(_periods2, (int*)&period2);
      }
      if (stepAlpha >= abs(stepAlphaTot) && stepBeta >= abs(stepBetaTot))
        return;
    }
}

void searchTime(float _vMax, unsigned int _stepTot, int _length, float _dAngle, float* _tAcc, float* _tTot, float* _velReal)
{
    float vel=minSpeed, period=0, s=_dAngle*(float)_length, time=0;
    unsigned int step=0;
    while(vel<_vMax&&step<_stepTot/2)
    {
        step++;
        period=micronRound(s/vel,1000000);
        time+=period;
        vel+=acceleration*period;
    }
    period=s/vel;
    *_tAcc=time+period;
    *_velReal=vel;
    *_tTot=time*2+s*((int)_stepTot-2*step)/vel;
}

void searchVMax(float _tTot, unsigned int _stepTot, float _dAngle, int _length, float* _vMax, float* _tAcc)
{
    float time=0, s=_dAngle*(float)_length, vel=minSpeed,period;
    unsigned int step=0;
    while(2*time+s*(_stepTot-2*step)/vel>_tTot)
    {
        step++;
        period=micronRound(s/vel,1000000);
        time+=period;
        vel+=acceleration*period;
    }
    period=s/vel;
    *_vMax=vel;
    *_tAcc=time+period;

}
void calcTrapezoidal(float _maxSpeed, float _tTot,float _sTot, Trapezoidal* _low)
{
    float vel1,vel2,tAcc,sAcc,vel;
    vel1=solveQE(1,-_tTot*acceleration-2*minSpeed,acceleration*absolute(_sTot)+pow(minSpeed,2),true);
    vel2=solveQE(1,-_tTot*acceleration-2*minSpeed,acceleration*absolute(_sTot)+pow(minSpeed,2),false);
    if(vel1>0&&vel1<_maxSpeed)
    {
        vel=vel1;
    }else
    {
        vel=vel2;
    }
    tAcc=(vel-minSpeed)/acceleration;
    sAcc=minSpeed*tAcc+0.5*acceleration*pow(tAcc,2);
    if(2*sAcc>_sTot)
    {
        sAcc=_sTot/2;
        float t1,t2;
        t1=solveQE(0.5*acceleration,minSpeed,-sAcc,true);
        t2=solveQE(0.5*acceleration,minSpeed,-sAcc,false);
        if(t1>0&&t1>t2)
        {
            tAcc=t1;
        }else
        {
            tAcc=t2;
        }
        setTrapezoidal(_maxSpeed,tAcc*2,tAcc,_low);
    }else
    {
        setTrapezoidal(vel,_tTot,tAcc,_low);
    }
}
void homing(void)
{
  isMovment = false;
  homeAsse1();
  homeAsse2();
  actualPoint.x = -136.3135;
  actualPoint.y = 113.9765;
  alpha.mainAngle = 25000;
  beta.mainAngle = 26.84/180*12500;
  isHomed = true;
}

int homeAsse1(void)
{
  isMovment = false;
  int step=0;
  hetSIGNAL_t signal;
  signal.duty = 40;
  signal.period = 400;
  iteration = 0;
  pwmEnableNotification(hetREG1, M1_CCW, pwmEND_OF_PERIOD);
  pwmSetSignal(hetRAM1, M1_CCW, signal);
  pwmStart(hetRAM1, M1_CCW);
  while (!gioGetBit(gioPORTA, PROXY_M1));
  pwmStop(hetRAM1, M1_CCW);
  pwmDisableNotification(hetREG1, M1_CCW, pwmEND_OF_PERIOD);
  step+=iteration;
  signal.period = 800;
  pwmSetSignal(hetRAM1, M1_CW, signal);
  iteration = 0;
  pwmEnableNotification(hetREG1, M1_CW, pwmEND_OF_PERIOD);
  pwmStart(hetRAM1, M1_CW);
  while (iteration < 250);
  pwmStop(hetRAM1, M1_CW);
  pwmDisableNotification(hetREG1, M1_CW, pwmEND_OF_PERIOD);
  step-=iteration;
  iteration = 0;
  pwmEnableNotification(hetREG1, M1_CCW, pwmEND_OF_PERIOD);
  pwmSetSignal(hetRAM1, M1_CCW, signal);
  pwmStart(hetRAM1, M1_CCW);
  while (!gioGetBit(gioPORTA, PROXY_M1));
  step+=iteration;
  iteration=0;
  while (iteration < 160);
  pwmStop(hetRAM1, M1_CCW);
  pwmDisableNotification(hetREG1, M1_CCW, pwmEND_OF_PERIOD);
  step+=iteration;
  return step;
}

int homeAsse2(void)
{
  isMovment = false;
  int step=0;
  hetSIGNAL_t signal;
  signal.duty = 40;
  signal.period = 1000;
  iteration = 0;
  pwmEnableNotification(hetREG1, M2_CW, pwmEND_OF_PERIOD);
  pwmSetSignal(hetRAM1, M2_CW, signal);
  pwmStart(hetRAM1, M2_CW);
  while (!gioGetBit(gioPORTA, PROXY_M2));
  pwmStop(hetRAM1, M2_CW);
  pwmDisableNotification(hetREG1, M2_CW, pwmEND_OF_PERIOD);
  step-=iteration;
  signal.period = 2500;
  pwmSetSignal(hetRAM1, M2_CCW, signal);
  iteration = 0;
  pwmEnableNotification(hetREG1, M2_CCW, pwmEND_OF_PERIOD);
  pwmStart(hetRAM1, M2_CCW);
  while (iteration < 300);
  pwmStop(hetRAM1, M2_CCW);
  pwmDisableNotification(hetREG1, M2_CCW, pwmEND_OF_PERIOD);
  step+=iteration;
  iteration = 0;
  pwmEnableNotification(hetREG1, M2_CW, pwmEND_OF_PERIOD);
  pwmSetSignal(hetRAM1, M2_CW, signal);
  pwmStart(hetRAM1, M2_CW);
  while (!gioGetBit(gioPORTA, PROXY_M2));
  step-=iteration;
  iteration=0;
  while (iteration < 140);
  pwmStop(hetRAM1, M2_CW);
  pwmDisableNotification(hetREG1, M2_CW, pwmEND_OF_PERIOD);
  step-=iteration;
  return step;
}

void asse1Step(int _step, int _velMax, Queue* _periods)
{
  hetSIGNAL_t signal;
  signal.duty=40;
  Period period;
  isMovment = true;
  if(_step!=0)
  {
      Trapezoidal law;
      float vel, tTot,tAcc;
      if (_step < 0)
      {
          searchTime(_velMax,-_step,LENGTH1,alpha.dAngle,&tAcc,&tTot,&vel);
          setTrapezoidal(vel,tTot,tAcc,&law);
          period.isCW = true;
      } else
      {
          searchTime(_velMax,_step,LENGTH1,alpha.dAngle,&tAcc,&tTot,&vel);
          setTrapezoidal(vel,tTot,tAcc,&law);
          period.isCW = false;
      }
      period.isPeriod = true;
      period.isEnd = false;
      float t=0;
      int step=0;
      period.period = 1000000*alpha.dAngle*LENGTH1/getTrapezoidalSpeed(t,law);
      step++;
      t+=(float)period.period/1000000;
      signal.period = period.period;
      if (!period.isCW)
      {
          pwmSetSignal(hetRAM1, M1_CCW, signal);
          pwmStart(hetRAM1, M1_CCW);
          pwmEnableNotification(hetREG1, M1_CCW, pwmEND_OF_DUTY);
      } else
      {
          pwmSetSignal(hetRAM1, M1_CW, signal);
          pwmStart(hetRAM1, M1_CW);
          pwmEnableNotification(hetREG1, M1_CW, pwmEND_OF_DUTY);
      }
      while (1) {
          if(_periods->index<(int)_periods->size-1)
          {
        if (step < abs(_step))
        {
          period.period = 1000000*alpha.dAngle*LENGTH1/getTrapezoidalSpeed(t,law);
          pushQueue(_periods, (int*)&period);
          step++;
          t+=(float)period.period/1000000;
        } else
        {
          period.isEnd = true;
          pushQueue(_periods, (int*)&period);
          break;
        }
          }
      }
  }
  alpha.mainAngle+=_step;
}

void asse2Step(int _step, int _velMax, Queue* _periods)
{
    hetSIGNAL_t signal;
    signal.duty=40;
    Period period;
    isMovment = true;
    if(_step!=0)
    {
        Trapezoidal law;
        float vel, tTot,tAcc;
        if (_step < 0)
        {
            searchTime(_velMax,-_step,LENGTH2,beta.dAngle,&tAcc,&tTot,&vel);
            setTrapezoidal(vel,tTot,tAcc,&law);
            period.isCW = true;
        } else
        {
            searchTime(_velMax,_step,LENGTH2,beta.dAngle,&tAcc,&tTot,&vel);
            setTrapezoidal(vel,tTot,tAcc,&law);
            period.isCW = false;
        }
        period.isPeriod = true;
        period.isEnd = false;
        float t=0;
        int step=0;
        period.period = 1000000*beta.dAngle*LENGTH2/getTrapezoidalSpeed(t,law);
        step++;
        t+=(float)period.period/1000000;
        signal.period = period.period;
        if (!period.isCW)
        {
            pwmSetSignal(hetRAM1, M2_CCW, signal);
            pwmStart(hetRAM1, M2_CCW);
            pwmEnableNotification(hetREG1, M2_CCW, pwmEND_OF_DUTY);
        } else
        {
            pwmSetSignal(hetRAM1, M2_CW, signal);
            pwmStart(hetRAM1, M2_CW);
            pwmEnableNotification(hetREG1, M2_CW, pwmEND_OF_DUTY);
        }
        while (1) {
            if(_periods->index<(int)_periods->size-1)
            {
                if (step < abs(_step))
                {
                    period.period = 1000000*beta.dAngle*LENGTH2/getTrapezoidalSpeed(t,law);
                    pushQueue(_periods, (int*)&period);
                    step++;
                    t+=(float)period.period/1000000;
                } else
                {
                    period.isEnd = true;
                    pushQueue(_periods, (int*)&period);
                    break;
                }
            }
        }
    }
    beta.mainAngle+=_step;
}

bool startCalcLoop(Command* _cmd, Queue* _queue1, Queue* _queue2)
{
  Cicloydal low;
  ParameterLinearMovment settings;
  setCicloydal(_cmd->parameter, pointDistance(actualPoint, point2D(_cmd->point)), &low);
  setLinMovment(actualPoint, point2D(_cmd->point), low, &settings);
  calcLoopLinear(settings, _queue1, _queue2);
  return true;
}

bool calcLoopLinear(ParameterLinearMovment _movment, Queue* _queue1, Queue* _queue2)
{
  Period period1, period2;
  hetSIGNAL_t signal;
  signal.duty = 40;
  _disable_IRQ_interrupt_();
  nextPeriodA(&period1, _movment, true);
  nextPeriodB(&period2, _movment, true);
  signal.period = period1.period;
  if (!period1.isCW)
  {
    pwmSetSignal(hetRAM1, M1_CCW, signal);
    pwmStart(hetRAM1, M1_CCW);
    pwmEnableNotification(hetREG1, M1_CCW, pwmEND_OF_DUTY);
  } else
  {
    pwmSetSignal(hetRAM1, M1_CW, signal);
    pwmStart(hetRAM1, M1_CW);
    pwmEnableNotification(hetREG1, M1_CW, pwmEND_OF_DUTY);
  }
  signal.period = period2.period;
  if (!period2.isCW)
  {
    pwmSetSignal(hetRAM1, M2_CCW, signal);
    pwmStart(hetRAM1, M2_CCW);
    pwmEnableNotification(hetREG1, M2_CCW, pwmEND_OF_DUTY);
  } else
  {
    pwmSetSignal(hetRAM1, M2_CW, signal);
    pwmStart(hetRAM1, M2_CW);
    pwmEnableNotification(hetREG1, M2_CW, pwmEND_OF_DUTY);
  }
  while (1)
  {
    if ((int)(_queue1->size - 1) > _queue1->index)
    {
      nextPeriodA(&period1, _movment, true);
      pushQueue(_queue1, (int*)&period1);
    }
    if ((int)(_queue2->size - 1) > _queue2->index)
    {
      nextPeriodB(&period2, _movment, true);
      pushQueue(_queue2, (int*)&period2);
    }
    if (period1.isEnd && period2.isEnd)
      break;
  }
  _enable_IRQ_interrupt_();
  return true;
}
