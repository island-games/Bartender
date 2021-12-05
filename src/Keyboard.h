#ifndef Keyboard_h_
#define Keyboard_h_
#include "controllers.h"
#include <vita2d.h>
#include <string>

class Keyboard{
public:

std::string* fieldWritingTo;
std::string fieldName;
int cursorXPosition;
int cursorYPosition;
int cursorPosition;
bool isShiftActive;
bool isActive;
controllers* controller;
vita2d_texture* keyTexture;
vita2d_font* keyboardFont;

Keyboard(controllers* con, vita2d_font* tempFont);
void SetFieldToEdit(std::string* fieldWritingToTemp);
void KeyboardUpdate();
void KeyboardClose();
void KeyboardMove();
void ShiftCheck();
void SelectKey();
void AddChar(const char* charToAdd);
void AddToElement();
void DrawKeyboard();
void KeyboardBackspace();

};

#endif