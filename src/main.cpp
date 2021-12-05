#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <vita2d.h>
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/ime_dialog.h> 
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/net/http.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/processmgr.h>
#include <filesystem>
#include <zip.h>

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cstdint>


#include "controllers.h"
#include "Keyboard.h"
#include "Trophies.h"
#include "TrophyPack.h"

bool establishConnection = false;
bool displayConsole = false;
bool isResyncingTrophies = false;
bool isLogOutScreenOpen = false;

int currentScreen =1;
bool signUpScreenOpen = false;
bool passwordResetScreen = false;
int menu0Position = 0;
int menu1Position = 0;
int menu2Position = 0;
int menu1Selection = 0;
int menu2Selection = 0;
int logOutSelection = 0;

vita2d_texture* bartenderUITexture;
vita2d_texture* backgroundTexture;
vita2d_texture* trophiesTexturePack;
vita2d_font* font;
vita2d_font* keyboardFont;
std::vector<vita2d_texture*> trophyPackTexturePackList;
std::string scoreText = "loading";


struct packContents
{
	std::string packName= "";
	std::string packPassword= "";
	std::string packID = "";
};

struct trophyPackImageStruct
{
	std::string imageDirectory = "";
	vita2d_texture* bartenderUITexture = NULL;
	int texturePosition = 0;
};

controllers* controller =  new controllers(0);
Keyboard* keyboard;
std::vector<std::string> possibleTrophyPacks;
std::vector<packContents> avilableTrophyPacks;
std::vector<trophyPackImageStruct> trophyPackImageList;
std::vector<TrophyPack*> listOfTrophyPacks;
std::vector<Trophies*> listOfTrophies;



std::string zipPackWithListOfGamesPassword = "";
const char* zipPackWithListOfGamesDirectory = "ux0:Data/Bartender/MasterList.pack";
const char* imageFileDirectory = "app0:img/bartender.png";
const char* backgroundImageFileDirectory = "app0:img/background.png";
const char* fontFileDirectory = "app0:img/kenvector_future.ttf";


Trophies* openTrophy = NULL; 
std::string consoleText = "";

long experienceScore = 0;
int currentLevel = 0;

std::string loadedUserName = "";
std::string loadedPassword = "";
std::string passwordResetEmail = "";
bool isLoggedIn = false;

std::string signUpName = "";
std::string signUpEmail = "";
std::string signupPassword = "";

std::string popupMessage = "";
int popupTimer = 180;

void GeneratePopupMessage(std::string tempString)
{
	// Generates the pop-up message.
	popupMessage = tempString;
	popupTimer = 0;
}


std::string EncryptTrophy(const std::string& in) {
    // Encodes the password and returns the value
	
	std::stringstream ss;

    return ss.str(); 
}

void Logout()
{
	sceIoRemove((char*)"ux0:Data/Bartender/settings.ini");
	isLogOutScreenOpen = false;
	currentScreen = 0;
	currentLevel = 0;
	menu0Position = 0;
	menu1Position = 0;
	menu2Position = 0;
	menu1Selection = 0;
	menu2Selection = 0;
	loadedPassword = "";
	loadedUserName = "";
	isLoggedIn = false;
	passwordResetEmail = "";
	signUpEmail = "";
	signUpName = "";
	signupPassword = "";
	scoreText = "";
}

void ReturnToPreviousScreen()
{
	//Returns back to the previous screen.
	if(controller->isCirclePressed == true && controller->canCircleBePressed == true)
	{
		if(currentScreen == 1)
		{
			if(isLogOutScreenOpen == false)
			{
				isLogOutScreenOpen = true;
				return;
			}
			else
			{
				isLogOutScreenOpen = false;
				return;
			}
		}
		
		if(currentScreen == 2)
		{
			currentScreen = 1;
			menu2Selection = 0;
			menu2Position = 0;
			listOfTrophies.clear();
			return;
		}
		
		if(currentScreen == 3)
		{
			currentScreen = 2;
			openTrophy = NULL;
			return;
		}
	}
}

void OpenTrophyPack()
{
	// Opens the trophy pack in the list screen and moves on to the next screen.
	if(currentScreen == 1 && isLogOutScreenOpen == false && menu1Selection < listOfTrophyPacks.size())
	{
		if(controller->isCrossPressed == true && controller->canCrossBePressed == true)
		{
			currentScreen = 2;

			
			// Load the trophy pack.
			std::string tempString = "ux0:data/Bartender/";
			tempString.append(loadedUserName);
			tempString.append("/");
			tempString.append(listOfTrophyPacks[menu1Selection]->gameID);
			tempString.append("/");
			tempString.append(listOfTrophyPacks[menu1Selection]->gameID);
			tempString.append(".dat");

			const char* trophyPackPath = tempString.c_str();
			zip_t* zipPackToOpen = zip_open(trophyPackPath,ZIP_RDONLY,0);
			const char* tempPassword = listOfTrophyPacks[menu1Selection]->password.c_str();
			zip_set_default_password(zipPackToOpen,tempPassword);		
			zip_file_t* trophyPackFile = zip_fopen(zipPackToOpen,"trophy.msv",ZIP_FL_UNCHANGED);
			zip_stat_t* trophyPackFileStat = new zip_stat_t();
			zip_stat(zipPackToOpen,"trophy.msv",ZIP_FL_UNCHANGED,trophyPackFileStat);
			char* buffer = new char[trophyPackFileStat->size + 1];
			zip_fread(trophyPackFile,buffer,trophyPackFileStat->size);
			buffer[trophyPackFileStat->size] = '\0';
			std::string trophyPackString = buffer;
			//trophyPackString.append("\0");
			consoleText.append("\n Password Size: ");
			consoleText.append(std::to_string(listOfTrophyPacks[menu1Selection]->password.size()));
			consoleText.append("\n");
			consoleText.append("Size of Trophy Pack Buffer: ");
			consoleText.append(std::to_string(trophyPackString.size()));
			
			
			
			int itemNumber = 0;
			std::string tempStringWritingTo = "";
			for(int i = 0; i < trophyPackString.size(); i++)
			{
				if(itemNumber == 0)
				{
					listOfTrophies.push_back(new Trophies("","",0,0,0));
					itemNumber++;
				}
				
				if(trophyPackString.substr(i,1) == "|" || trophyPackString.substr(i,1) == "\n" || i == trophyPackString.size() -1 || trophyPackString.substr(i,1) == "\0")
				{
					if(i == trophyPackString.size() -1 && trophyPackString.substr(i,1) != "\0" && trophyPackString.substr(i,1) != " ")
					{
						tempStringWritingTo.append(trophyPackString.substr(i,1));
					}
					
					if(itemNumber == 1)
					{
						listOfTrophies[listOfTrophies.size()-1]->nameOfTrophy = tempStringWritingTo;
						tempStringWritingTo = "";
						itemNumber++;
						
					}
					else if(itemNumber == 2)
					{
						listOfTrophies[listOfTrophies.size()-1]->descriptionOfTrophy = tempStringWritingTo;
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 3)
					{
						listOfTrophies[listOfTrophies.size()-1]->trophyType = std::stoi(tempStringWritingTo);
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 4)
					{
						bool tempBool = false;
						
						if(tempStringWritingTo == "1" || tempStringWritingTo == "true"|| tempStringWritingTo == "True"|| tempStringWritingTo == "TRUE")
						{	
							tempBool = true;
						}					
						listOfTrophies[listOfTrophies.size()-1]->isHidden = tempBool;
						tempStringWritingTo = "";
						itemNumber++;
						
					}
					else if(itemNumber == 5)
					{
						listOfTrophies[listOfTrophies.size()-1]->x = std::stoi(tempStringWritingTo);
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 6)
					{
						listOfTrophies[listOfTrophies.size()-1]->y = std::stoi(tempStringWritingTo);
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 7)
					{
						listOfTrophies[listOfTrophies.size()-1]->unlockKey = tempStringWritingTo;
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 8)
					{
						listOfTrophies[listOfTrophies.size()-1]->userUnlocked = tempStringWritingTo;
						tempStringWritingTo = "";
						itemNumber = 0;
					}
					
					if(trophyPackString.substr(i,1) == "\0")
					{
						break;
					}
				}
				else
				{
					tempStringWritingTo.append(trophyPackString.substr(i,1));
				}
				
			}			
			
			
			
			tempString = "ux0:data/Bartender/";
			tempString.append(loadedUserName);
			tempString.append("/");
			tempString.append(avilableTrophyPacks[menu1Selection].packID);
			tempString.append("/");
			tempString.append("trophy.png");
			const char* trophyImageDirectory = tempString.c_str();
			trophiesTexturePack = vita2d_load_PNG_file(trophyImageDirectory);
			
			controller->GetCanPresses();
		}
		
	}
}

void OpenTropy()
{
	// Opens the trophy and brings you to the trophy screen.
	
	if(currentScreen == 2)
	{
		if(controller->isCrossPressed == true && controller->canCrossBePressed == true)
		{
			openTrophy = NULL;
			openTrophy = new Trophies(listOfTrophies[menu2Selection]->nameOfTrophy,listOfTrophies[menu2Selection]->descriptionOfTrophy,listOfTrophies[menu2Selection]->trophyType,listOfTrophies[menu2Selection]->x,listOfTrophies[menu2Selection]->y);
			openTrophy->isHidden = listOfTrophies[menu2Selection]->isHidden;
			openTrophy->unlockKey = listOfTrophies[menu2Selection]->unlockKey;
			openTrophy->FormatText();
			currentScreen = 3;
			controller->GetCanPresses();
			consoleText.append("unlock key:");
			consoleText.append(listOfTrophies[menu2Selection]->unlockKey);
			consoleText.append("unlock");
		}
	}
}


