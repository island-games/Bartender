#include "Trophies.h"

Trophies::Trophies(std::string tempName, std::string tempDescription, int tempType, int tempX, int tempY)
{
	nameOfTrophy = tempName;
	descriptionOfTrophy = tempDescription;
	trophyType = tempType;
	x = tempX;
	y = tempY;
	isHidden = false;
	unlockKey = "";
	unlockRate = 0.0f;
	userUnlocked = "";



}

void Trophies::FormatText()
{
	std::string tempString = "";
	std::string currentLine = "";
	
	for(int i = 0; i < descriptionOfTrophy.size(); i++)
	{
		currentLine.append(descriptionOfTrophy.substr(i,1));
		
		if(currentLine.size() >=35)
		{
			int tempi = i;
			for(int ii = currentLine.size()-1; ii > 0; ii--)
			{
				
				if(currentLine.substr(ii,1) == " ")
				{
					tempString.append(currentLine.substr(0,ii));
					tempString.append("\n");
					currentLine = "";
					i = tempi;
					break;
				}
				
				else if(ii == 0)
				{
					tempString.append(currentLine);
					tempString.append("\n");
					currentLine = "";
					break;
				}
				else
				{
					tempi--;
				}
				
			}
		}
		
		if(i == descriptionOfTrophy.size()-1)
		{
			tempString.append(currentLine);
		}
		
	}
	
	descriptionOfTrophy = tempString;
	
}