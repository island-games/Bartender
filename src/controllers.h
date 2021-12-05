
#ifndef controllers_h_
#define controllers_h_
#include <psp2/ctrl.h>

class controllers {

public:
	bool isLeftPressed = false;
	bool isRightPressed = false;
	bool isUpPressed = false;
	bool isDownPressed = false;
	bool isLeftTriggerPressed = false;
	bool isRightTriggerPressed = false;
	bool isTrianglePressed = false;
	bool isCirclePressed = false;
	bool isCrossPressed = false;
	bool isSquarePressed = false;
	bool isStartPressed = false;
	bool isSelectPressed = false;
	
	bool canLeftBePressed = false;
	bool canRightBePressed = false;
	bool canUpBePressed = false;
	bool canDownBePressed = false;
	bool canLeftTriggerBePressed = false;
	bool canRightTriggerBePressed = false;
	bool canTriangleBePressed = false;
	bool canCircleBePressed = false;
	bool canSquareBePressed = false;
	bool canCrossBePressed = false;
	bool canStartBePressed = false;
	bool canSelectBePressed = false;
	int leftStickX;
	int leftStickY;
	int rightStickX;
	int rightStickY;
	int controllerNumber;
	SceCtrlData pad;
	SceCtrlPortInfo portInfo;
	uint8_t portInfoNumber;
	
	
controllers(int num);
void CheckInputs();
void GetCanPresses();
};
#endif