void OpenZipPackWithListOfGames()
{
	// Opens the file that contains the list of games.
	
	zip_t* zipPackToOpen = zip_open(zipPackWithListOfGamesDirectory,ZIP_RDONLY,0);
		
	const char* tempPassword = zipPackWithListOfGamesPassword.c_str();
	zip_set_default_password(zipPackToOpen,tempPassword);
	zip_file_t* masterFile = zip_fopen(zipPackToOpen,"masterfile.msv",ZIP_FL_UNCHANGED);
	zip_stat_t* zipStatPack = new zip_stat_t();
	zip_stat(zipPackToOpen, "masterfile.msv", ZIP_FL_UNCHANGED, zipStatPack);
	char* buffers = new char[zipStatPack->size + 1];
	buffers[zipStatPack->size] = '\0';
	zip_fread(masterFile,buffers,zipStatPack->size);
	zip_fclose(masterFile);
	zip_close(zipPackToOpen);
		
// Write the avilableTrophyPacks variable GameID|GameName|GamePassword
	std::string trophyText = buffers;
	
	int itemNumber = 0;
	std::string tempWritingTo = "";
	for(int i = 0; i < trophyText.size(); i++)
	{
		if(itemNumber == 0)
		{
			packContents tempPack; 
			avilableTrophyPacks.push_back(tempPack);
			itemNumber++;
		}
		
		if(trophyText.substr(i,1) == "|" || trophyText.substr(i,1) == "\n" || i == trophyText.size() - 1)
		{
			if(i == trophyText.size() - 1 && trophyText.substr(i,1) != "\n")
			{
				tempWritingTo.append(trophyText.substr(i,1));
			}
			if(itemNumber == 1)
			{
				avilableTrophyPacks[avilableTrophyPacks.size()-1].packID = tempWritingTo;
				tempWritingTo = "";
			}
			else if(itemNumber == 2)
			{
				avilableTrophyPacks[avilableTrophyPacks.size()-1].packName = tempWritingTo;
				tempWritingTo = "";
			}
			
			else if(itemNumber == 3)
			{
				avilableTrophyPacks[avilableTrophyPacks.size()-1].packPassword = tempWritingTo;
				tempWritingTo = "";
			}
			
			itemNumber++;
			if(itemNumber > 3)
			{
				itemNumber = 0;
			}
		}
		else
		{
			tempWritingTo.append(trophyText.substr(i,1));
		}
	}
	
	
	for(int i = 0; i < avilableTrophyPacks.size(); i++)
	{
		if(avilableTrophyPacks[i].packName.length() <= 0||
		avilableTrophyPacks[i].packID.length() <=0||
		avilableTrophyPacks[i].packPassword.length() <=0)
		{
			avilableTrophyPacks.erase(avilableTrophyPacks.begin()+i);
		}
	}
	
	
}



void CheckAgainstTrophyDatabase()
{
	// Checks the list of possible trophies against the downloaded database
	listOfTrophyPacks.clear();
	for(int i = 0; i < possibleTrophyPacks.size(); i++)
	{
		for(int n = 0; n < avilableTrophyPacks.size(); n++)
		{
			
			if(possibleTrophyPacks[i] == avilableTrophyPacks[n].packID)
			{
				std::string tempString = "ux0:Data/Bartender/";
				tempString.append(loadedUserName);
				tempString.append("/");
				tempString.append(avilableTrophyPacks[n].packID);
				tempString.append("/");
				tempString.append("game.png");
				
				
				listOfTrophyPacks.push_back(new TrophyPack(avilableTrophyPacks[n].packName,avilableTrophyPacks[n].packID, tempString,avilableTrophyPacks[n].packPassword));
				break;
			}
		}
	}
}

void GetListOfPossibleTrophyPacks()
{
	// Generates a list of possible trophy packs.
	std::string tempString = "ux0:Data/Bartender/";
	tempString.append(loadedUserName);
	SceUID trophyPackFolder = sceIoDopen(tempString.c_str());
	SceIoDirent listOfDirs;
	possibleTrophyPacks.clear();
	while(sceIoDread(trophyPackFolder, &listOfDirs) > 0)
	{
		possibleTrophyPacks.push_back(listOfDirs.d_name);
		consoleText.append(listOfDirs.d_name);
		consoleText.append("\n");
	}
	
	CheckAgainstTrophyDatabase();
}

void AddPreviousImage()
{
	// Adds the previous image for scrolling
	trophyPackImageStruct trophyImage;
	trophyImage.texturePosition = menu1Selection;
	trophyImage.imageDirectory = listOfTrophyPacks[menu1Selection]->imageDirectory;
	trophyImage.bartenderUITexture = vita2d_load_PNG_file(trophyImage.imageDirectory.c_str());
	trophyPackImageList.push_back(trophyImage);
}

void AddNextImage()
{
	// Adds the next image for scrolling
	trophyPackImageStruct trophyImage;
	trophyImage.texturePosition = menu1Selection;
	trophyImage.imageDirectory = listOfTrophyPacks[menu1Selection]->imageDirectory;
	trophyImage.bartenderUITexture = vita2d_load_PNG_file(trophyImage.imageDirectory.c_str());
	trophyPackImageList.push_back(trophyImage);
}

void LoadInitialListOfTrophyPackImages()
{
	// Loads the initial list of trophy pack images
	for(int i = menu1Position; i <menu1Position + 4; i++)
	{
		if(i < listOfTrophyPacks.size())
		{
		trophyPackImageStruct trophyImage;
		trophyImage.texturePosition = i;
		trophyImage.imageDirectory = listOfTrophyPacks[i]->imageDirectory;
		trophyImage.bartenderUITexture = vita2d_load_PNG_file(trophyImage.imageDirectory.c_str());
		trophyPackImageList.push_back(trophyImage);
		}
	}
}

void ScrollUp()
{
	// Scrolls the screen up
	if(controller->isUpPressed == true && controller->canUpBePressed == true)
	{
		if(currentScreen == 0)
		{
			if(keyboard->isActive == false)
			{
				if(signUpScreenOpen == true || passwordResetScreen == true)
				{
					if(signUpScreenOpen == true)
					{
						switch(menu0Position)
						{
							case 0:
							menu0Position = 3;
							break;
							
							case 1:
							menu0Position--;
							break;
							
							case 2:
							menu0Position--;
							break;
							
							case 3:
							menu0Position--;
							break;
						}
					}
					
					if(passwordResetScreen == true)
					{
						switch(menu0Position)
						{
							case 0:
							menu0Position = 1;
							break;
							
							case 1:
							menu0Position--;
							break;
						}
					}
				}
				else
				{
					switch(menu0Position)
					{
						case 0:
						menu0Position = 4;
						break;
						
						case 1:
						menu0Position--;
						break;
						
						case 2:
						menu0Position--;
						break;
						
						case 3:
						menu0Position--;
						break;
						
						case 4:
						menu0Position--;
						break;
					}
				}
			}
		}
		else if(currentScreen == 1 && isLogOutScreenOpen == false)
		{
			menu1Selection--;
			if(menu1Selection <0)
			{
				menu1Selection = 0;
			}
			if(menu1Selection < menu1Position)
			{
				menu1Position--;
				AddPreviousImage();
			}
		}
		else if(currentScreen == 2)
		{
			menu2Selection--;
			if(menu2Selection <0)
			{
				menu2Selection = 0;
			}
			if(menu2Selection < menu2Position)
			{
				menu2Position--;
			}
		}
		

	}
}

void ScrollDown()
{
	// Scrolls the screen down.
	if(controller->isDownPressed == true && controller->canDownBePressed == true)
	{
		if(currentScreen == 0)
		{
			if(keyboard->isActive == false)
			{
				if(signUpScreenOpen == true || passwordResetScreen == true)
				{
					if(signUpScreenOpen == true)
					{
						switch(menu0Position)
						{
							case 0:
							menu0Position++;
							break;
							
							case 1:
							menu0Position++;
							break;
							
							case 2:
							menu0Position++;
							break;
							
							case 3:
							menu0Position = 0;
							break;
						}
					}
					
					if(passwordResetScreen == true)
					{
						switch(menu0Position)
						{
							case 0:
							menu0Position++;
							break;
							
							case 1:
							menu0Position = 0;
							break;
						}
					}
				}
				else
				{
						switch(menu0Position)
					{
						case 0:
						menu0Position++;
						break;
						
						case 1:
						menu0Position++;
						break;
						
						case 2:
						menu0Position++;
						break;
						
						case 3:
						menu0Position++;
						break;
						
						case 4:
						menu0Position = 0;
						break;
					}
				}
				
			}
		}
		else if(currentScreen == 1 && isLogOutScreenOpen == false)
		{
			if(!listOfTrophyPacks.empty())
			{
				menu1Selection++;
				if(menu1Selection > listOfTrophyPacks.size() -1)
				{
					menu1Selection = listOfTrophyPacks.size() -1;
				}
				
				if(menu1Selection > menu1Position+4)
				{
					menu1Position++;
					AddNextImage();
				}
			}
		}
		else if(currentScreen == 2)
		{
			menu2Selection++;
			if(menu2Selection > listOfTrophies.size() -1)
			{
				menu2Selection = listOfTrophies.size() -1;
			}
			if(menu2Selection > menu2Position+4)
			{
				menu2Position++;
			}
		}
		
		
	}	
}



