/** @file HL_sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com  
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
#include "HL_sci.h"
#include "Library.h"
#include "HL_sys_vim.h"
#include "math.h"
#include "Scheduling.h"
#include "GlobalVar.h"
#include "HL_sys_core.h"
#include "HL_het.h"
#include "HL_rti.h"
#include "HL_gio.h"
/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"

/* USER CODE BEGIN (1) */
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
hetSIGNAL_t signal;
int tolerance = 1, iteration = 0, index = 0;
MotorAngle alpha, beta, betaAbs;
Point2D origin, actualPoint;
float maxSpeed = 1000, maxAccel = 50, minSpeed = 5, acceleration=500;
Queue commands, periods1, periods2, periods3;
bool isMovment = false, isEmergenza= false, isRead=false,isSent=false, isHomed=false, isFinished=true, isEmergency=false;

void setup(void);
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
  setup();
  while (1)
  {
   reader(&commands);
   interpreter(&commands, &periods1, &periods2, &periods3);
  }
  /* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void setup(void)    //setta tutti i valori della scheda nello stato iniziale
{
    queueInitializer(&commands, 10, sizeof(Command));
      queueInitializer(&periods1, 100, sizeof(unsigned int));
      queueInitializer(&periods2, 100, sizeof(unsigned int));
      queueInitializer(&periods3, 100, sizeof(unsigned int));
      alpha.dAngle = 2 * PI / 50000;
      beta.dAngle = 2 * PI / 25000;
      signal.duty = 40;
      hetInit();
      rtiInit();
      sciInit();
      gioInit();
      while(sciIsRxReady(sciREG1))
          sciReceiveByte(sciREG1);
      char message[]={"sO2,3,4,5,6|v!"};
      sciSend(sciREG3,14,(uint8*)message);
      reset();
      _enable_interrupt_();
}

void gioNotification(gioPORT_t *port, uint32 bit)
{
/* USER CODE BEGIN (22) */
    switch((int)port)
    {
    case (int)gioPORTA:

        break;
    case (int)gioPORTB:
        switch(bit)
        {
        case 3:
            if(!gioGetBit(gioPORTB,3))
            {
                emergenza();
                while(!gioGetBit(gioPORTB,3));
                isEmergency=true;
                isHomed=false;
                emergencyMsg();
            }
            break;
        }
        break;
    }
}



