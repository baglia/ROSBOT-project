#include "Scheduling.h"

bool interpreter(Queue* _cmds, Queue* _queue1, Queue* _queue2, Queue* _queue3)
{
    Command* _cmd;

	if(_cmds->index==-1)
		return false;
	_cmd=(Command*)popQueue(_cmds);
	Command cmd;
	switch((int)_cmd->type)
	{
		case LINEAR_MOV:
		cmd.type=_cmd->type-11;
		pointCopy(_cmd->point, &cmd.point);
		cmd.parameter=_cmd->parameter;
		startCalcLoop(cmd, _queue1, _queue2);
		break;
		case CIRC_OR_MOV:

		break;
		case CIRC_AOR_MOV:

		break;
		case UNRELATED:

		break;
		case JOINT:

		break;
		case PROG:

		break;
		case GRIPPER:

		break;
		case NUL:

		break;
		case PAUSE:

		break;
		case IF:

		break;
		case V_MIN:

		break;
		default:
		return false;
	}
	return true;
}

bool startCalcLoop(Command _cmd, Queue* _queue1, Queue* _queue2)
{
    Cicloydal low;
    ParameterLinearMovment settings;
    Point2D point;
	switch((int)_cmd.type)
	{
		case 0: //MOVIMENTO LINEARE
		    point.x=_cmd.point.x;
		    point.y=_cmd.point.y;
		setCicloydal(_cmd.parameter,pointDistance(actualPoint,point), &low);
		setLinMovment(actualPoint, point, low, &settings);
		calcLoopLinear(settings, _queue1, _queue2);
		break;
		case 1:

		break;
		case 3:

		break;
		case 4:

		break;
		default:
		return false;
	}
	return true;
}

bool calcLoopLinear(ParameterLinearMovment _movment, Queue* _queue1, Queue* _queue2)
{
	Period period1, period2;
	nextPeriodA(&period1, _movment, true);
	nextPeriodB(&period2, _movment, true);
	pushQueue(_queue1,(int*)&period1);
	pushQueue(_queue2,(int*)&period2);
	while(1)
	{
		if(_queue1->size-1>_queue1->index)
		{
			nextPeriodA(&period1, _movment, true);
			pushQueue(_queue1,(int*)&period1);
		}
		if(_queue2->size-1>_queue2->index)
		{
			nextPeriodB(&period2, _movment, true);
			pushQueue(_queue2,(int*)&period2);
		}
		if(period1.isEnd && period2.isEnd)
			break;
	}
	return true;
}