void WriteTrophiesToDB(const char* trophyPackPath, const char* encryptionKey2)
{
	// Writes the trophy information in memory back to the file.
	sceIoRemove(trophyPackPath);
	zip_t* zipFile = zip_open(trophyPackPath, ZIP_CREATE, 0);
	std::string myFileString = "";
	
	for (int i = 0; i < listOfTrophies.size(); i++)
	{
		myFileString.append(listOfTrophies[i]->nameOfTrophy);
		myFileString.append("|");
		myFileString.append(listOfTrophies[i]->descriptionOfTrophy);
		myFileString.append("|");
		myFileString.append(std::to_string(listOfTrophies[i]->trophyType));
		myFileString.append("|");
		myFileString.append(std::to_string(listOfTrophies[i]->isHidden));
		myFileString.append("|");
		myFileString.append(std::to_string(listOfTrophies[i]->x));
		myFileString.append("|");
		myFileString.append(std::to_string(listOfTrophies[i]->y));
		myFileString.append("|");
		myFileString.append(listOfTrophies[i]->unlockKey);
		myFileString.append("|");
		myFileString.append(listOfTrophies[i]->userUnlocked);
		
		if(i != listOfTrophies.size()-1)
		{
			myFileString.append("\n");
		}
		
	}
	
	const char *fileToWrite = myFileString.c_str();
	zip_source_t * sourceFileToWrite = zip_source_buffer_create(fileToWrite, strlen(fileToWrite), 0, 0);
	int replaceCount = zip_file_add(zipFile,"trophy.msv", sourceFileToWrite, ZIP_FL_OVERWRITE|ZIP_FL_ENC_GUESS);
	zip_stat_t* zipStatPack = new zip_stat_t();
	zip_stat(zipFile, "trophy.msv", ZIP_FL_UNCHANGED, zipStatPack);
	zip_uint64_t fileIndex = zipStatPack->index;
	zip_file_set_encryption(zipFile, fileIndex, ZIP_EM_AES_256, encryptionKey2);
	
	int zipCloseCount = zip_close(zipFile);
	
}

void DownloadTrophiesNetwork(std::string tempID, std::string tempPassword)
{
	// Downloads the completed trophies from the server
	std::vector<std::string> listOfCodes;
	std::vector<std::string> downloadedCodes;

	for(int i = 0; i < listOfTrophies.size(); i++)
	{
		std::string tempKey = "";
		//Private function
		std::string code = EncryptTrophy(tempKey);
		listOfCodes.push_back(code);
	}
	
	
	std::string textToPost ="";
	//Private stuff
	const char* textToPostChar = textToPost.c_str();
	
		//Check the login
	int tpl = sceHttpCreateTemplate("Signup", 1, 1);
	int conn = sceHttpCreateConnectionWithURL(tpl, downloadURL, 0);
	int request = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_POST, downloadURL, strlen(textToPostChar));
	int header = sceHttpAddRequestHeader(request, "Content-Type", "application/x-www-form-urlencoded", SCE_HTTP_HEADER_OVERWRITE);
	int handle = sceHttpSendRequest(request, textToPostChar, strlen(textToPostChar));
	std::string readData = "";
	char data[16*1024];
	long read = 0;

	// read data until finished
	while ((read = sceHttpReadData(request, &data, sizeof(data)-1)) > 0) {
		
		// writing the count of read bytes from the data buffer to the file
		data[read] = '\0';
			readData.append(data);
		
		//int write = sceIoWrite(fh, data, read);
		
	}
	
	//Parsing the file
	std::string stringBuilding = "";
	for(int i = 0; i < readData.size(); i++)
	{
		if(readData.substr(i,1) == "\n" || i == readData.size() - 1)
		{
			if(readData.substr(i,1) != "\n")
			{
				stringBuilding.append(readData.substr(i,1));
			}
			downloadedCodes.push_back(stringBuilding);
			stringBuilding = "";
		}
		else
		{
			stringBuilding.append(readData.substr(i,1));
		}
	}
	

	
	
	for(int i = 0; i < listOfCodes.size(); i++)
	{
		for(int n = 0; n < downloadedCodes.size(); n++)
		{
			if(listOfCodes[i] == downloadedCodes[n])
			{
				listOfTrophies[i]->unlockKey = "1";
				break;
			}
		}
	}
}

void DownloadTrophiesFromServer()
{
	// Opens the trophy pack then calls the function to download the completed trohies from the server and then writes them to the DB.
	for(int i = 0; i < listOfTrophyPacks.size(); i++)
		{
			std::string tempString = "ux0:Data/Bartender/";
			tempString.append(loadedUserName);
			tempString.append("/");
			tempString.append(listOfTrophyPacks[i]->gameID);
			tempString.append("/");
			tempString.append(listOfTrophyPacks[i]->gameID);
			tempString.append(".dat");
			const char* trophyPackPath = tempString.c_str();
			zip_t* zipPackToOpen = zip_open(trophyPackPath,ZIP_RDONLY,0);
			const char* tempPassword = listOfTrophyPacks[i]->password.c_str();
			zip_set_default_password(zipPackToOpen,tempPassword);
			zip_file_t* trophyPackFile = zip_fopen(zipPackToOpen,"trophy.msv",ZIP_FL_UNCHANGED);
			zip_stat_t* trophyPackFileStat = new zip_stat_t();;
			zip_stat(zipPackToOpen,"trophy.msv",ZIP_FL_UNCHANGED,trophyPackFileStat);
			char* buffer = new char[trophyPackFileStat->size + 1];
			zip_fread(trophyPackFile,buffer,trophyPackFileStat->size);
			buffer[trophyPackFileStat->size] = '\0';
			std::string trophyPackString = buffer;
			zip_fclose(trophyPackFile);
			zip_close(zipPackToOpen);
			int itemNumber = 0;
			std::string tempStringWritingTo = "";
			
			for(int n = 0; n < trophyPackString.size(); n++)
			{
				if(itemNumber == 0)
				{
					listOfTrophies.push_back(new Trophies("","",0,0,0));
					itemNumber++;
				}
				
				if(trophyPackString.substr(n,1) == "|" || trophyPackString.substr(n,1) == "\n" || n == trophyPackString.size()-1)
				{
					if(n == trophyPackString.size()-1 &&  trophyPackString.substr(n,1) != " " &&  trophyPackString.substr(n,1) != "\n")
					{
						tempStringWritingTo.append(trophyPackString.substr(n,1));
					}
					
					if(itemNumber == 1)
					{
						listOfTrophies[listOfTrophies.size()-1]->nameOfTrophy = tempStringWritingTo;
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 2)
					{
						listOfTrophies[listOfTrophies.size()-1]->descriptionOfTrophy = tempStringWritingTo;
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 3)
					{
						listOfTrophies[listOfTrophies.size()-1]->trophyType = std::stoi(tempStringWritingTo);
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 4)
					{
						bool tempBool = false;
						
						if(tempStringWritingTo == "1" || tempStringWritingTo == "true"|| tempStringWritingTo == "True"|| tempStringWritingTo == "TRUE")
						{	
							tempBool = true;
						}					
						listOfTrophies[listOfTrophies.size()-1]->isHidden = tempBool;
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 5)
					{
						listOfTrophies[listOfTrophies.size()-1]->x = std::stoi(tempStringWritingTo);
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 6)
					{
						listOfTrophies[listOfTrophies.size()-1]->y = std::stoi(tempStringWritingTo);
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 7)
					{
						listOfTrophies[listOfTrophies.size()-1]->unlockKey = tempStringWritingTo;
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 8)
					{
						listOfTrophies[listOfTrophies.size()-1]->userUnlocked = tempStringWritingTo;
						tempStringWritingTo = "";
						itemNumber = 0;
					}
				}
				else
				{
					tempStringWritingTo.append(trophyPackString.substr(n,1));
				}
				
			}	
			
			
			DownloadTrophiesNetwork(listOfTrophyPacks[i]->gameID, tempPassword);
			WriteTrophiesToDB(trophyPackPath, tempPassword);
			listOfTrophies.clear();
			
		}
		
		isResyncingTrophies = false;
}

std::string UploadFlagToServer(std::string tempGameID, std::string trophyCode, std::string trophyScore)
{
	// Writes the trophies to the server
	std::string textToPost =""// Private;

	
		const char* textToPostChar = textToPost.c_str();
		const char* downloadURL = "";
		
		//Check the login
	int tpl = sceHttpCreateTemplate("Sync", 1, 1);
	int conn = sceHttpCreateConnectionWithURL(tpl, downloadURL, 0);
	int request = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_POST, downloadURL, strlen(textToPostChar));
	int header = sceHttpAddRequestHeader(request, "Content-Type", "application/x-www-form-urlencoded", SCE_HTTP_HEADER_OVERWRITE);
	int handle = sceHttpSendRequest(request, textToPostChar, strlen(textToPostChar));
	std::string readData = "";
	char data[16*1024];
	long read = 0;

	// read data until finished
		while ((read = sceHttpReadData(request, &data, sizeof(data))) > 0) {
			
			// writing the count of read bytes from the data buffer to the file
			data[read] = '\0';
				readData.append(data);
		}
	
	return readData;
}