void pwmNotification(hetBASE_t * hetREG, uint32 pwm, uint32 notification)
{
  /*  enter user code between the USER CODE BEGIN and USER CODE END. */
  /* USER CODE BEGIN (38) */
  switch (notification)
  {
    case pwmEND_OF_DUTY:
      switch (pwm)
      {
        case M1_CCW:
          if (periods1.index < 0)
          {
            pwmStop(hetRAM1, M1_CCW);
            pwmDisableNotification(hetREG1,M1_CCW,pwmEND_OF_BOTH);
            rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK0);
            rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE0);
          } else
          {
            Period* popped = (Period*)popQueue(&periods1);
            if (popped->isPeriod)
            {
              if (popped->isEnd)
              {
                pwmStop(hetRAM1, M1_CCW);
              } else
              {
                signal.period = popped->period;
                if (!popped->isCW)
                {
                  pwmSetSignal(hetRAM1, M1_CCW, signal);
                } else
                {
                  pwmStop(hetRAM1, M1_CCW);
                  pwmEnableNotification(hetREG1, M1_CCW, pwmEND_OF_PERIOD);
                  pwmSetSignal(hetRAM1, M1_CW, signal);
                }
              }

            }
          }
          break;
        case M1_CW:
          if (periods1.index < 0)
          {
            pwmStop(hetRAM1, M1_CW);
            pwmDisableNotification(hetREG1,M1_CW,pwmEND_OF_BOTH);
            rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK0);
            rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE0);
          } else
          {
            Period* popped = (Period*)popQueue(&periods1);
            if (popped->isPeriod)
            {
              if (popped->isEnd)
              {
                pwmStop(hetRAM1, M1_CW);
              } else
              {
                signal.period = popped->period;
                if (popped->isCW)
                {
                  pwmSetSignal(hetRAM1, M1_CW, signal);

                } else
                {
                  pwmStop(hetRAM1, M1_CW);
                  pwmEnableNotification(hetREG1, M1_CW, pwmEND_OF_PERIOD);
                  pwmSetSignal(hetRAM1, M1_CCW, signal);
                }
              }

            }
          }
          break;
        case M2_CCW:
          if (periods2.index < 0)
          {
            pwmStop(hetRAM1, M2_CCW);
            pwmDisableNotification(hetREG1,M2_CCW,pwmEND_OF_BOTH);
            rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK1);
            rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE1);
          } else
          {
            Period* popped = (Period*)popQueue(&periods2);
            if (popped->isPeriod)
            {
              if (popped->isEnd)
              {
                pwmStop(hetRAM1, M2_CCW);
                pwmDisableNotification(hetREG1,M2_CCW,pwmEND_OF_DUTY);
              } else
              {
                signal.period = popped->period;
                if (!popped->isCW)
                {
                  pwmSetSignal(hetRAM1, M2_CCW, signal);
                } else
                {
                  pwmStop(hetRAM1, M2_CCW);
                  pwmEnableNotification(hetREG1, M2_CCW, pwmEND_OF_PERIOD);
                  pwmSetSignal(hetRAM1, M2_CW, signal);
                }
              }

            }
          }
          break;
        case M2_CW:
          if (periods2.index < 0)
          {
            pwmStop(hetRAM1, M2_CW);
            pwmDisableNotification(hetREG1,M2_CW,pwmEND_OF_BOTH);
            rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK0);
            rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE1);
          } else
          {
            Period* popped = (Period*)popQueue(&periods2);
            if (popped->isPeriod)
            {
              if (popped->isEnd)
              {
                pwmStop(hetRAM1, M2_CW);
                pwmDisableNotification(hetREG1,M2_CW,pwmEND_OF_DUTY);
              } else
              {
                signal.period = popped->period;
                if (popped->isCW)
                {
                  pwmSetSignal(hetRAM1, M2_CW, signal);
                } else
                {
                  pwmStop(hetRAM1, M2_CW);
                  pwmEnableNotification(hetREG1, M2_CW, pwmEND_OF_PERIOD);
                  pwmSetSignal(hetRAM1, M2_CCW, signal);
                }
              }

            }
          }
          break;
        case pwm5:
            pwmStop(hetRAM1,pwm5);
            pwmDisableNotification(hetREG1,pwm5,pwmEND_OF_DUTY);
            break;
      }
      break;
    case pwmEND_OF_PERIOD:
      if(isMovment){
      switch (pwm)
      {
        case M1_CCW:
          pwmStart(hetRAM1, M1_CW);
          pwmEnableNotification(hetREG1, M1_CW, pwmEND_OF_DUTY);
          pwmDisableNotification(hetREG1, M1_CCW, pwmEND_OF_BOTH);
          break;
        case M1_CW:
          pwmStart(hetRAM1, M1_CCW);
          pwmEnableNotification(hetREG1, M1_CCW, pwmEND_OF_DUTY);
          pwmDisableNotification(hetREG1, M1_CW, pwmEND_OF_BOTH);
          break;
        case M2_CCW:
          pwmStart(hetRAM1, M2_CW);
          pwmEnableNotification(hetREG1, M2_CW, pwmEND_OF_DUTY);
          pwmDisableNotification(hetREG1, M2_CCW, pwmEND_OF_BOTH);
          break;
        case M2_CW:
          pwmStart(hetRAM1, M2_CCW);
          pwmEnableNotification(hetREG1, M2_CCW, pwmEND_OF_DUTY);
          pwmDisableNotification(hetREG1, M2_CW, pwmEND_OF_BOTH);
          break;
      }
      }else
      {
          iteration++;
      }
      break;
  }
}

void rtiNotification(rtiBASE_t *rtiREG, uint32 notification)
{
  /*  enter user code between the USER CODE BEGIN and USER CODE END. */
  /* USER CODE BEGIN (12) */
  switch (notification)
  {
    case rtiNOTIFICATION_COMPARE0:
      if (periods1.index >= 0)
      {
        Period* popped = (Period*)popQueue(&periods1);
        if (popped->isPeriod)
        {
          if (!popped->isEnd)
          {
            signal.period = popped->period;
            if (popped->isCW)
            {
              pwmSetSignal(hetRAM1, M1_CW, signal);
              pwmStart(hetRAM1, M1_CW);
              pwmEnableNotification(hetREG1, M1_CW, pwmEND_OF_DUTY);
            } else
            {
              pwmSetSignal(hetRAM1, M1_CCW, signal);
              pwmStart(hetRAM1, M1_CCW);
              pwmEnableNotification(hetREG1, M1_CCW, pwmEND_OF_DUTY);
            }
          }
        }
        rtiStopCounter(rtiREG1, rtiCOUNTER_BLOCK0);
        rtiDisableNotification(rtiREG1, rtiNOTIFICATION_COMPARE0);
      }
      break;
    case rtiNOTIFICATION_COMPARE1:
      if (periods2.index >= 0)
      {
        Period* popped = (Period*)popQueue(&periods2);
        if (popped->isPeriod)
        {
          if (!popped->isEnd)
          {
            signal.period = popped->period;
            if (popped->isCW)
            {
              pwmSetSignal(hetRAM1, M2_CW, signal);
              pwmStart(hetRAM1, M2_CW);
              pwmEnableNotification(hetREG1, M2_CW, pwmEND_OF_DUTY);
            } else
            {
              pwmSetSignal(hetRAM1, M2_CCW, signal);
              pwmStart(hetRAM1, M2_CCW);
              pwmEnableNotification(hetREG1, M2_CCW, pwmEND_OF_DUTY);
            }
          }
        }
        rtiStopCounter(rtiREG1, rtiCOUNTER_BLOCK1);
        rtiDisableNotification(rtiREG1, rtiNOTIFICATION_COMPARE1);
      }
      break;
  }
}
/* USER CODE END */
