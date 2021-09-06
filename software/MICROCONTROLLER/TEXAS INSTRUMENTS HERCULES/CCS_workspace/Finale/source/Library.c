#include "Library.h"

Command* parser(char* _message)
{
  static int step = 0, i = 0;
  Command* new;
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
      case 1:
      step = 4;
      break;
      case 2:
      step = 5;
      break;
      case 3:
      step = 6;
      break;
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
      case 5:
      step=19;
      break;
    }
    break;
    case 4:
    new = (Command*)malloc(sizeof(Command));
    new->type=LINEAR_MOV;
    if(!readMovment(holdInput,new,&i))
    {
      step = -1;
      break;
    }
    if(!isPointReachable(new->point))
    {
        step = -2;
        break;
    }
    isLast=true;
    break;
    case 5:
    new = (Command*)malloc(sizeof(Command));
    new->type=CIRC_OR_MOV;
    if(!readMovment(holdInput,new,&i))
    {
      step = -1;
      break;
    }
    if(!isPointReachable(new->point))
        {
            step = -2;
            break;
        }
    isLast=true;
    break;
    case 6:
    new = (Command*)malloc(sizeof(Command));
    new->type=CIRC_AOR_MOV;
    if(!readMovment(holdInput,new,&i))
    {
      step = -1;
      break;
    }
    if(!isPointReachable(new->point))
        {
            step = -2;
            break;
        }
    isLast=true;
    break;
    case 7:
    new = (Command*)malloc(sizeof(Command));
    new->type=UNRELATED;
    if(!readMovment(holdInput,new,&i))
    {
      step = -1;
      break;
    }
    if(!isPointReachable(new->point))
        {
            step = -2;
            break;
        }
    isLast=true;
    break;
    case 8:
    new = (Command*)malloc(sizeof(Command));
    new->type=JOINT;
    if(!readMovment(holdInput,new,&i))
    {
      step = -1;
      break;
    }
    isLast=true;
    break;
    case 9:
    new = (Command*)malloc(sizeof(Command));
    new->type=PROG;
    isLast=true;
    break;
    case 10:
    new = (Command*)malloc(sizeof(Command));
    new->type=GRIPPER;
    if(!readMovment(holdInput,new,&i))
    {
      step = -1;
      break;
    }
    isLast=true;
    break;
    case 11:
    new = (Command*)malloc(sizeof(Command));
    new->type=NUL;
    break;
    case 12:
    new = (Command*)malloc(sizeof(Command));
    new->type=PAUSE;
    if(!readCondition(holdInput,new,&i))
    {
      step = -1;
      break;
    }
    isLast=true;
    break;
    case 13:
    new = (Command*)malloc(sizeof(Command));
    new->type=IF;
    if(!readCondition(holdInput,new,&i))
    {
      step = -1;
      break;
    }
    isLast=true;
    break;
    case 14:
    new = (Command*)malloc(sizeof(Command));
    new->type=V_MIN;
    if(!readSetting(holdInput,new,&i))
    {
      step = -1;
      break;
    }
    isLast=true;
    break;
    case 15:
    new = (Command*)malloc(sizeof(Command));
    new->type=HOMING;
    isLast=true;
    break;
    case 16:
    new = (Command*)malloc(sizeof(Command));
    new->type=RESET;
    isLast=true;
    break;
    case 17:
    new = (Command*)malloc(sizeof(Command));
    new->type=EMERGENCY;
    isLast=true;
    break;
    case 18:
    new = (Command*)malloc(sizeof(Command));
    new->type=S_TEST;
    isLast=true;
    break;
    case 19:
    new = (Command*)malloc(sizeof(Command));
    new->type=SET_ACC;
            isLast=true;
    break;
    case -1: //in caso di errore
    new = (Command*)malloc(sizeof(Command));
    new->type=ERROR;
    step =0;
    i=0;
    return new;
    case -2:    //in caso il punto non sia raggiungibile
        new = (Command*)malloc(sizeof(Command));
            new->type=UNREACHABLE;
            step =0;
            i=0;
            return new;
  }
  if(isLast)
    {
  if(*(_message+i)!=CH_STOP)
    {
      step = -1;
      new = (Command*)malloc(sizeof(Command));
            new->type=CONTINUE;
    }
    else
    {
      step = 0;
      i=0;
    }
    }else{
        new = (Command*)malloc(sizeof(Command));
        new->type=CONTINUE;
    }
    return new;
}

bool readMovment(char* _string, Command* _command, int* _index)
{
  int c=0;
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
    default:
    return false;
  }
  *_index+=c;
  return true;
}

bool readCondition(char* _string, Command* _command, int* _index)
{
  int c=0;
  switch(*_string)
  {
    case 'T':
    c++;
    _command->parameter=stringToNumber(_string+c,&c);
    break;
    case 'I':
    c++;
    _command->parameter=stringToNumber(_string+c,&c);
    break;
    default:
    return false;
  }
  *_index+=c;
  return true;
}

bool readSetting(char* _string, Command* _command, int* _index)
{
  int c=0;
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

double stringToNumber(char* _string, int* _index)
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

  while(c<=i)
  {
    number+=pow(10,i-c)*(int)(*(str+c-1)-48);
    c++;
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


bool reader(Queue* _commands)
{
    char ErrorMessage[]={"Er!"};
    char okMessage[]={"Ok!"};
    char UnreachableMessage[]={"Un!"};
  static char message[32];
  static Command* command;
  static int index, isRead, isParsed;
  if(sciIsRxReady(sciREG1)&&!isRead&&!isParsed)
  {
    message[index]=sciReceiveByte(sciREG1);
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
      sciSend(sciREG1, ERROR_LENGTH, (uint8*)ErrorMessage);
      break;
      case CONTINUE:
      free(command);
      break;
      default:
      sciSend(sciREG1, OK_LENGTH, (uint8*)okMessage);
      isParsed=true;
      isRead=false;
      break;
      case UNREACHABLE:
      free(command);
      isRead=false;
      sciSend(sciREG1, UN_LENGTH, (uint8*)UnreachableMessage);
      break;
    }
  }else if(isParsed)
  {
    if(pushQueue(_commands,(int*)command))
    {
      isParsed=false;
      return true;
    }else{
        char fullMsg[]={"Fll!"};
        sciSend(sciREG1,4,(uint8*)fullMsg);
    }
  }
  return false;
}

bool pushQueue(Queue* _queue, int* _ptr)
{
  if(_queue->index==_queue->size-1)
    return false;
  _queue->index++;
  _queue->elements[_queue->index]=(int)_ptr;
  return true;
}

int* popQueue(Queue* _queue)
{
  if(_queue->index==-1)
    return 0;
  int* first;
  first=(int*)_queue->elements[0];
  _queue->index--;
  int i;
  for(i = 0; i<_queue->index; i++)
  {
    _queue->elements[i]=_queue->elements[i+1];
  }
  return first;
}

inline bool queueInitializer(Queue* _queue, unsigned int _size, unsigned int _sizeOf)
{
  _queue->size=_size;
  _queue->index=-1;
  _queue->elements= (int*)malloc(_size*_sizeOf);
  return true;
}
