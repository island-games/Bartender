#include "Keyboard.h"


Keyboard::Keyboard(controllers* con, vita2d_font* tempFont)
{
cursorXPosition = 0;
cursorYPosition = 0;
cursorPosition = 0;
isShiftActive = false;
isActive = false;
controller = con;
const char * tempString = "app0:img/key.png";
keyTexture = vita2d_load_PNG_file(tempString);
keyboardFont = tempFont;
fieldWritingTo = NULL;
}


void Keyboard::SelectKey()
{
	if(controller->isCrossPressed == true && controller->canCrossBePressed == true)
	{
		switch(cursorYPosition)
		{
			case 0:
			
			switch(cursorXPosition)
			{
				case 0 :
				if(isShiftActive == true)
				{
					AddChar("!");
				}
				else
				{
					AddChar("1");
				}
				
				
				break;
				
				case 1 :
				if(isShiftActive == true)
				{
					AddChar("@");
				}
				else
				{
					AddChar("2");
				}
				
				break;
				
				case 2 :
				if(isShiftActive == true)
				{
					AddChar("#");
				}
				else
				{
					AddChar("3");
				}
				
				break;
				
				case 3 :
				if(isShiftActive == true)
				{
					AddChar("$");
				}
				else
				{
					AddChar("4");
				}
				
				break;
				
				case 4 :
				if(isShiftActive == true)
				{
					AddChar("%");
				}
				else
				{
					AddChar("5");
				}
				break;
				
				case 5 :
				if(isShiftActive == true)
				{
					AddChar("^");
				}
				else
				{
					AddChar("6");
				}
				break;
				
				case 6 :
				if(isShiftActive == true)
				{
					AddChar("&");
				}
				else
				{
					AddChar("7");
				}
				break;
				
				case 7 :
				if(isShiftActive == true)
				{
					AddChar("*");
				}
				else
				{
					AddChar("8");
				}
				break;
				
				case 8 :
				if(isShiftActive == true)
				{
					AddChar("(");
				}
				else
				{
					AddChar("9");
				}
				break;
				
				case 9 :
				if(isShiftActive == true)
				{
					AddChar(")");
				}
				else
				{
					AddChar("0");
				}
				break;
				
				case 10 :
				if(isShiftActive == true)
				{
					AddChar("_");
				}
				else
				{
					AddChar("-");
				}
				break;
				
			}
			
			break;
			
			case 1:
			switch(cursorXPosition)
			{
				case 0 :
				if(isShiftActive == true)
				{
					AddChar("Q");
				}
				else
				{
					AddChar("q");
				}
				
				
				break;
				
				case 1 :
				if(isShiftActive == true)
				{
					AddChar("W");
				}
				else
				{
					AddChar("w");
				}
				
				break;
				
				case 2 :
				if(isShiftActive == true)
				{
					AddChar("E");
				}
				else
				{
					AddChar("e");
				}
				
				break;
				
				case 3 :
				if(isShiftActive == true)
				{
					AddChar("R");
				}
				else
				{
					AddChar("r");
				}
				
				break;
				
				case 4 :
				if(isShiftActive == true)
				{
					AddChar("T");
				}
				else
				{
					AddChar("t");
				}
				break;
				
				case 5 :
				if(isShiftActive == true)
				{
					AddChar("Y");
				}
				else
				{
					AddChar("y");
				}
				break;
				
				case 6 :
				if(isShiftActive == true)
				{
					AddChar("U");
				}
				else
				{
					AddChar("u");
				}
				break;
				
				case 7 :
				if(isShiftActive == true)
				{
					AddChar("I");
				}
				else
				{
					AddChar("i");
				}
				break;
				
				case 8 :
				if(isShiftActive == true)
				{
					AddChar("O");
				}
				else
				{
					AddChar("o");
				}
				break;
				
				case 9 :
				if(isShiftActive == true)
				{
					AddChar("P");
				}
				else
				{
					AddChar("p");
				}
				break;
				
				case 10 :
				if(isShiftActive == true)
				{
					AddChar("{");
				}
				else
				{
					AddChar("[");
				}
				break;
				
				
			}
			
			break;
			
			case 2:
			switch(cursorXPosition)
			{
				case 0 :
				if(isShiftActive == true)
				{
					AddChar("A");
				}
				else
				{
					AddChar("a");
				}
				
				
				break;
				
				case 1 :
				if(isShiftActive == true)
				{
					AddChar("S");
				}
				else
				{
					AddChar("s");
				}
				
				break;
				
				case 2 :
				if(isShiftActive == true)
				{
					AddChar("D");
				}
				else
				{
					AddChar("d");
				}
				
				break;
				
				case 3 :
				if(isShiftActive == true)
				{
					AddChar("F");
				}
				else
				{
					AddChar("f");
				}
				
				break;
				
				case 4 :
				if(isShiftActive == true)
				{
					AddChar("G");
				}
				else
				{
					AddChar("g");
				}
				break;
				
				case 5 :
				if(isShiftActive == true)
				{
					AddChar("H");
				}
				else
				{
					AddChar("h");
				}
				break;
				
				case 6 :
				if(isShiftActive == true)
				{
					AddChar("J");
				}
				else
				{
					AddChar("j");
				}
				break;
				
				case 7 :
				if(isShiftActive == true)
				{
					AddChar("K");
				}
				else
				{
					AddChar("k");
				}
				break;
				
				case 8 :
				if(isShiftActive == true)
				{
					AddChar("L");
				}
				else
				{
					AddChar("l");
				}
				break;
				
				case 9 :
				if(isShiftActive == true)
				{
					AddChar(":");
				}
				else
				{
					AddChar(";");
				}
				break;
				
				case 10 :
				if(isShiftActive == true)
				{
					AddChar("\"");
				}
				else
				{
					AddChar("'");
				}
				break;
				
				

			}
			
			break;
			
			case 3:
			switch(cursorXPosition)
			{
				case 0 :
				if(isShiftActive == true)
				{
					AddChar("Z");
				}
				else
				{
					AddChar("z");
				}
				
				
				break;
				
				case 1 :
				if(isShiftActive == true)
				{
					AddChar("X");
				}
				else
				{
					AddChar("x");
				}
				
				break;
				
				case 2 :
				if(isShiftActive == true)
				{
					AddChar("C");
				}
				else
				{
					AddChar("c");
				}
				
				break;
				
				case 3 :
				if(isShiftActive == true)
				{
					AddChar("V");
				}
				else
				{
					AddChar("v");
				}
				
				break;
				
				case 4 :
				if(isShiftActive == true)
				{
					AddChar("B");
				}
				else
				{
					AddChar("b");
				}
				break;
				
				case 5 :
				if(isShiftActive == true)
				{
					AddChar("N");
				}
				else
				{
					AddChar("n");
				}
				break;
				
				case 6 :
				if(isShiftActive == true)
				{
					AddChar("M");
				}
				else
				{
					AddChar("m");
				}
				break;
				
				case 7 :
				if(isShiftActive == true)
				{
					AddChar("<");
				}
				else
				{
					AddChar(",");
				}
				break;
				
				case 8 :
				if(isShiftActive == true)
				{
					AddChar(">");
				}
				else
				{
					AddChar(".");
				}
				break;
				
				case 9 :
				if(isShiftActive == true)
				{
					AddChar("?");
				}
				else
				{
					AddChar("/");
				}
				break;
				
				case 10 :
				if(isShiftActive == true)
				{
					AddChar("+");
				}
				else
				{
					AddChar("=");
				}
				break;
				
				
			}
			
			break;
		}
	}
}

