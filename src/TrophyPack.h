#ifndef TrophyPack_h_
#define TrophyPack_h_
#include <string>
class TrophyPack{
public:

std::string gameName;
std::string imageDirectory;
std::string gameID;
std::string password;

TrophyPack(std::string tempGameName, std::string tempGameID, std::string tempImageDirectory, std::string tempPassword);

};

#endif