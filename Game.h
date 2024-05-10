#ifndef GAME_H_
#define GAME_H_

#include "Word.h"
#include <vector>

class Game {
private:

	Word w;
	char* wordChars;
	
	//promenne ciste pro hadani pocitacem
	int mistakes=0,wins=0,mistakeSum=0;
	std::vector<int> mistakeLog;
	std::vector<std::string> winWordLog;
	std::string alphabet = "";
	std::string charSeries = "";

	//metody vykreslovani prvku hry=
	void renderHangman(int mistakes);
	void renderBlanks();
	void renderGameOver();
	void renderWin();
	void renderLogo();
	//==============================


	//metody pro hadani pocitacem===
	void simulate(int approachID);					//metoda podle approachID zavola prislusnou metodu z nize uvedenych
	void simulateRandom();							//hada nahodne znaky BEZ opakovani
	void simulateBogo();							//hada nahodne znaky s moznosti opakovani
	void simulateMostCommonSeries();				//hada znaky v poradi urcenem serii nejvice pouzivanych znaku v anglicke abecede
	void simulateUserInputSeries();					//hada znaky v poradi urcenem hracem, je osetreno proti nekonecnemu loopu tim, ze hrac musi zadat vsechny znaky abecedy
	//==============================
	
	//Pomocne metody================
	void tryChar(char randomChar);					//zavola metodu guess(randomChar) tridy Word a pokud je ve znak ve slovu, doplni ho do jiz nalezenych znaku 
	void resetVarsBetweenWords(std::string line);	//vola se mezi kazdym novym slovem, ktere pocitac hada
	void resetVarsBetweenGames();					//vola se po uhodnuti vsech slov pocitacem
	void logResults();								//zaloguje vysledky po kazdem slovu a vytiskne relevantni info
	void printResults();							//vytiskne souhrnna data tykajici se vsech slov
	void printExitPromt();							//vyzve uzivatele k ukonceni / navratu do menu
	
	char generateChar(std::string alphabet, int delay);		//nahodne vybere znak z parametru, delay urcuje jak dlouho bude system cekat mezi generovanim - pro zamezeni generovani stejnych znaku
	bool completed();								//kontrola, jestli uz byly nalezeny vsechny znaky ve slove
	bool isIntInput(std::string input,int options); //kontrola inputu
	bool isValidSeries(std::string input);			//kontrola uzivatelem zadane serie znaku
	//==============================

	void playerGameRun();							//zacne hru hrace - cela hra probiha v teto funkci v loopu

public:
	
	//CONSTRUCTORY
	Game();
	Game(Word w);

	void intro();									//metoda zacinajici celou hru
};

#endif