void Keyboard::KeyboardMove()
{
	if(controller->isLeftPressed == true && controller->canLeftBePressed == true)
	{
		if(cursorXPosition > 0)
		{
			cursorXPosition--;
		}
	}
	
	if(controller->isRightPressed == true && controller->canRightBePressed == true)
	{
		if(cursorXPosition < 10)
		{
			cursorXPosition++;
		}
	}
	
	if(controller->isUpPressed == true && controller->canUpBePressed == true)
	{
		if(cursorYPosition > 0)
		{
			cursorYPosition--;
		}
	}
	
	if(controller->isDownPressed == true && controller->canDownBePressed == true)
	{
		if(cursorYPosition < 3)
		{
			cursorYPosition++;
		}
	}
}

void Keyboard::KeyboardClose()
{
	if(controller->isStartPressed == true && controller->canStartBePressed == true)
	{
		isActive = false;
	}
}

void Keyboard::KeyboardBackspace()
{
	if(controller->isCirclePressed == true && controller->canCircleBePressed == true)
	{
		*fieldWritingTo = fieldWritingTo->substr(0,fieldWritingTo->size()-1);
	}
}

void Keyboard::SetFieldToEdit(std::string* fieldWritingToTemp)
{
	fieldWritingTo = fieldWritingToTemp;
	
}

