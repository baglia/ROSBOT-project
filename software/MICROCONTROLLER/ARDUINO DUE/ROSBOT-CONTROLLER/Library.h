

#ifndef LIBRARY_H
#define LIBRARY_H
#include "math.h"
#include "stdlib.h"
#include "GlobalVar.h"
#include "Mathematic.h"

#define ERROR 0
#define CONTINUE 1
#define UNREACHABLE 2
#define LINEAR_MOV 11
#define CIRC_OR_MOV 12
#define CIRC_AOR_MOV 13
#define UNRELATED 14
#define JOINT 15
#define PROG 16
#define GRIPPER 17
#define HOMING 18
#define NUL 92
#define PAUSE 93
#define WAIT 94
#define SET 95
#define V_MIN 174
#define RESET 175
#define EMERGENCY 176
#define SET_ACC 177
#define CH_STOP '!'

#define ERROR_LENGTH 3
#define OK_LENGTH 3
#define UN_LENGTH 3

typedef struct
{
    char type;
    Point2D point;
    float parameter;
}Command;

Command* parser(char* _message);
bool reader(int* _command);
double stringToNumber(char* _string, unsigned int* _index);
bool readMovment(char* _string, Command* _command, unsigned int* _index);
bool readCondition(char* _string, Command* _command, unsigned int* _index);
bool readSetting(char* _string, Command* _command, unsigned int* _index);

#endif
