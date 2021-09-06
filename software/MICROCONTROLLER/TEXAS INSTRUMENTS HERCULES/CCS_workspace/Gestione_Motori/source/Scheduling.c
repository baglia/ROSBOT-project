#include "Scheduling.h"

bool interpreter(Queue* _cmds, Queue* _queue1, Queue* _queue2, Queue* _queue3)
{
  Command* _cmd;

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
      asse1Step(_cmd->point.x / 360 * 50000, 20 * LENGTH1 / _cmd->parameter);
      asse2Step(_cmd->point.y / 360 * 25000, 40 * LENGTH2 / _cmd->parameter);
      alpha.mainAngle += _cmd->point.x / 180 * PI;
      beta.mainAngle += _cmd->point.y / 180 * PI;
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
      gioSetBit(gioPORTA, 3, 1);
      gioSetBit(gioPORTA, 0, 1);
      gioSetBit(gioPORTB, 1, 1);
      isEmergency=false;
      finishedMsg();
      break;
    case EMERGENCY:
      gioSetBit(gioPORTA, 3, 0);
      gioSetBit(gioPORTA, 0, 0);
      gioSetBit(gioPORTB, 1, 0);
      isEmergency=true;
      emergencyMsg();
      finishedMsg();
      break;
    default:
      return false;
  }
  return true;
}

void unrelated(Point2D _point, float _vel, Queue* _periods1, Queue* _periods2)
{
  float deltaAlpha, deltaBeta, velAlpha, velBeta;
  int stepAlphaTot, stepBetaTot, stepAlpha = 0, stepBeta = 0;
  Period period1, period2;
  Cicloydal lowAlpha, lowBeta;
  hetSIGNAL_t signal;
  signal.duty = 40;
  calcAngles(_point, &deltaAlpha, &deltaBeta);
  deltaAlpha = setAngle(deltaAlpha - alpha.mainAngle);
  deltaBeta = setAngle(deltaBeta - beta.mainAngle);
  isMovment = true;
  if (absolute(deltaAlpha) > absolute(deltaBeta))
  {
    velAlpha = _vel;
    velBeta = velAlpha / deltaAlpha * deltaBeta/LENGTH1*LENGTH2;
  } else
  {
    velBeta = _vel;
    velAlpha = velBeta / deltaBeta * deltaAlpha/LENGTH2*LENGTH1;

  }
  stepAlphaTot = deltaAlpha / PI * 25000;
  stepBetaTot = deltaBeta / PI * 12500;
  alpha.mainAngle+=(float)stepAlphaTot/25000.0*PI;
  beta.mainAngle+=(float)stepBetaTot/12500.0*PI;
  calcPoint(alpha.mainAngle,beta.mainAngle,&actualPoint);
  setCicloydal(absolute(velAlpha), abs(stepAlphaTot), &lowAlpha);
  setCicloydal(absolute(velBeta), abs(stepBetaTot), &lowBeta);
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
  period1.period = 20*LENGTH1/getCicloydalSpeed(stepAlpha, lowAlpha);
  period2.period = 40*LENGTH2/getCicloydalSpeed(stepBeta, lowBeta);
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
      period1.period = 20*LENGTH1/getCicloydalSpeed(stepAlpha, lowAlpha);
      pushQueue(_periods1, (int*)&period1);
      stepAlpha++;
    } else
    {
      period1.isEnd = true;
      pushQueue(_periods1, (int*)&period1);
    }
      }
      if(_periods2->index<(int)_periods2->size-1)
    if (stepBeta < abs(stepBetaTot))
    {
      period2.period = 40*LENGTH2/getCicloydalSpeed(stepBeta, lowBeta);
      pushQueue(_periods2, (int*)&period2);
      stepBeta++;
    } else
    {
      period2.isEnd = true;
      pushQueue(_periods2, (int*)&period2);
    }
    if (stepAlpha >= abs(stepAlphaTot) && stepBeta >= abs(stepBetaTot))
      return;
  }
}

void homing(void)
{
  isMovment = false;
  homeAsse1();
  homeAsse2();
  actualPoint.x = -136.3135;
  actualPoint.y = 113.9765;
  alpha.mainAngle = PI;
  beta.mainAngle = 26 * PI / 180.0;
  isHomed = true;
}