void Keyboard::AddToElement()
{
	
}

void Keyboard::DrawKeyboard()
{
	if(isActive == true)
	{
	
			vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(150,150,150,255));
			
			
			
			vita2d_draw_rectangle(126, 86, 708, 64, RGBA8(0,0,0,255));
			vita2d_draw_rectangle(130, 90, 700, 56, RGBA8(255,255,255,255));
	
			vita2d_font_draw_text(keyboardFont, 126, 80, RGBA8(0,0,0,255), 32, fieldName.c_str());
			
			vita2d_font_draw_text(keyboardFont, 45, 520, RGBA8(255,255,255,255), 24,(char*)"Cross: Select           Circle: Backspace          R1: Shift(Toggle)          Start: Confirm");
			
			std::string fieldDisplaying = "";
			if(fieldWritingTo->size() >33)
			{
				fieldDisplaying = fieldWritingTo->substr(fieldWritingTo->size() - 33);
			}
			else
			{
				fieldDisplaying = *fieldWritingTo;
			}
			vita2d_font_draw_text(keyboardFont, 134, 130, RGBA8(0,0,0,255), 32, fieldDisplaying.c_str());
			
			for(int y2 = 0; y2 < 4; y2++)
			{
				
				int yPosition = 172 +(y2*80);
				for(int x2 = 0; x2 < 11; x2++)
				{
					int xPosition = 40 + (80 * x2);
					std::string stringToDisplay = "";
					if(cursorXPosition == x2 && cursorYPosition == y2)
					{
						if(keyTexture != NULL)
						{
							vita2d_draw_texture_tint(keyTexture, xPosition, yPosition, RGBA8(100,125,130,255));
						}
					}
					else
					{
						if(keyTexture != NULL)
						{
							vita2d_draw_texture_tint(keyTexture, xPosition, yPosition, RGBA8(255,255,255,255));
						}
					}
					
					
					switch(y2)
					{
				case 0:
				
				switch(x2)
				{
					case 0 :
					if(isShiftActive == true)
					{
						stringToDisplay = "!";
					}
					else
					{
						stringToDisplay = "1";
					}
					
					
					break;
					
					case 1 :
					if(isShiftActive == true)
					{
						stringToDisplay = "@";
					}
					else
					{
						stringToDisplay = "2";
					}
					
					break;
					
					case 2 :
					if(isShiftActive == true)
					{
						stringToDisplay="#";
					}
					else
					{
						stringToDisplay="3";
					}
					
					break;
					
					case 3 :
					if(isShiftActive == true)
					{
						stringToDisplay="$";
					}
					else
					{
						stringToDisplay="4";
					}
					
					break;
					
					case 4 :
					if(isShiftActive == true)
					{
						stringToDisplay="%";
					}
					else
					{
						stringToDisplay="5";
					}
					break;
					
					case 5 :
					if(isShiftActive == true)
					{
						stringToDisplay="^";
					}
					else
					{
						stringToDisplay="6";
					}
					break;
					
					case 6 :
					if(isShiftActive == true)
					{
						stringToDisplay="&";
					}
					else
					{
						stringToDisplay="7";
					}
					break;
					
					case 7 :
					if(isShiftActive == true)
					{
						stringToDisplay="*";
					}
					else
					{
						stringToDisplay="8";
					}
					break;
					
					case 8 :
					if(isShiftActive == true)
					{
						stringToDisplay="(";
					}
					else
					{
						stringToDisplay="9";
					}
					break;
					
					case 9 :
					if(isShiftActive == true)
					{
						stringToDisplay=")";
					}
					else
					{
						stringToDisplay="0";
					}
					break;
					
					case 10 :
					if(isShiftActive == true)
					{
						stringToDisplay="_";
					}
					else
					{
						stringToDisplay="-";
					}
					break;
					
				}
				
				break;
				
				case 1:
				switch(x2)
				{
					case 0 :
					if(isShiftActive == true)
					{
						stringToDisplay="Q";
					}
					else
					{
						stringToDisplay="q";
					}
					
					
					break;
					
					case 1 :
					if(isShiftActive == true)
					{
						stringToDisplay="W";
					}
					else
					{
						stringToDisplay="w";
					}
					
					break;
					
					case 2 :
					if(isShiftActive == true)
					{
						stringToDisplay="E";
					}
					else
					{
						stringToDisplay="e";
					}
					
					break;
					
					case 3 :
					if(isShiftActive == true)
					{
						stringToDisplay="R";
					}
					else
					{
						stringToDisplay="r";
					}
					
					break;
					
					case 4 :
					if(isShiftActive == true)
					{
						stringToDisplay="T";
					}
					else
					{
						stringToDisplay="t";
					}
					break;
					
					case 5 :
					if(isShiftActive == true)
					{
						stringToDisplay="Y";
					}
					else
					{
						stringToDisplay="y";
					}
					break;
					
					case 6 :
					if(isShiftActive == true)
					{
						stringToDisplay="U";
					}
					else
					{
						stringToDisplay="u";
					}
					break;
					
					case 7 :
					if(isShiftActive == true)
					{
						stringToDisplay="I";
					}
					else
					{
						stringToDisplay="i";
					}
					break;
					
					case 8 :
					if(isShiftActive == true)
					{
						stringToDisplay="O";
					}
					else
					{
						stringToDisplay="o";
					}
					break;
					
					case 9 :
					if(isShiftActive == true)
					{
						stringToDisplay="P";
					}
					else
					{
						stringToDisplay="p";
					}
					break;
					
					case 10 :
					if(isShiftActive == true)
					{
						stringToDisplay="{";
					}
					else
					{
						stringToDisplay="[";
					}
					break;
					
					
				}
				
				break;
				
				case 2:
				switch(x2)
				{
					case 0 :
					if(isShiftActive == true)
					{
						stringToDisplay="A";
					}
					else
					{
						stringToDisplay="a";
					}
					
					
					break;
					
					case 1 :
					if(isShiftActive == true)
					{
						stringToDisplay="S";
					}
					else
					{
						stringToDisplay="s";
					}
					
					break;
					
					case 2 :
					if(isShiftActive == true)
					{
						stringToDisplay="D";
					}
					else
					{
						stringToDisplay="d";
					}
					
					break;
					
					case 3 :
					if(isShiftActive == true)
					{
						stringToDisplay="F";
					}
					else
					{
						stringToDisplay="f";
					}
					
					break;
					
					case 4 :
					if(isShiftActive == true)
					{
						stringToDisplay="G";
					}
					else
					{
						stringToDisplay="g";
					}
					break;
					
					case 5 :
					if(isShiftActive == true)
					{
						stringToDisplay="H";
					}
					else
					{
						stringToDisplay="h";
					}
					break;
					
					case 6 :
					if(isShiftActive == true)
					{
						stringToDisplay="J";
					}
					else
					{
						stringToDisplay="j";
					}
					break;
					
					case 7 :
					if(isShiftActive == true)
					{
						stringToDisplay="K";
					}
					else
					{
						stringToDisplay="k";
					}
					break;
					
					case 8 :
					if(isShiftActive == true)
					{
						stringToDisplay="L";
					}
					else
					{
						stringToDisplay="l";
					}
					break;
					
					case 9 :
					if(isShiftActive == true)
					{
						stringToDisplay=":";
					}
					else
					{
						stringToDisplay=";";
					}
					break;
					
					case 10 :
					if(isShiftActive == true)
					{
						stringToDisplay= "\"";
					}
					else
					{
						stringToDisplay="'";
					}
					break;
					
					

				}
				
				break;
				
				case 3:
				switch(x2)
				{
					case 0 :
					if(isShiftActive == true)
					{
						stringToDisplay="Z";
					}
					else
					{
						stringToDisplay="z";
					}
					
					
					break;
					
					case 1 :
					if(isShiftActive == true)
					{
						stringToDisplay="X";
					}
					else
					{
						stringToDisplay="x";
					}
					
					break;
					
					case 2 :
					if(isShiftActive == true)
					{
						stringToDisplay="C";
					}
					else
					{
						stringToDisplay="c";
					}
					
					break;
					
					case 3 :
					if(isShiftActive == true)
					{
						stringToDisplay="V";
					}
					else
					{
						stringToDisplay="v";
					}
					
					break;
					
					case 4 :
					if(isShiftActive == true)
					{
						stringToDisplay="B";
					}
					else
					{
						stringToDisplay= "b";
					}
					break;
					
					case 5 :
					if(isShiftActive == true)
					{
						stringToDisplay="N";
					}
					else
					{
						stringToDisplay= "n";
					}
					break;
					
					case 6 :
					if(isShiftActive == true)
					{
						stringToDisplay= "M";
					}
					else
					{
						stringToDisplay= "m";
					}
					break;
					
					case 7 :
					if(isShiftActive == true)
					{
						stringToDisplay= "<";
					}
					else
					{
						stringToDisplay= ",";
					}
					break;
					
					case 8 :
					if(isShiftActive == true)
					{
						stringToDisplay= ">";
					}
					else
					{
						stringToDisplay= ".";
					}
					break;
					
					case 9 :
					if(isShiftActive == true)
					{
						stringToDisplay= "?";
					}
					else
					{
						stringToDisplay= "/";
					}
					break;
					
					case 10 :
					if(isShiftActive == true)
					{
						stringToDisplay= "+";
					}
					else
					{
						stringToDisplay= "=";
					}
					break;
					
					
				}
				
				break;
		}
			if(keyboardFont != NULL)
			{
				vita2d_font_draw_text(keyboardFont, xPosition + 32, yPosition + 48, RGBA8(255,255,255,255), 32, stringToDisplay.c_str());	
			}
						
				}
			}
			
	}
	
}


void Keyboard::AddChar(const char* charToAdd)
{
	fieldWritingTo->append(charToAdd);
}

void Keyboard::ShiftCheck()
{
	if(controller->isRightTriggerPressed == true && controller->canRightTriggerBePressed == true)
	{
		if(isShiftActive == true)
		{
			isShiftActive = false;
			return;
		}
		else
		{
			isShiftActive = true;
			return;
		}
	}
}

void Keyboard::KeyboardUpdate()
{
	if(isActive == true)
	{
		KeyboardMove();
		ShiftCheck();
		SelectKey();
		AddToElement();
		KeyboardBackspace();
		KeyboardClose();
	
	}
}