void CheckWhatNeedsToBeSynced(std::string tempID, std::string tempPassword)
{
	// Checks which trophies need to be synced
	for(int i = 0; i < listOfTrophies.size(); i++)
	{
		if(listOfTrophies[i]->unlockKey.size() > 1)
		{

			std::string tempKey = "";
			//Private Trophy Encrypt Function
			std::string code = EncryptTrophy(tempKey);
			bool success = false;
			
			if(code == listOfTrophies[i]->unlockKey && loadedUserName == listOfTrophies[i]->userUnlocked)
			{
				std::string trophyScore = "";
				switch(listOfTrophies[i]->trophyType)
				{
					case 1:
					trophyScore = "5";
					break;
					
					case 2:
					trophyScore = "10";
					break;
					
					case 3:
					trophyScore = "20";
					break;
					
					case 4:
					trophyScore = "50";
					break;
				}
				
				std::string successCheck = UploadFlagToServer(tempID,listOfTrophies[i]->unlockKey, trophyScore);
				if(successCheck == "Success")
				{
					success = true;
				}
			}
			else
			{
				listOfTrophies[i]->unlockKey = "0";
			}
			
			
			if(success == true)
			{
				listOfTrophies[i]->unlockKey = "1";
			}
		}
	}
}

void SyncTrophyPacks()
{
	// Begins the syncing process and calls the check to be synced function and then syncs with the server
		for(int i = 0; i < listOfTrophyPacks.size(); i++)
		{
			std::string tempString = "ux0:Data/Bartender/";
			tempString.append(loadedUserName);
			tempString.append("/");
			tempString.append(listOfTrophyPacks[i]->gameID);
			tempString.append("/");
			tempString.append(listOfTrophyPacks[i]->gameID);
			tempString.append(".dat");
			const char* trophyPackPath = tempString.c_str();
			zip_t* zipPackToOpen = zip_open(trophyPackPath,ZIP_RDONLY,0);
			const char* tempPassword = listOfTrophyPacks[i]->password.c_str();
			zip_set_default_password(zipPackToOpen,tempPassword);
			zip_file_t* trophyPackFile = zip_fopen(zipPackToOpen,"trophy.msv",ZIP_FL_UNCHANGED);
			zip_stat_t* trophyPackFileStat = new zip_stat_t();;
			zip_stat(zipPackToOpen,"trophy.msv",ZIP_FL_UNCHANGED,trophyPackFileStat);
			char* buffer = new char[trophyPackFileStat->size + 1];
			zip_fread(trophyPackFile,buffer,trophyPackFileStat->size);
			buffer[trophyPackFileStat->size] = '\0';
			std::string trophyPackString = buffer;
			zip_fclose(trophyPackFile);
			zip_close(zipPackToOpen);
			int itemNumber = 0;
			std::string tempStringWritingTo = "";
			
			for(int n = 0; n < trophyPackString.size(); n++)
			{
				if(itemNumber == 0)
				{
					listOfTrophies.push_back(new Trophies("","",0,0,0));
					itemNumber++;
				}
				
				if(trophyPackString.substr(n,1) == "|" || trophyPackString.substr(n,1) == "\n" || n == trophyPackString.size()-1)
				{
					if(n == trophyPackString.size()-1 &&  trophyPackString.substr(n,1) != " " &&  trophyPackString.substr(n,1) != "\n")
					{
						tempStringWritingTo.append(trophyPackString.substr(n,1));
					}
					
					if(itemNumber == 1)
					{
						listOfTrophies[listOfTrophies.size()-1]->nameOfTrophy = tempStringWritingTo;
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 2)
					{
						listOfTrophies[listOfTrophies.size()-1]->descriptionOfTrophy = tempStringWritingTo;
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 3)
					{
						listOfTrophies[listOfTrophies.size()-1]->trophyType = std::stoi(tempStringWritingTo);
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 4)
					{
						bool tempBool = false;
						
						if(tempStringWritingTo == "1" || tempStringWritingTo == "true"|| tempStringWritingTo == "True"|| tempStringWritingTo == "TRUE")
						{	
							tempBool = true;
						}					
						listOfTrophies[listOfTrophies.size()-1]->isHidden = tempBool;
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 5)
					{
						listOfTrophies[listOfTrophies.size()-1]->x = std::stoi(tempStringWritingTo);
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 6)
					{
						listOfTrophies[listOfTrophies.size()-1]->y = std::stoi(tempStringWritingTo);
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 7)
					{
						listOfTrophies[listOfTrophies.size()-1]->unlockKey = tempStringWritingTo;
						tempStringWritingTo = "";
						itemNumber++;
					}
					else if(itemNumber == 8)
					{
						listOfTrophies[listOfTrophies.size()-1]->userUnlocked = tempStringWritingTo;
						tempStringWritingTo = "";
						itemNumber = 0;
						
					}
				}
				else
				{
					tempStringWritingTo.append(trophyPackString.substr(n,1));
				}
				
			}	
			
			CheckWhatNeedsToBeSynced(listOfTrophyPacks[i]->gameID, tempPassword);
			WriteTrophiesToDB(trophyPackPath, tempPassword);
			listOfTrophies.clear();
			
		}
	
}

void GetScore()
{
	// Writes the trophies to the server
	std::string textToPost = "" //Private;

	
		const char* textToPostChar = textToPost.c_str();
		const char* downloadURL = "";
		
		//Check the login
	int tpl = sceHttpCreateTemplate("Sync", 1, 1);
	int conn = sceHttpCreateConnectionWithURL(tpl, downloadURL, 0);
	int request = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_POST, downloadURL, strlen(textToPostChar));
	int header = sceHttpAddRequestHeader(request, "Content-Type", "application/x-www-form-urlencoded", SCE_HTTP_HEADER_OVERWRITE);
	int handle = sceHttpSendRequest(request, textToPostChar, strlen(textToPostChar));
	std::string readData = "";
	char data[16*1024];
	long read = 0;

	// read data until finished
		while ((read = sceHttpReadData(request, &data, sizeof(data))) > 0) {
			
			// writing the count of read bytes from the data buffer to the file
			data[read] = '\0';
				readData.append(data);
		}
	
	scoreText = readData;
}

void initHttp()
{

	sceSysmoduleLoadModule(SCE_SYSMODULE_HTTP);
	sceHttpInit(1*1024*1024);
}


void initNet()
{

	sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
	

	SceNetInitParam netInitParam;
	int size = 1*1024*1024;
	netInitParam.memory = malloc(size);
	netInitParam.size = size;
	netInitParam.flags = 0;
	sceNetInit(&netInitParam);

	sceNetCtlInit();
}


void EstablishNetwork()
{
	initNet();
	initHttp();

}

void DownloadTrophyPack()
{
	// This function downloads the pack from the server that contains the list of possible games that contains trophies.
	
	std::string downloadURL = "";
	std::string downloadedFile = "";
	const char* tempFile = "";
		//Download the new pack list.
	int tpl = sceHttpCreateTemplate("Pack Download", 1, 1);
	int conn = sceHttpCreateConnectionWithURL(tpl, downloadURL.c_str(), 0);
	int request = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_GET, downloadURL.c_str(), 0);
	int handle = sceHttpSendRequest(request, NULL, 0);
	int fh = sceIoOpen(tempFile, SCE_O_WRONLY | SCE_O_CREAT, 0777);
	
	unsigned char data[16*1024];
	int read = 0;

	// read data until finished
	while ((read = sceHttpReadData(request, &data, sizeof(data))) > 0) {
		
		// writing the count of read bytes from the data buffer to the file
				
		int write = sceIoWrite(fh, data, read);
		
	}
	sceIoClose(fh);
	fh = sceIoOpen(tempFile, SCE_O_RDONLY, 0777);
	long fileLength = sceIoLseek(fh, 0, SCE_SEEK_END);
	long extraLength= sceIoLseek(fh, 0,SCE_SEEK_SET);
	char reread[fileLength];
	int fileSizeRead = sceIoRead(fh,reread, fileLength);
	
	
	downloadedFile.append(reread);
	

	
	sceIoClose(fh);
	sceIoClose(tempFile);
}

void DrawConsole()
{
	// Draws the console

}

void DrawPopUp()
{
	// Draws the pop-up
	if(popupTimer < 180)
	{
		popupTimer++;
		vita2d_draw_rectangle(330, 0, 630, 72, RGBA8(0,0,0,255));
		vita2d_font_draw_text(font, 340,28 , RGBA8(255,255,255,255), 18, popupMessage.c_str());
	}
}

void DrawSyncPopup()
{
	// Draw the pop-up
	if(isResyncingTrophies == true)
	{
		vita2d_draw_rectangle(180, 122, 600, 300, RGBA8(200,200,200,255));
		vita2d_draw_rectangle(183, 125, 594, 294, RGBA8(65,76,90,255));
		vita2d_font_draw_text(font, 270,250 , RGBA8(255,255,255,255), 32, (char*)"Resyncing Trophies");
		vita2d_font_draw_text(font, 340,290 , RGBA8(255,255,255,255), 32, (char*)"Please Wait");
		
	}
	
}


