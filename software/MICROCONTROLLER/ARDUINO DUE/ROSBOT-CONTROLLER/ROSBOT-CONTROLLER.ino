#include "Library.h"
#include "math.h"
#include "Scheduling.h"
#include "GlobalVar.h"


int tolerance = 1, epromPosition[12];
MotorAngle alpha, beta, betaAbs;
Point2D origin, actualPoint;
Command* command;
Command empty;
float maxSpeed = 1000, maxAccel = 50, minSpeed = 5, acceleration=1000;
bool isMovment = false, isEmergenza= false, isRead=false,isSent=false, isFinished=true;
volatile bool isEmergency=false, isHomed=false, isCom=false;


void emergenza(void);


void setup() {
  /*if(eeprom.read(0)==0)
  {
    
    eeprom.write(0,1);
    //eeprom.write(
  }*/
  origin.x=0;
  origin.y=0;
  Serial1.begin(115200);
  Serial.begin(115200);
  alpha.dAngle = 2 * PI / 50000;
  beta.dAngle = 2 * PI / 25000;
  pinMode(M1_RUN,1);
  pinMode(M2_RUN,1);
  pinMode(MZ_EN,1);
  pinMode(M1_CW,1);
  pinMode(M1_CCW,1);
  pinMode(M2_CW,1);
  pinMode(M2_CCW,1);
  pinMode(MZ_PROG,1);
  pinMode(MZ_MOVING,0);
  pinMode(MZ_RST,1);
  pinMode(MZ_ALM,0);
  pinMode(MZ_TRG,1);
  pinMode(M1_ALM,0);
  pinMode(M2_ALM,0);
  pinMode(PROXY_M1,0);
  pinMode(PROXY_M2,0);
  pinMode(EMG_CHN,0);
  delay(2000);
  Serial1.print("sO2,3,4,5,6,7|v!");
  digitalWrite(M1_RUN,1);
  digitalWrite(M2_RUN,0);
  digitalWrite(MZ_EN,1);
  digitalWrite(MZ_RST,1);
  delay(1);
  digitalWrite(MZ_RST,0);
  digitalWrite(MZ_EN,0);
  attachInterrupt(digitalPinToInterrupt(EMG_CHN),emergenza,LOW);
  attachInterrupt(digitalPinToInterrupt(MZ_ALM),emergenza,RISING);
  //attachInterrupt(digitalPinToInterrupt(M1_ALM),emergenza,RISING);
  //attachInterrupt(digitalPinToInterrupt(M2_ALM),emergenza,RISING);
}
void loop() {
  reader((int*)&command);
  interpreter(command);
  command=&empty;
  if(isEmergency&&!isCom)
  {
    emergencyMsg();
    isCom=true;
  }
}

void emergenza(void)
{
  digitalWrite(M1_RUN,0);
  digitalWrite(M2_RUN,1);
  digitalWrite(MZ_EN,1);
  isEmergency=true;
  isHomed=false;
  isCom=false;
}
