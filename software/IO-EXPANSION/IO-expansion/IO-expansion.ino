//===== MESSAGGIO lettura input: sixxx!
//===== MESSAGGIO impostazione output: soxxx|vuv!
//===== MESSAGGIO impostazione output lite: sOxxx|v!
//===== MESSAGGIO di risposta: svuv!

#define CH_STOP '!'

int pin[50], numberPin;
bool value[50];
bool parser(char* _message);
double stringToNumber(char* _string, int* _index);
char message[32];
int inde, isRead;
void setup() {
  Serial.begin(115200);
  //=======SETTAGGIO PIN ARDUINO UNO===================
  for (int i = 2; i < 20; i++) {
    pinMode(i, OUTPUT);
  }
  digitalWrite(11,1);
  /*pinMode(52, OUTPUT);
  for (int i = 32; i < 51; i++) {
    i++;
    pinMode(i, INPUT);
  }*/
}

void loop() {


  if (Serial.available() && !isRead)
  {
    message[inde] = Serial.read();

    if (message[inde] == CH_STOP)
    {
      inde = -1;
      isRead = true;
    }
    else if (inde == 31)
    {
      inde = -1;
      isRead = true;
      message[0] = 0;
    }

    inde++;
  }
  else if (isRead)
  {
    if (!parser(message)) {
      Serial.print("sE!");
    }
    isRead = false;
  }
}

bool parser(char* _message)
{
  int step = 0, i = 0;
  bool isLast = false;
  while (1) {
    switch (step)
    {
      case 0:
        switch (*(_message+i))
        {
          case 's':
            step = 1;
            break;
          default:
            step = -1;
            break;
        }
        i++;
        break;
      case 1:
        switch (*(_message+i))
        {
          case 'o':
            i++;
            setOutput((_message+i));

            break;
          case 'O':
            i++;
            setOutputL((_message+i));
            break;
          case 'i':
            i++;
            readInput((_message+i));
            break;
          default:
            step = -1;
            break;
        }
        return true;
        break;
      default: //in caso di errore
        step = 0;
        i = 0;
        return false;
    }
  }
}

bool setOutput(char* _string)
{
  int c = -1;
  numberPin = 0;
  if (*_string != '|') {
    do {
      c++;
      pin[numberPin] = (int)stringToNumber(_string + c, &c);
      numberPin++;
    } while (*(_string + c) != '|' && *(_string + c) == ',');
  }
  if (*(_string + c) != '|')
    return false;
  int f = 0;
  if(*(_string + c) != CH_STOP){
    c++;
  do{
    if (*(_string + c) == 'v') {
      value[f] = true;
    } else {
      value[f] = false;
      
    }
    f++;
    c++;
  }while (*(_string + c) != CH_STOP);
    
  }
  if (numberPin != f)
    return false;
  for (int i = 0; i < numberPin; i++)
  {
    digitalWrite(pin[i], value[i]);
  }
  return true;
}

bool setOutputL(char* _string)
{
  int c = -1;
  numberPin = 0;
  if (*_string != '|') {
    do {
      c++;
      pin[numberPin] = (int)stringToNumber(_string + c, &c);
      
      numberPin++;
    } while (*(_string + c) != '|' && *(_string + c) == ',');
  }
  if (*(_string + c) != '|')
    return false;
  c++;
  switch (*(_string + c))
  {
    case 'v':
      for (int i = 0; i < numberPin; i++)
      {
        Serial.println(pin[i]);
        digitalWrite(pin[i], true);
      }
      break;
    case 'u':
      for (int i = 0; i < numberPin; i++)
      {
        digitalWrite(pin[i], false);
      }
      break;
    default:
      return false;
  }
  return true;
}

bool readInput(char* _string)
{
  int c = -1;
  numberPin = 0;
  if (*_string != CH_STOP) {
    do {
      c++;
      pin[numberPin] = (int)stringToNumber(_string + c, &c);
      numberPin++;
    } while (*(_string + c) != '|' && *(_string + c) == ',');
  }
  if (*(_string + c) != CH_STOP)
    return false;
  for (int i = 0; i < numberPin; i++)
  {
    value[i] = digitalRead(pin[i]);
  }
  Serial.print('s');
  bool primo=true;
  for (int i = 0; i < numberPin; i++)
  {
    if(!primo){
      Serial.print(',');
    }
    primo=false;
    Serial.print(value[i]);
  }
  Serial.print(CH_STOP);
  return true;
}

double stringToNumber(char* _string, int* _index)
{
  int i = 0;
  double number = 0;
  char* str = _string;
  while (*(str + i) > 47 && *(str + i) < 58)
  {
    i++;
  }
  int c;
  for (c = 1; c <= i; c++)
  {
    number += pow(10, i - c) * (int)(*(str + c - 1) - 48);
  }
  str += i;
  if (*str == '.')
  {
    str++;
    i++;
    int f = 0;
    while (*(str + f) > 47 && *(str + f) < 58)
    {
      i++;
      f++;
    }
    int c;
    for (c = 1; c <= f; c++)
    {
      number += pow(10, -c) * (int)(*(str + c - 1) - 48);
    }
  }
  *_index += i;
  return number;
}