void Draw()
{
	// The main draw call
	vita2d_start_drawing();
	vita2d_clear_screen();
	vita2d_draw_texture_part(backgroundTexture, 0, 0, 0, 0, 960, 544);
	
	if(currentScreen == 0)
	{
		if(signUpScreenOpen == false && passwordResetScreen == false)
		{
			// Background
			vita2d_draw_rectangle(20, 20, 920, 504, RGBA8(0,0,0,255));
			vita2d_draw_rectangle(24, 24, 912, 496, RGBA8(200,200,200,255));
		
			//User Name
			vita2d_font_draw_text(font, 126, 68, RGBA8(0,0,0,255), 24, "User Name:");
			vita2d_draw_rectangle(126, 72, 708, 64, RGBA8(0,0,0,255));
			
			if(menu0Position == 0)
			{
				vita2d_draw_rectangle(130, 76, 700, 56, RGBA8(255,255,175,255));
			}
			else
			{
				vita2d_draw_rectangle(130, 76, 700, 56, RGBA8(255,255,255,255));
			}
			
			
			std::string userNameFieldDisplaying = "";
			if(loadedUserName.size() >33)
			{
				userNameFieldDisplaying = loadedUserName.substr(loadedUserName.size() - 33);
			}
			else
			{
				userNameFieldDisplaying = loadedUserName;
			}
			vita2d_font_draw_text(keyboardFont, 134, 116, RGBA8(0,0,0,255), 32, userNameFieldDisplaying.c_str());
			
	 
			// Password
			vita2d_font_draw_text(font, 126, 188, RGBA8(0,0,0,255), 24, "Password:");
			vita2d_draw_rectangle(126, 192, 708, 64, RGBA8(0,0,0,255));
			if(menu0Position == 1)
			{
				vita2d_draw_rectangle(130, 196, 700, 56, RGBA8(255,255,175,255));
			}
			else
			{
				vita2d_draw_rectangle(130, 196, 700, 56, RGBA8(255,255,255,255));
			}
			
			std::string userPasswordFieldDisplaying = "";
			if(loadedPassword.size() >33)
			{
				userPasswordFieldDisplaying = loadedPassword.substr(loadedUserName.size() - 33);
			}
			else
			{
				userPasswordFieldDisplaying = loadedPassword;
			}
			vita2d_font_draw_text(keyboardFont, 134, 236, RGBA8(0,0,0,255), 32, userPasswordFieldDisplaying.c_str());
			
			
			
			// Login Button
			vita2d_draw_rectangle(416, 280, 128, 48, RGBA8(0,0,0,255));
			if(menu0Position == 2)
			{
				vita2d_draw_rectangle(420, 284, 120, 40, RGBA8(120,0,120,255));
			}
			else
			{
				vita2d_draw_rectangle(420, 284, 120, 40, RGBA8(0,0,120,255));
			}
			vita2d_font_draw_text(font, 435, 310, RGBA8(255,255,255,255), 24, "Login");
			
			// Sign Up Button
			vita2d_draw_rectangle(416, 350, 128, 48, RGBA8(0,0,0,255));
			if(menu0Position == 3)
			{
				vita2d_draw_rectangle(420, 354, 120, 40, RGBA8(120,0,120,255));
			}
			else
			{
				vita2d_draw_rectangle(420, 354, 120, 40, RGBA8(0,0,120,255));
			}
			vita2d_font_draw_text(font, 426, 380, RGBA8(255,255,255,255), 24, "Sign Up");
			
			// Forgot Password Button
			vita2d_draw_rectangle(316, 420, 328, 48, RGBA8(0,0,0,255));
			if(menu0Position == 4)
			{
				vita2d_draw_rectangle(320, 424, 320, 40, RGBA8(120,0,120,255));
			}
			else
			{
				vita2d_draw_rectangle(320, 424, 320, 40, RGBA8(0,0,120,255));
			}
			vita2d_font_draw_text(font, 337, 450, RGBA8(255,255,255,255), 24, "Forgot Password");
			
		}
		
		if(signUpScreenOpen == true)
		{
			// Background
			vita2d_draw_rectangle(20, 20, 920, 504, RGBA8(0,0,0,255));
			vita2d_draw_rectangle(24, 24, 912, 496, RGBA8(200,200,200,255));
		
			//Email
			vita2d_font_draw_text(font, 126, 68, RGBA8(0,0,0,255), 24, "Email Address:");
			vita2d_draw_rectangle(126, 72, 708, 64, RGBA8(0,0,0,255));
			
			if(menu0Position == 0)
			{
				vita2d_draw_rectangle(130, 76, 700, 56, RGBA8(255,255,175,255));
			}
			else
			{
				vita2d_draw_rectangle(130, 76, 700, 56, RGBA8(255,255,255,255));
			}
			std::string emailFieldDisplaying = "";
			if(signUpEmail.size() >33)
			{
				emailFieldDisplaying = signUpEmail.substr(loadedUserName.size() - 33);
			}
			else
			{
				emailFieldDisplaying = signUpEmail;
			}
			vita2d_font_draw_text(keyboardFont, 134, 116, RGBA8(0,0,0,255), 32, emailFieldDisplaying.c_str());
			
			
			
			
	 
			//User Name
			vita2d_font_draw_text(font, 126, 188, RGBA8(0,0,0,255), 24, "User Name:");
			vita2d_draw_rectangle(126, 192, 708, 64, RGBA8(0,0,0,255));
			if(menu0Position == 1)
			{
				vita2d_draw_rectangle(130, 196, 700, 56, RGBA8(255,255,175,255));
			}
			else
			{
				vita2d_draw_rectangle(130, 196, 700, 56, RGBA8(255,255,255,255));
			}
			
			std::string userNameFieldDisplaying = "";
			if(signUpName.size() >33)
			{
				userNameFieldDisplaying = signUpName.substr(loadedUserName.size() - 33);
			}
			else
			{
				userNameFieldDisplaying = signUpName;
			}
			vita2d_font_draw_text(keyboardFont, 134, 236, RGBA8(0,0,0,255), 32, userNameFieldDisplaying.c_str());
			
			
			
			
			
			//Password
			vita2d_font_draw_text(font, 126,308, RGBA8(0,0,0,255), 24, "Password:");
			vita2d_draw_rectangle(126, 312, 708, 64, RGBA8(0,0,0,255));
			if(menu0Position == 2)
			{
				vita2d_draw_rectangle(130, 316, 700, 56, RGBA8(255,255,175,255));
			}
			else
			{
				vita2d_draw_rectangle(130, 316, 700, 56, RGBA8(255,255,255,255));
			}
			std::string passwordFieldDisplaying = "";
			if(signupPassword.size() >33)
			{
				passwordFieldDisplaying = signupPassword.substr(loadedUserName.size() - 33);
			}
			else
			{
				passwordFieldDisplaying = signupPassword;
			}
			vita2d_font_draw_text(keyboardFont, 134, 356, RGBA8(0,0,0,255), 32, passwordFieldDisplaying.c_str());
			
			
			
			
			// Sign Up Button
			vita2d_draw_rectangle(392, 420, 128, 48, RGBA8(0,0,0,255));
			if(menu0Position == 3)
			{
				vita2d_draw_rectangle(396, 424, 120, 40, RGBA8(120,0,120,255));
			}
			else
			{
				vita2d_draw_rectangle(396, 424, 120, 40, RGBA8(0,0,120,255));
			}
			vita2d_font_draw_text(font, 402, 450, RGBA8(255,255,255,255), 24, "Submit");
			
		}
	

		if(passwordResetScreen == true)
		{
			// Background
			vita2d_draw_rectangle(20, 20, 920, 504, RGBA8(0,0,0,255));
			vita2d_draw_rectangle(24, 24, 912, 496, RGBA8(200,200,200,255));
		
			//Email
			vita2d_font_draw_text(font, 126, 68, RGBA8(0,0,0,255), 24, "Email Address:");
			vita2d_draw_rectangle(126, 72, 708, 64, RGBA8(0,0,0,255));
			
			if(menu0Position == 0)
			{
				vita2d_draw_rectangle(130, 76, 700, 56, RGBA8(255,255,175,255));
			}
			else
			{
				vita2d_draw_rectangle(130, 76, 700, 56, RGBA8(255,255,255,255));
			}
			std::string emailFieldDisplaying = "";
			if(passwordResetEmail.size() >33)
			{
				emailFieldDisplaying = passwordResetEmail.substr(loadedUserName.size() - 33);
			}
			else
			{
				emailFieldDisplaying = passwordResetEmail;
			}
			vita2d_font_draw_text(keyboardFont, 134, 116, RGBA8(0,0,0,255), 32, emailFieldDisplaying.c_str());
			
			
			// Sign Up Button
			vita2d_draw_rectangle(392, 220, 128, 48, RGBA8(0,0,0,255));
			if(menu0Position == 1)
			{
				vita2d_draw_rectangle(396, 224, 120, 40, RGBA8(120,0,120,255));
			}
			else
			{
				vita2d_draw_rectangle(396, 224, 120, 40, RGBA8(0,0,120,255));
			}
			vita2d_font_draw_text(font, 402, 250, RGBA8(255,255,255,255), 24, "Submit");
			
			
		}
	}
	
	if(currentScreen == 1)
	{	
		// Draw Header
		
		// To do: drop the curve down in paint.net
		vita2d_draw_texture_part(backgroundTexture, 0, 0, 0, 580, 960, 64);
		
		vita2d_draw_texture_part(backgroundTexture, 3, 0, 0, 660, 95, 48);
		vita2d_font_draw_textf(font, 600, 20, RGBA8(0,0,0,255), 24, "%s" "%s", (char*)"Bar Tab: ", scoreText.c_str());
		vita2d_font_draw_text(font, 120, 20, RGBA8(0,0,0,255), 24, (char*)"Bartender");
		
		// Draw Boxes
		int itemNumber = 0;
		for(int i = menu1Position; i <= menu1Position + 4; i++)
		{
			if(menu1Selection != i)
			{
				vita2d_draw_texture_part(bartenderUITexture, 10, 68 + (itemNumber * 95), 0, 0, 940, 100);
			}
			else
			{
				vita2d_draw_texture_part(bartenderUITexture, 10, 68 + (itemNumber * 95), 0, 100, 940, 100);
			}
			itemNumber++;
		}
		itemNumber = 0;
		// Write and Draw Game information
		for(int i = menu1Position; i <= menu1Position + 4; i++)
		{
			if(i < listOfTrophyPacks.size())
			{
				for(int ii = 0; ii < trophyPackImageList.size(); ii++)
				{
					if(trophyPackImageList[ii].texturePosition == i)
					{
						vita2d_draw_texture_part(trophyPackImageList[ii].bartenderUITexture, 30, 77 + (itemNumber * 95), 0, 0, 150, 80);
					}
				}
				
				vita2d_font_draw_text(font, 200, 118 + (itemNumber * 95), RGBA8(255,255,255,255), 24, listOfTrophyPacks[i]->gameName.c_str());
			}
			itemNumber++;
		}
		
		if(isLogOutScreenOpen == true)
		{
			// Pop-Up Outline
			vita2d_draw_rectangle(230, 182 , 500, 240, RGBA8(255,255,255,255));
			vita2d_draw_rectangle(233, 185 , 494, 234, RGBA8(65,76,90,255));
			vita2d_font_draw_text(font, 265, 270,RGBA8(255,255,255,255), 18, (char*)"Are you sure you want to log out?");
			
			
			if(logOutSelection == 0)
			{
			
			// Yes Button
			vita2d_draw_rectangle(303, 330 , 150, 60, RGBA8(0,0,0,255));
			vita2d_draw_rectangle(305, 332 , 146, 56, RGBA8(120,0,120,255));
			vita2d_font_draw_text(font, 342, 370,RGBA8(255,255,255,255), 32, (char*)"Yes");
			
			// No Button
			vita2d_draw_rectangle(507, 330 , 150, 60, RGBA8(0,0,0,255));
			vita2d_draw_rectangle(509, 332 , 146, 56, RGBA8(0,0,120,255));
			vita2d_font_draw_text(font, 562, 370,RGBA8(255,255,255,255), 32, (char*)"No");
			
			}
			else if(logOutSelection == 1){
			// Yes Button
			vita2d_draw_rectangle(303, 330 , 150, 60, RGBA8(0,0,0,255));
			vita2d_draw_rectangle(305, 332 , 146, 56, RGBA8(0,0,120,255));
			vita2d_font_draw_text(font, 342, 370,RGBA8(255,255,255,255), 32, (char*)"Yes");
			
			// No Button
			vita2d_draw_rectangle(507, 330 , 150, 60, RGBA8(0,0,0,255));
			vita2d_draw_rectangle(509, 332 , 146, 56, RGBA8(120,0,120,255));
			vita2d_font_draw_text(font, 562, 370,RGBA8(255,255,255,255), 32, (char*)"No");
			}
			
			
		}
	}
	
	if(currentScreen == 2)
	{
		// To do: drop the curve down in paint.net
		vita2d_draw_texture_part(backgroundTexture, 0, 0, 0, 580, 960, 64);
		vita2d_draw_texture_part(backgroundTexture, 30, 5, 0, 713, 43, 47);
		vita2d_font_draw_textf(font, 600, 20, RGBA8(0,0,0,255), 24, "%s" "%s", (char*)"Bar Tab: ", scoreText.c_str());
		vita2d_font_draw_text(font, 120, 20, RGBA8(0,0,0,255), 24, (char*)"Bartender");
		int itemNumber = 0;
		for(int i = menu2Position; i <= menu2Position + 4; i++)
		{
			if(menu2Selection != i)
			{
				vita2d_draw_texture_part(bartenderUITexture, 10, 68 + (itemNumber * 95), 0, 0, 940, 100);
			}
			else
			{
				vita2d_draw_texture_part(bartenderUITexture, 10, 68 + (itemNumber * 95), 0, 100, 940, 100);
			}
			
			if(i < listOfTrophies.size())
			{
				if(listOfTrophies[i]->isHidden == true && listOfTrophies[i]->unlockKey != "1")
				{
					vita2d_draw_texture_part_scale(bartenderUITexture, 35, 85 + (itemNumber * 95), 0, 224, 32, 32,2,2);
					vita2d_font_draw_text(font, 200, 118 + (itemNumber * 95), RGBA8(255,255,255,255), 24, "Hidden");
					vita2d_font_draw_text(font, 200, 138 + (itemNumber * 95), RGBA8(255,255,255,255), 14, "Hidden");
					
					if(listOfTrophies[i]->trophyType == 1)
					{
						vita2d_draw_texture_part_scale(bartenderUITexture, 170, 106 + (itemNumber * 95), 928, 232, 24, 24,1,1);
					}
					else if(listOfTrophies[i]->trophyType == 2)
					{
						vita2d_draw_texture_part_scale(bartenderUITexture, 170, 106 + (itemNumber * 95), 952, 232, 24, 24,1,1);
					}
					else if(listOfTrophies[i]->trophyType == 3)
					{
						vita2d_draw_texture_part_scale(bartenderUITexture, 170, 106 + (itemNumber * 95), 976, 232, 24, 24,1,1);
					}
					else if(listOfTrophies[i]->trophyType == 4)
					{
						vita2d_draw_texture_part_scale(bartenderUITexture, 170, 106 + (itemNumber * 95), 100, 232, 24, 24,1,1);
					}
				}
				else
				{
					if(listOfTrophies[i]->unlockKey != "1")
					{
						vita2d_draw_texture_part_scale(bartenderUITexture, 35, 85 + (itemNumber * 95), 0, 224, 32, 32,2,2);
					}
					else
					
					{
						vita2d_draw_texture_part_scale(trophiesTexturePack, 40, 90 + (itemNumber * 95), listOfTrophies[i]->x, listOfTrophies[i]->y, 48, 48,1,1);
					}
					vita2d_font_draw_text(font, 200, 118 + (itemNumber * 95), RGBA8(255,255,255,255), 24, listOfTrophies[i]->nameOfTrophy.c_str());
					vita2d_font_draw_text(font, 200, 138 + (itemNumber * 95), RGBA8(255,255,255,255), 14, listOfTrophies[i]->descriptionOfTrophy.c_str());
					if(listOfTrophies[i]->trophyType == 1)
					{
						vita2d_draw_texture_part_scale(bartenderUITexture, 170, 106 + (itemNumber * 95), 928, 232, 24, 24,1,1);
					}
					else if(listOfTrophies[i]->trophyType == 2)
					{
						vita2d_draw_texture_part_scale(bartenderUITexture, 170, 106 + (itemNumber * 95), 952, 232, 24, 24,1,1);
					}
					else if(listOfTrophies[i]->trophyType == 3)
					{
						vita2d_draw_texture_part_scale(bartenderUITexture, 170, 106 + (itemNumber * 95), 976, 232, 24, 24,1,1);
					}
					else if(listOfTrophies[i]->trophyType == 4)
					{
						vita2d_draw_texture_part_scale(bartenderUITexture, 170, 106 + (itemNumber * 95), 1000, 232, 24, 24,1,1);
					}
				}
			}
			
			itemNumber++;
		}
	}
	
	if(currentScreen == 3)
	{
		
		vita2d_draw_texture_part(backgroundTexture, 0, 0, 0, 580, 960, 64);
		vita2d_font_draw_textf(font, 600, 20, RGBA8(0,0,0,255), 24, "%s" "%s", (char*)"Bar Tab: ", scoreText.c_str());
		vita2d_draw_texture_part(backgroundTexture, 30, 5, 0, 713, 43, 47);
		vita2d_font_draw_text(font, 120, 20, RGBA8(0,0,0,255), 24, (char*)"Bartender");
		vita2d_font_draw_text(font, 60, 180, RGBA8(255,255,255,255), 24, "Drink Type:");
		vita2d_font_draw_text(font, 60, 220 , RGBA8(255,255,255,255), 24, "Status:");
		vita2d_font_draw_text(font, 60, 260 , RGBA8(255,255,255,255), 24, "Description:");

		
		vita2d_font_draw_text(font, 56, 529 , RGBA8(255,255,255,255), 16, "Created By: @_island_games,   Icon By: @GideonOnGaming,   UI Art By: @SomeonPC");
		
		if(openTrophy->isHidden == true && openTrophy->unlockKey !="1")
		{
			vita2d_draw_texture_part_scale(bartenderUITexture, 80, 90, 0, 224, 32, 32,1,1);
			vita2d_font_draw_text(font, 150, 110, RGBA8(255,255,255,255), 24, "Hidden");
			std::string trophyLevel = "";
			switch(openTrophy->trophyType)
			{
				case 1:
				trophyLevel = "Rum and Coke";
				break;
				
				case 2:
				trophyLevel = "Mojito";
				break;
				
				case 3:
				trophyLevel = "Piña colada";
				break;
				
				case 4:
				trophyLevel = "Vita Island Ice Tea";
				break;
			}
			
			std::string isEarned = "";
			
			if(openTrophy->unlockKey == "0" )
			{
				isEarned = "Locked";
			}
			else if(openTrophy->unlockKey == "1")
			{
				isEarned = "Unlocked";
			}
			else
			{
				isEarned = "Pending";
			}
			
			vita2d_font_draw_text(font, 350, 180, RGBA8(255,255,255,255), 24, trophyLevel.c_str());
			vita2d_font_draw_text(font, 350, 220 , RGBA8(255,255,255,255), 24, isEarned.c_str());
			vita2d_font_draw_text(font, 350, 260 , RGBA8(255,255,255,255), 24, "Hidden");
			if(openTrophy->trophyType == 1)
			{
				vita2d_draw_texture_part_scale(bartenderUITexture, 315, 156, 928, 232, 24, 24,1,1);
			}
			else if(openTrophy->trophyType == 2)
			{
				vita2d_draw_texture_part_scale(bartenderUITexture, 315, 156, 952, 232, 24, 24,1,1);
			}
			else if(openTrophy->trophyType == 3)
			{
				vita2d_draw_texture_part_scale(bartenderUITexture, 315, 156, 976, 232, 24, 24,1,1);
			}
			else if(openTrophy->trophyType == 4)
			{
				vita2d_draw_texture_part_scale(bartenderUITexture, 315, 156, 1000, 232, 24, 24,1,1);
			}
		}
		else
		{
			if(openTrophy->unlockKey != "1")
			{
				vita2d_draw_texture_part_scale(bartenderUITexture, 80, 90, 0, 224, 32, 32,1,1);
			}
			else
			{
				vita2d_draw_texture_part(trophiesTexturePack, 80, 90, openTrophy->x, openTrophy->y, 48, 48);
			}
			
			vita2d_font_draw_text(font, 150, 110, RGBA8(255,255,255,255), 24, openTrophy->nameOfTrophy.c_str());
			std::string trophyLevel = "";
			switch(openTrophy->trophyType)
			{
				case 1:
				trophyLevel = "Rum and Coke";
				break;
				
				case 2:
				trophyLevel = "Mojito";
				break;
				
				case 3:
				trophyLevel = "Piña colada";
				break;
				
				case 4:
				trophyLevel = "Vita Island Ice Tea";
				break;
			}
			
			std::string isEarned = "";
			
			if(openTrophy->unlockKey == "0" )
			{
				isEarned = "Locked";
			}
			else if(openTrophy->unlockKey == "1")
			{
				isEarned = "Unlocked";
			}
			else
			{
				isEarned = std::to_string(openTrophy->unlockKey.size());
			}
			
			vita2d_font_draw_text(font, 350, 180, RGBA8(255,255,255,255), 24, trophyLevel.c_str());
			vita2d_font_draw_text(font, 350, 220 , RGBA8(255,255,255,255), 24, isEarned.c_str());
			vita2d_font_draw_text(font, 350, 260 , RGBA8(255,255,255,255), 24, openTrophy->descriptionOfTrophy.c_str());
			if(openTrophy->trophyType == 1)
			{
				vita2d_draw_texture_part_scale(bartenderUITexture, 315, 156, 928, 232, 24, 24,1,1);
			}
			else if(openTrophy->trophyType == 2)
			{
				vita2d_draw_texture_part_scale(bartenderUITexture, 315, 156, 952, 232, 24, 24,1,1);
			}
			else if(openTrophy->trophyType == 3)
			{
				vita2d_draw_texture_part_scale(bartenderUITexture, 315, 156, 976, 232, 24, 24,1,1);
			}
			else if(openTrophy->trophyType == 4)
			{
				vita2d_draw_texture_part_scale(bartenderUITexture, 315, 156, 1000, 232, 24, 24,1,1);
			}
		}
		
	}
	
	DrawConsole();
	DrawPopUp();
	DrawSyncPopup();
	
	keyboard->DrawKeyboard();
	vita2d_end_drawing();
	vita2d_swap_buffers();
	
}



