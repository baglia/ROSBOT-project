#include "Library.h"

Command* parser(char* _message)
{
  static int step = 0;
  static unsigned int i = 0;
  Command* newe;
  char* holdInput = _message + i;
  bool isLast=false;
  switch (step)
  {
    case 0:
    switch (*holdInput)
    {
      case 'M':
      step = 1;
      break;
      case 'C':
      step = 2;
      break;
      case 'S':
      step = 3;
      break;
      default:
      step = -1;
      break;
    }
    i++;
    break;
    case 1:
    switch ((unsigned int)stringToNumber(holdInput,&i))
    {
      case 4:
      step = 7;
      break;
      case 5:
      step = 8;
      break;
      case 6:
      step = 9;
      break;
      case 7:
      step = 10;
      break;
      case 8:
      step = 15;
      break;
      default:
      step = -1;
      break;
    }
    break;
    case 2:
    switch ((unsigned int)stringToNumber(holdInput,&i))
    {
      case 0:
      step = 11;
      break;
      case 1:
      step = 12;
      break;
      case 2:
      step = 13;
      break;
      case 3:
      step = 19;
      break;
      default:
      step = -1;
      break;
    }
    break;
    case 3:
    switch ((unsigned int)stringToNumber(holdInput,&i))
    {
      case 1:
      step = 14;
      break;
      case 2:
      step = 16;
      break;
      case 3:
      step=17;
      break;
      case 4:
      step=18;
      break;
    }
    break;
    case 7:
    newe = (Command*)malloc(sizeof(Command));
    newe->type=UNRELATED;
    if(!readMovment(holdInput,newe,&i))
    {
      step = -1;
      break;
    }
    if(!isPointReachable(newe->point))
        {
            step = -2;
            break;
        }
    isLast=true;
    break;
    case 8:
    newe = (Command*)malloc(sizeof(Command));
    newe->type=JOINT;
    if(!readMovment(holdInput,newe,&i))
    {
      step = -1;
      break;
    }
    isLast=true;
    break;
    case 9:
    newe = (Command*)malloc(sizeof(Command));
    newe->type=PROG;
    if(!readMovment(holdInput,newe,&i))
    {
      step = -1;
      break;
    }
    isLast=true;
    break;
    case 10:
    newe = (Command*)malloc(sizeof(Command));
    newe->type=GRIPPER;
    if(!readMovment(holdInput,newe,&i))
    {
      step = -1;
      break;
    }
    isLast=true;
    break;
    case 11:
    newe = (Command*)malloc(sizeof(Command));
    newe->type=NUL;
    break;
    case 12:
    newe = (Command*)malloc(sizeof(Command));
    newe->type=PAUSE;
    if(!readCondition(holdInput,newe,&i))
    {
      step = -1;
      break;
    }
    isLast=true;
    break;
    case 13:
    newe = (Command*)malloc(sizeof(Command));
    newe->type=WAIT;
    if(!readCondition(holdInput,newe,&i))
    {
      step = -1;
      break;
    }
    isLast=true;
    break;
    case 14:
    newe = (Command*)malloc(sizeof(Command));
    newe->type=V_MIN;
    if(!readSetting(holdInput,newe,&i))
    {
      step = -1;
      break;
    }
    isLast=true;
    break;
    case 15:
    newe = (Command*)malloc(sizeof(Command));
    newe->type=HOMING;
    isLast=true;
    break;
    case 16:
    newe = (Command*)malloc(sizeof(Command));
    newe->type=RESET;
    isLast=true;
    break;
    case 17:
    newe = (Command*)malloc(sizeof(Command));
    newe->type=EMERGENCY;
    isLast=true;
    break;
    case 18:
    newe = (Command*)malloc(sizeof(Command));
    newe->type=SET_ACC;
    if(!readSetting(holdInput,newe,&i))
    {
      step = -1;
      break;
    }
    isLast=true;
    break;
    case 19:
    newe = (Command*)malloc(sizeof(Command));
    newe->type=SET;
    if(!readCondition(holdInput,newe,&i))
    {
      step = -1;
      break;
    }
    isLast=true;
    break;
    case -1: //in caso di errore
    newe = (Command*)malloc(sizeof(Command));
    newe->type=ERROR;
    step =0;
    i=0;
    return newe;
    case -2:    //in caso il punto non sia raggiungibile
        newe = (Command*)malloc(sizeof(Command));
            newe->type=UNREACHABLE;
            step =0;
            i=0;
            return newe;
  }
  if(isLast)
    {
  if(*(_message+i)!=CH_STOP)
    {
      step = -1;
      newe = (Command*)malloc(sizeof(Command));
      newe->type=CONTINUE;
    }
    else
    {
      step = 0;
      i=0;
    }
    }else{
        newe = (Command*)malloc(sizeof(Command));
        newe->type=CONTINUE;
    }
    return newe;
}

