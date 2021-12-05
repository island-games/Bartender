using namespace std;
#include "controllers.h"
//#include <psp2/ctrl.h>
controllers::controllers(int num)
{
	isLeftPressed = false;
	isRightPressed = false;
	isUpPressed = false;
	isDownPressed = false;
	isLeftTriggerPressed = false;
	isRightTriggerPressed = false;
	isTrianglePressed = false;
	isCirclePressed = false;
	isCrossPressed = false;
	isSquarePressed = false;
	isStartPressed = false;
	isSelectPressed = false;
	
	canLeftBePressed = false;
	canRightBePressed = false;
	canUpBePressed = false;
	canDownBePressed = false;
	canLeftTriggerBePressed = false;
	canRightTriggerBePressed = false;
	canTriangleBePressed = false;
	canCircleBePressed = false;
	canSquareBePressed = false;
	canCrossBePressed = false;
	canStartBePressed = false;
	canSelectBePressed = false;	
	controllerNumber = num;
	leftStickX = 0;
	leftStickY = 0;
	rightStickX = 0;
	rightStickY = 0;
	
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	portInfo;
	
	if(portInfo.port[1] == SCE_CTRL_TYPE_UNPAIRED && controllerNumber == 1)
	{
		controllerNumber = 0;
	}
	
	pad;
	
}

void controllers::CheckInputs()
{
	sceCtrlPeekBufferPositive(controllerNumber, &pad, 1);
	if(pad.buttons & SCE_CTRL_LEFT)
	{
		isLeftPressed = true;
	}
	else
	{
		isLeftPressed = false;
	}
	
	if(pad.buttons & SCE_CTRL_RIGHT)
	{
		isRightPressed = true;
	}
	else
	{
		isRightPressed = false;
	}
	
	if(pad.buttons & SCE_CTRL_UP)
	{
		isUpPressed = true;
	}
	else
	{
		isUpPressed = false;
	}
	
	if(pad.buttons & SCE_CTRL_DOWN)
	{
		isDownPressed = true;
	}
	else
	{
		isDownPressed = false;
	}
	
	if(pad.buttons &SCE_CTRL_LTRIGGER )
	{
		isLeftTriggerPressed = true;
	}
	else
	{
		isLeftTriggerPressed = false;
	}
	
	if(pad.buttons &SCE_CTRL_RTRIGGER )
	{
		isRightTriggerPressed = true;
	}
	else
	{
		isRightTriggerPressed = false;
	}
	
	if(pad.buttons & SCE_CTRL_TRIANGLE)
	{
		isTrianglePressed = true;
	}
	else
	{
		isTrianglePressed = false;
	}
	
	if(pad.buttons & SCE_CTRL_SQUARE)
	{
		isSquarePressed = true;
	}
	else
	{
		isSquarePressed = false;
	}
	
	if(pad.buttons & SCE_CTRL_CROSS)
	{
		isCrossPressed = true;
	}
	else
	{
		isCrossPressed = false;
	}
	
	if(pad.buttons & SCE_CTRL_CIRCLE)
	{
		isCirclePressed = true;
	}
	else
	{
		isCirclePressed = false;
	}
	
	if(pad.buttons & SCE_CTRL_START)
	{
		isStartPressed = true;
	}
	else
	{
		isStartPressed = false;
	}
	
	if(pad.buttons & SCE_CTRL_SELECT)
	{
		isSelectPressed = true;
	}
	else
	{
		isSelectPressed = false;
	}
	
	leftStickX = (int)pad.lx;
	leftStickY = (int)pad.ly;
	rightStickX = (int)pad.rx;
	rightStickY = (int)pad.ry;
}

void controllers::GetCanPresses()
{
	if(isLeftPressed == true)
	{
		canLeftBePressed = false;
	}
	else
	{
		canLeftBePressed = true;
	}
	
	if(isRightPressed == true)
	{
		canRightBePressed = false;
	}
	else
	{
		canRightBePressed = true;
	}
	
	if(isUpPressed == true)
	{
		canUpBePressed = false;
	}
	else
	{
		canUpBePressed = true;
	}
	
	if(isDownPressed == true)
	{
		canDownBePressed = false;
	}
	else
	{
		canDownBePressed = true;
	}
	
	if(isCrossPressed == true)
	{
		canCrossBePressed = false;
	}
	else
	{
		canCrossBePressed = true;
	}
	
	if(isCirclePressed == true)
	{
		canCircleBePressed = false;
	}
	else
	{
		canCircleBePressed = true;
	}
	
	if(isTrianglePressed == true)
	{
		canTriangleBePressed = false;
	}
	else
	{
		canTriangleBePressed = true;
	}
	
	if(isSquarePressed == true)
	{
		canSquareBePressed = false;
	}
	else
	{
		canSquareBePressed = true;
	}
	
	if(isLeftTriggerPressed == true)
	{
		canLeftTriggerBePressed = false;
	}
	else
	{
		canLeftTriggerBePressed = true;
	}
	
	if(isRightTriggerPressed == true)
	{
		canRightTriggerBePressed = false;
	}
	else
	{
		canRightTriggerBePressed = true;
	}
	
	if(isStartPressed == true)
	{
		canStartBePressed = false;
	}
	else
	{
		canStartBePressed = true;
	}
	
	if(isSelectPressed == true)
	{
		canSelectBePressed = false;
	}
	
	else
	{
		canSelectBePressed = true;
	}
}