void CheckTrophiesStillInUse()
{
	// Checks which trophy images are currently in use and remove those that are not.
	for(int i = 0; i < trophyPackImageList.size(); i++)
	{
		if(trophyPackImageList[i].texturePosition < menu1Position || trophyPackImageList[i].texturePosition > menu1Position + 4)
		{
			trophyPackImageList.erase(trophyPackImageList.begin() + i);
		}
	}
	
}

void CheckConsoleDisplay()
{
	
}


void Login()
{
	// Logs into the server
	if(loadedUserName.size() > 0 && loadedPassword.size() > 0)
	{
		std::string textToPost = "";
		
		const char* textToPostChar = textToPost.c_str();
		const char* downloadURL = "";
		//Check the login
	int tpl = sceHttpCreateTemplate("Login", 1, 1);
	int conn = sceHttpCreateConnectionWithURL(tpl, downloadURL, 0);
	int request = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_POST, downloadURL, strlen(textToPostChar));
	int header = sceHttpAddRequestHeader(request, "Content-Type", "application/x-www-form-urlencoded", SCE_HTTP_HEADER_OVERWRITE);
	int handle = sceHttpSendRequest(request, textToPostChar, strlen(textToPostChar));
	//int fh = sceIoOpen(tempFile, SCE_O_WRONLY | SCE_O_CREAT, 0777);
	std::string readData = "";
	char data[16*1024];
	long read = 0;

	// read data until finished
		while ((read = sceHttpReadData(request, &data, sizeof(data))) > 0) {
			
			// writing the count of read bytes from the data buffer to the file
			data[read] = '\0';
				readData.append(data);
			

			
		}

	
	
	//downloadedFile.append(reread);
		if(readData == "Success")
		{
			isLoggedIn = true;
		}
		else if(readData == "Fail")
		{
			isLoggedIn = false;
			GeneratePopupMessage("Login Credentials are Incorrect");
		}
				
	}
}

