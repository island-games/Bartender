#ifndef Trophies_h_
#define Trophies_h_
#include <string>
class Trophies{
public:

std::string nameOfTrophy;
std::string descriptionOfTrophy;
int trophyType;
int x;
int y;
bool isHidden;
std::string unlockKey;
float unlockRate;
std::string userUnlocked;

Trophies(std::string tempName, std::string tempDescription, int tempType, int tempX, int tempY);
void FormatText();
};

#endif