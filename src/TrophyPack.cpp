#include "TrophyPack.h"

TrophyPack::TrophyPack(std::string tempGameName, std::string tempGameID, std::string tempImageDirectory, std::string tempPassword)
{
	gameName = tempGameName;
	gameID = tempGameID;
	imageDirectory = tempImageDirectory;
	password = tempPassword;
}