bool readMovment(char* _string, Command* _command, unsigned int* _index)
{
  unsigned int c=0;
  switch(*_string)
  {
    case 'X':
    c++;
    _command->point.x=stringToNumber(_string+c,&c);
    if(*(_string+c)!='Y')
      return false;
    c++;
    _command->point.y=stringToNumber(_string+c,&c);
    if(*(_string+c)!='V')
      return false;
    c++;
    _command->parameter=stringToNumber(_string+c,&c);
    break;
    case 'P':
    c++;
    _command->point.x=stringToNumber(_string+c,&c);
    if(*(_string+c)!='S')
      return false;
    c++;
    _command->point.y=stringToNumber(_string+c,&c);
    if(*(_string+c)!='V')
      return false;
    c++;
    _command->parameter=stringToNumber(_string+c,&c);
    break;
    case 'S':
    c++;
    _command->parameter=stringToNumber(_string+c,&c);
    break;
    case 'Z':
    c++;
    _command->parameter=stringToNumber(_string+c,&c);
    break;
    default:
    return false;
  }
  *_index+=c;
  return true;
}

bool readCondition(char* _string, Command* _command, unsigned int* _index)
{
  unsigned int c=0;
  switch(*_string)
  {
    case 'T':
    c++;
    _command->parameter=stringToNumber(_string+c,&c);
    break;
    case 'P':
    c++;
    _command->point.x=stringToNumber(_string+c,&c);
    if(*(_string+c)!='S')
      return false;
    c++;
    _command->parameter=stringToNumber(_string+c,&c);
    break;
    default:
    return false;
  }
  *_index+=c;
  return true;
}

bool readSetting(char* _string, Command* _command, unsigned int* _index)
{
  unsigned int c=0;
  switch(*_string)
  {
    case 'V':
    c++;
    _command->parameter=stringToNumber(_string+c,&c);
    break;
    case 'A':
    c++;
    _command->parameter=stringToNumber(_string+c,&c);
    break;
    default:
    return false;
  }
  *_index+=c;
  return true;
}

double stringToNumber(char* _string, unsigned int* _index)
{
  int i = 0, sign=1;
  double number = 0;
  char* str=_string;
  int c=1;
  if(*str=='-')
  {
      sign=-1;
      i++;
      c++;
  }
  while(*(str+i)>47&&*(_string+i)<58)
  {
    i++;
  }

  for(c; c<=i; c++)
  {
    number+=pow(10,i-c)*(int)(*(str+c-1)-48);
  }
  str+=i;
  if(*str=='.')
  {
    str++;
    i++;
    int f =0;
    while(*(str+f)>47&&*(str+f)<58)
    {
      i++;
      f++;
    }
    int c;
    for(c=1; c<=f;c++)
    {
      number+=pow(10,-c)*(int)(*(str+c-1)-48);
    }
  }
  *_index+=i;
  return number*sign;
}


bool reader(int* _command)
{
  static char message[32];
  static Command* command;
  static int index, isRead, isParsed;
  if(Serial.available()&&!isRead&&!isParsed)
  {
    message[index]=Serial.read();
    if(message[index]=='!')
    {
      index=-1;
      isRead=true;
    }
    else if(index==31)
    {
      index=-1;
      isRead=true;
      message[0]=0;
    }

    index++;
  }
  else if(isRead&&!isParsed)
  {
    command=parser(message);
    switch((int)command->type)
    {
      case ERROR:
      free(command);
      isRead=false;
      errorMsg();
      break;
      case CONTINUE:
      free(command);
      break;
      default:
      okMsg();
      isParsed=true;
      isRead=false;
      break;
      case UNREACHABLE:
      free(command);
      isRead=false;
      unreachbleMsg();
      break;
    }
  }else if(isParsed)
  {      
      *_command=(int)command;
      isParsed=false;
      return true;
  }
  return false;
}