void SaveSettingsFile()
{
	// Saves the settings file
	std::string fileToSave = "";
	fileToSave.append(loadedUserName);
	fileToSave.append("|");
	fileToSave.append(loadedPassword);
	const char* fileToSaveChar = fileToSave.c_str();
	consoleText.append(fileToSave);
	SceUID settingsFile= sceIoOpen("ux0:data/Bartender/settings.ini", SCE_O_WRONLY|SCE_O_CREAT|SCE_O_TRUNC, 0777);
	if(settingsFile > 0) 
	{
		int written = sceIoWrite(settingsFile, fileToSaveChar, fileToSave.size());
		int closing = sceIoClose(settingsFile);
	}
	else
	{
		GeneratePopupMessage("Couldn't save settings file.");
	}
	
	
}

void CheckLogin()
{
	// Checks if the login was succesful upon logging in
	if(isLoggedIn == true)
	{
		currentScreen = 1;
		SaveSettingsFile();
	}
	else
	{
		currentScreen = 0;
	}
}


void SignUpSubmission()
{
	// New user signup submission
	if(signUpEmail.find("@") != std::string::npos)
	{
		
		
	}
	else
	{
		GeneratePopupMessage("Invalid email address");
		return;
	}
	
	if(signUpName.size() < 3)
	{
		GeneratePopupMessage("User names must be at least 3 characters.");
		return;
	}
	
	if(signUpName.rfind("!") != std::string::npos ||
	signUpName.rfind("@") != std::string::npos ||
	signUpName.rfind("#") != std::string::npos ||
	signUpName.rfind("$") != std::string::npos ||
	signUpName.rfind("^") != std::string::npos ||
	signUpName.rfind("&") != std::string::npos ||
	signUpName.rfind("*") != std::string::npos ||
	signUpName.rfind("(") != std::string::npos ||
	signUpName.rfind(")") != std::string::npos ||
	signUpName.rfind("{") != std::string::npos ||
	signUpName.rfind(":") != std::string::npos ||
	signUpName.rfind("\"") != std::string::npos ||
	signUpName.rfind("<") != std::string::npos ||
	signUpName.rfind(">") != std::string::npos ||
	signUpName.rfind("?") != std::string::npos ||
	signUpName.rfind("+") != std::string::npos ||
	signUpName.rfind("[") != std::string::npos ||
	signUpName.rfind(";") != std::string::npos ||
	signUpName.rfind("'") != std::string::npos ||
	signUpName.rfind(",") != std::string::npos ||
	signUpName.rfind(".") != std::string::npos ||
	signUpName.rfind("/") != std::string::npos ||
	signUpName.rfind("=") != std::string::npos)
	{
		GeneratePopupMessage("UserName can only contain special\n characters - or _");
		return;
	}
	
	if(signupPassword.size() < 6)
	{
		GeneratePopupMessage("Passwords must be at least 6 characters.");
		return;
	}
	
		if(signUpEmail.size() > 0 && signupPassword.size() > 0 && signUpName.size() > 0)
	{
		std::string textToPost ="";

		const char* textToPostChar = textToPost.c_str();
		const char* downloadURL = "";
		//Check the login
	int tpl = sceHttpCreateTemplate("Signup", 1, 1);
	int conn = sceHttpCreateConnectionWithURL(tpl, downloadURL, 0);
	int request = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_POST, downloadURL, strlen(textToPostChar));
	int header = sceHttpAddRequestHeader(request, "Content-Type", "application/x-www-form-urlencoded", SCE_HTTP_HEADER_OVERWRITE);
	int handle = sceHttpSendRequest(request, textToPostChar, strlen(textToPostChar));
	//int fh = sceIoOpen(tempFile, SCE_O_WRONLY | SCE_O_CREAT, 0777);
	std::string readData = "";
	char data[16*1024];
	long read = 0;

	// read data until finished
		while ((read = sceHttpReadData(request, &data, sizeof(data))) > 0) {
			
			// writing the count of read bytes from the data buffer to the file
			data[read] = '\0';
				readData.append(data);
			
		}
	
	
		if(readData == "Success")
		{
			//Display account created. Return to previous login page.
			signUpScreenOpen = false;
			menu0Position = 0;
			GeneratePopupMessage("User account created succesfully.");
		}
		else
		{
			//Display a pop-up with the error message
			GeneratePopupMessage(readData);
		}
				
	}
}