void homeAsse1(void)
{
  hetSIGNAL_t signal;
  signal.duty = 40;
  signal.period = 400;
  pwmSetSignal(hetRAM1, M1_CCW, signal);
  pwmStart(hetRAM1, M1_CCW);
  while (!gioGetBit(gioPORTA, PROXY_M1));
  pwmStop(hetRAM1, M1_CCW);
  signal.period = 800;
  pwmSetSignal(hetRAM1, M1_CW, signal);
  iteration = 0;
  pwmEnableNotification(hetREG1, M1_CW, pwmEND_OF_PERIOD);
  pwmStart(hetRAM1, M1_CW);
  while (iteration < 250);
  pwmStop(hetRAM1, M1_CW);
  pwmDisableNotification(hetREG1, M1_CW, pwmEND_OF_PERIOD);
  iteration = 0;
  pwmSetSignal(hetRAM1, M1_CCW, signal);
  pwmStart(hetRAM1, M1_CCW);
  while (!gioGetBit(gioPORTA, PROXY_M1));
  pwmEnableNotification(hetREG1, M1_CCW, pwmEND_OF_PERIOD);
  while (iteration < 160);
  pwmStop(hetRAM1, M1_CCW);
  pwmDisableNotification(hetREG1, M1_CCW, pwmEND_OF_PERIOD);
}

void homeAsse2(void)
{
  hetSIGNAL_t signal;
  signal.duty = 40;
  signal.period = 1000;
  pwmSetSignal(hetRAM1, M2_CW, signal);
  pwmStart(hetRAM1, M2_CW);
  while (!gioGetBit(gioPORTA, PROXY_M2));
  pwmStop(hetRAM1, M2_CW);
  signal.period = 2500;
  pwmSetSignal(hetRAM1, M2_CCW, signal);
  iteration = 0;
  pwmEnableNotification(hetREG1, M2_CCW, pwmEND_OF_PERIOD);
  pwmStart(hetRAM1, M2_CCW);
  while (iteration < 300);
  pwmStop(hetRAM1, M2_CCW);
  pwmDisableNotification(hetREG1, M2_CCW, pwmEND_OF_PERIOD);
  iteration = 0;
  pwmSetSignal(hetRAM1, M2_CW, signal);
  pwmStart(hetRAM1, M2_CW);
  while (!gioGetBit(gioPORTA, PROXY_M2));
  pwmEnableNotification(hetREG1, M2_CW, pwmEND_OF_PERIOD);
  while (iteration < 140);
  pwmStop(hetRAM1, M2_CW);
  pwmDisableNotification(hetREG1, M2_CW, pwmEND_OF_PERIOD);
}

void asse1Step(int _step, unsigned int period)
{
  hetSIGNAL_t signal;
  signal.duty = 40;
  signal.period = period;
  iteration = 0;
  if (_step < 0)
  {
    pwmSetSignal(hetRAM1, M1_CW, signal);
    pwmEnableNotification(hetREG1, M1_CW, pwmEND_OF_PERIOD);
    pwmStart(hetRAM1, M1_CW);
    while (iteration < -_step);
    pwmStop(hetRAM1, M1_CW);
    pwmDisableNotification(hetREG1, M1_CW, pwmEND_OF_PERIOD);
  } else
  {
    pwmSetSignal(hetRAM1, M1_CCW, signal);
    pwmEnableNotification(hetREG1, M1_CCW, pwmEND_OF_PERIOD);
    pwmStart(hetRAM1, M1_CCW);
    while (iteration < _step);
    pwmStop(hetRAM1, M1_CCW);
    pwmDisableNotification(hetREG1, M1_CCW, pwmEND_OF_PERIOD);
  }
}

void asse2Step(int _step, unsigned int period)
{
  hetSIGNAL_t signal;
  signal.duty = 40;
  signal.period = period;
  iteration = 0;
  if (_step < 0)
  {
    pwmSetSignal(hetRAM1, M2_CW, signal);
    pwmEnableNotification(hetREG1, M2_CW, pwmEND_OF_PERIOD);
    pwmStart(hetRAM1, M2_CW);
    while (iteration < -_step);
    pwmStop(hetRAM1, M2_CW);
    pwmDisableNotification(hetREG1, M2_CW, pwmEND_OF_PERIOD);
  } else
  {
    pwmSetSignal(hetRAM1, M2_CCW, signal);
    pwmEnableNotification(hetREG1, M2_CCW, pwmEND_OF_PERIOD);
    pwmStart(hetRAM1, M2_CCW);
    while (iteration < _step);
    pwmStop(hetRAM1, M2_CCW);
    pwmDisableNotification(hetREG1, M2_CCW, pwmEND_OF_PERIOD);
  }
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
