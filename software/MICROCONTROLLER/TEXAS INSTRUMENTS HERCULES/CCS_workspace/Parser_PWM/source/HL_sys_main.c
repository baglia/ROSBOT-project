/** @file HL_sys_main.c
    @brief Application main file
    @date 11-Dec-2018
    @version 04.07.01

    This file contains an empty main function,
    which can be used for the application.
*/

/*
  Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com


   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

     Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the
     distribution.

     Neither the name of Texas Instruments Incorporated nor the names of
     its contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"
#include "HL_sys_core.h"
#include "HL_esm.h"
#include "HL_epc.h"
#include "HL_het.h"
#include "HL_sci.h"
#include "Library.h"
#include "HL_sys_vim.h"
#include "math.h"
#include "Scheduling.h"
#include "GlobalVar.h"

/* USER CODE BEGIN (1) */
/* USER CODE END */

/** @fn void main(void)
    @brief Application main function
    @note This function is empty by default.

    This function is called after startup.
    The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
int length1 = 370, length2 = 260, tolerance = 1;
MotorAngle alpha, beta;
Point2D origin, actualPoint;
float maxSpeed = 100, maxAccel = 50, minSpeed = 1;
bool pwmM1=false, pwmM2=false, pwmM3=false;
/* USER CODE END */

int main(void)
{
  /* USER CODE BEGIN (3) */
  hetInit();
  _enable_IRQ_interrupt_();
  actualPoint.x = 260;
  actualPoint.y = 370;
  alpha.dAngle = 2 * PI / 50000;
  beta.dAngle = 2 * PI / 25000;
  sciInit();
  sciSendByte(sciREG1, 'c');
  Queue commands, periods1, periods2, periods3;
  queueInitializer(&commands, 10, sizeof(Command));
  queueInitializer(&periods1, 100, sizeof(unsigned int));
  queueInitializer(&periods2, 100, sizeof(unsigned int));
  queueInitializer(&periods1, 100, sizeof(unsigned int));
  hetSIGNAL_t signal;
  signal.duty=50;
  while (1)
  {
    reader(&commands);
    interpreter(&commands, &periods1, &periods2, &periods3);
    if(!pwmM1){
        if(periods1.index>-1)
        {
            signal.period=popQueue(periods1);
        }
    }
  }
  /* USER CODE END */


}


/* USER CODE BEGIN (4) */
void pwmNotification(hetBASE_t * hetREG, uint32 pwm, uint32 notification)
{
  /*  enter user code between the USER CODE BEGIN and USER CODE END. */
  /* USER CODE BEGIN (38) */
    //Settaggio periodo del pwm dei motori
    //In caso non ci siano periodi ma il moviento non è terminato stoppo pwm e avvio un timer per controllare periodicamente
    //se ci sono nuovi periodi
}
/* USER CODE END */