void LoginScreen()
{
	// Controls the login screen.
	if(keyboard->isActive == false)
	{
		if(currentScreen == 0 && passwordResetScreen == false && signUpScreenOpen == false)
		{
			if(controller->canCrossBePressed == true && controller->isCrossPressed == true)
			{
				switch(menu0Position)
				{
					case 0:
					// Username
					keyboard->isActive = true;
					keyboard->SetFieldToEdit(&loadedUserName);
					keyboard->fieldName= "User Name:";
					controller->GetCanPresses();
					
					break;
					
					case 1:
					// Password
					keyboard->isActive = true;
					keyboard->SetFieldToEdit(&loadedPassword);
					keyboard->fieldName= "Password:";
					controller->GetCanPresses();
					
					break;
					
					case 2:
					// Login Button
					Login();
					CheckLogin();
					
					if(isLoggedIn == true)
					{
						GetListOfPossibleTrophyPacks();
						LoadInitialListOfTrophyPackImages();
						SyncTrophyPacks();
						GetScore();
					}
					controller->GetCanPresses();
					
					break;
					
					case 3:
					// Sign Up Button
					signUpScreenOpen = true;
					menu0Position = 0;
					controller->GetCanPresses();
					
					break;
					
					case 4:
					// Password Reset
					passwordResetScreen = true;
					menu0Position = 0;
					controller->GetCanPresses();
					
					break;
					
				}
			}
		}
		
	}
}

void PasswordResetRequestSubmission()
{
	// Submits the password reset request.
	if(passwordResetEmail.find("@") != std::string::npos)
	{
		
		
	}
	else
	{
		GeneratePopupMessage("Invalid email address");
		return;
	}
	
	if(passwordResetEmail.size() > 0)
	{
		std::string textToPost ="";
		

		const char* textToPostChar = textToPost.c_str();
		const char* downloadURL = "";
		//Check the login
	int tpl = sceHttpCreateTemplate("Password Reset", 1, 1);
	int conn = sceHttpCreateConnectionWithURL(tpl, downloadURL, 0);
	int request = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_POST, downloadURL, strlen(textToPostChar));
	int header = sceHttpAddRequestHeader(request, "Content-Type", "application/x-www-form-urlencoded", SCE_HTTP_HEADER_OVERWRITE);
	int handle = sceHttpSendRequest(request, textToPostChar, strlen(textToPostChar));
	//int fh = sceIoOpen(tempFile, SCE_O_WRONLY | SCE_O_CREAT, 0777);
	std::string readData = "";
	char data[16*1024];
	long read = 0;

	// read data until finished
		while ((read = sceHttpReadData(request, &data, sizeof(data))) > 0) {
			
			// writing the count of read bytes from the data buffer to the file
			data[read] = '\0';
				readData.append(data);
			
		}
	
	
		if(readData == "Email has been sent")
		{
			//Display account created. Return to previous login page.
			passwordResetScreen = false;
			menu0Position = 0;
			GeneratePopupMessage(readData);
		}
		else
		{
			//Display a pop-up with the error message
			GeneratePopupMessage(readData);
		}		
	}
}


void PasswordResetScreen()
{
	// Controls the password reset screen.
	
	if(keyboard->isActive == false)
	{
		if(currentScreen == 0)
		{
			if(passwordResetScreen == true)
			{
				if(controller->canCrossBePressed == true && controller->isCrossPressed == true)
				{
					switch(menu0Position)
					{
						case 0: 
						// Email
						keyboard->isActive = true;
						keyboard->SetFieldToEdit(&passwordResetEmail);
						keyboard->fieldName= "Email Address:";
						controller->GetCanPresses();
						
						break;
						
						case 1:
						PasswordResetRequestSubmission();
						controller->GetCanPresses();
						break;
						
					}
				}
				
				if(controller->canCircleBePressed == true && controller->isCirclePressed == true)
				{
					passwordResetScreen = false;
					menu0Position = 0;
					passwordResetEmail = "";
					return;
				}
			}
			
		}
		
	}
}

void SignUpScreen()
{
	// Controls the sign up screen
	if(keyboard->isActive == false)
	{
		if(currentScreen == 0)
		{
			if(signUpScreenOpen == true)
			{
				if(controller->canCrossBePressed == true && controller->isCrossPressed == true)
				{
					switch(menu0Position)
					{
						case 0: 
						// Email
						keyboard->isActive = true;
						keyboard->SetFieldToEdit(&signUpEmail);
						keyboard->fieldName= "Email Address:";
						controller->GetCanPresses();
						
						break;
						
						case 1:
						// User Name
						keyboard->isActive = true;
						keyboard->SetFieldToEdit(&signUpName);
						keyboard->fieldName= "User Name:";
						controller->GetCanPresses();
						break;
						
						
						case 2:
						// Password
						keyboard->isActive = true;
						keyboard->SetFieldToEdit(&signupPassword);
						keyboard->fieldName= "Password:";
						controller->GetCanPresses();
						break;
						
						
						case 3:
						// Submit
						SignUpSubmission();
						controller->GetCanPresses();
						break;
						
						
					}
				}
				
				
				if(controller->canCircleBePressed == true && controller->isCirclePressed == true)
				{
					signUpScreenOpen = false;
					menu0Position = 0;
					return;
				}
			}
		}
	}
}

void ResyncTrophies()
{
	if(controller->canTriangleBePressed == true && controller->isTrianglePressed == true)
	{
		if(currentScreen == 1)
		{
			isResyncingTrophies = true;
		}
	}
}


void LogoutPopup()
{
	if(isLogOutScreenOpen == true)
	{
		if(controller->isLeftPressed == true && controller->canLeftBePressed == true)
		{
			logOutSelection--;
			if(logOutSelection < 0)
			{
				logOutSelection = 0;
			}
		}
		
		if(controller->isRightPressed == true && controller->canRightBePressed == true)
		{
			logOutSelection++;
			if(logOutSelection > 1)
			{
				logOutSelection =1;
			}
		}
		
		if(controller->isCrossPressed == true && controller->canCrossBePressed == true)
		{
			if(logOutSelection == 0)
			{
				Logout();
			}
			
			if(logOutSelection == 1)
			{
				isLogOutScreenOpen = false;
			}
		}
	}
	
}



void Update()
{
	while (true)
	{
		controller->CheckInputs();
		
		if(isResyncingTrophies == true)
		{
			
			DownloadTrophiesFromServer();
		}
		
		ResyncTrophies();
		
		if(isResyncingTrophies == false)
		{
			ScrollUp();
			ScrollDown();
			CheckTrophiesStillInUse();
			OpenTrophyPack();
			OpenTropy();	
			ReturnToPreviousScreen();
			SignUpScreen();
			LoginScreen();
			PasswordResetScreen();
			keyboard->KeyboardUpdate();
			CheckConsoleDisplay();
			LogoutPopup();
			
		}
		controller->GetCanPresses();
		Draw();
	}
}

void CheckSignInCredentials()
{
	// Check if the bartender directory exist. If it does not, it will create it.
	if(sceIoDopen("ux0:data/Bartender") < 0)
	{		
		sceIoMkdir("ux0:data/Bartender",0777); 
	}
	// Check if the settings file exist
	std::string settingDirectory = "ux0:Data/Bartender/settings.ini";
	SceUID settingsFile = sceIoOpen(settingDirectory.c_str(),SCE_O_RDONLY, 0777);
	if(settingsFile >=0)
	{
		// Read the file
		long fileSize = sceIoLseek(settingsFile,0,SCE_SEEK_END);
		long tempFileSize= sceIoLseek(settingsFile,0,SCE_SEEK_SET);
		char fileToRead[fileSize+1];
		
		
		long fileReadBytes = sceIoRead(settingsFile,fileToRead,fileSize);
		
		fileToRead[fileSize] = '\0';
		std::string tempFileString = fileToRead;
		
				
		// Parse file
		int itemNumber = 0;
		std::string stringBuilding = "";
		sceIoClose(settingsFile);
		for(int i = 0; i < tempFileString.size(); i++)
		{
			if(tempFileString.substr(i,1) == "|" || i == tempFileString.size()-1)
			{
				if(i == tempFileString.size()-1)
				{
					if(tempFileString.substr(i,1) != "\n")
					{
						stringBuilding.append(tempFileString.substr(i,1));
					}
				}
				if(itemNumber == 0)
				{
					loadedUserName = stringBuilding;
				}
				else if(itemNumber == 1)
				{
					loadedPassword = stringBuilding;
				}
				
				itemNumber++;
				stringBuilding = "";
			}				
			else
			{
				stringBuilding.append(tempFileString.substr(i,1));
			}
		}
		
	}
	else
	{
		int fh = sceIoOpen(settingDirectory.c_str(), SCE_O_WRONLY | SCE_O_CREAT, 0777);
		sceIoClose(fh);
	}
}


int main (int argc, char *argv[])
{
	
	vita2d_init();
	bartenderUITexture = vita2d_load_PNG_file(imageFileDirectory);
	backgroundTexture = vita2d_load_PNG_file(backgroundImageFileDirectory);
	font = vita2d_load_font_file(fontFileDirectory);
	keyboardFont = vita2d_load_font_file((char*)"app0:img/Attari_Salees.ttf");
	keyboard = new Keyboard(controller,keyboardFont);
	EstablishNetwork();
	CheckSignInCredentials();
	Login();
	CheckLogin();
	
	DownloadTrophyPack();
	if(avilableTrophyPacks.size() <= 0)
	{
		OpenZipPackWithListOfGames();
	}
	
	if(isLoggedIn == true)
	{
		GetListOfPossibleTrophyPacks();
		LoadInitialListOfTrophyPackImages();
		SyncTrophyPacks();
		GetScore();
	}

	consoleText.append(std::to_string(listOfTrophyPacks.size()));
	Update();
}