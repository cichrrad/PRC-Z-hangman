#include "Game.h"
#include "color.h"
#include <chrono>
#include <random>
#include <thread>
#include <iomanip>
#include <iostream>

//CONSTRUCTORY=================================================
Game::Game() {
	
    this->w = Word();
    this->wordChars = new char[w.getLength()];
	mistakes = 0;
}
Game::Game(Word w) {
	
    this->w = w;
    this->wordChars = new char[w.getLength()];
	mistakes = 0;
}
//=============================================================

//HLAVNI METODA================================================
void Game::intro()
{
     srand(std::chrono::steady_clock::now().time_since_epoch().count());
    //UVODNI MENU
    while (true) {
        
        std::string input;
        //ZADANI PRVNIHO INPUTU
        do {
            input = "";
            system("cls");
            renderLogo();
            std::cout << "\n|ACTIONS|" << dye::aqua("=========================================================================================\n");
            std::cout << "[1] PLAY - RANDOM WORD\n";
            std::cout << "[2] SIMULATE \n";
            std::cout << "[3] VIEW WORD LIST DETAILS\n";
            std::cout << dye::aqua("==================================================================================================\n");
            std::cout << "YOUR INPUT: "; std::cin >> input;
        } while (!isIntInput(input, 3));
        
        std::string input1;
        //VYHODNOCENI PRVNIHO INPUTU
        switch (stoi(input)) {
        //HADANI HRACEM
        case 1:
            this->mistakes = 0;
            for (int i = 0; i < w.getLength(); i++) { wordChars[i] = NULL; }
            this->w = Word();
            playerGameRun();
            break;
        //HADANI POCITACEM
        case 2:
            do {
                system("cls");
                std::cout << "\n|SIMULATE|" << dye::aqua("========================================================================================\n");
                std::cout << "[1] RANDOM LETTER SERIES NO REPEATS\n";
                std::cout << "[2] RANDOM LETTER SERIES WITH REPEATING(SLOW!)\n";
                std::cout << "[3] MOST COMMON LETTERS FIRST SERIES\n"; \
                std::cout << "[4] CUSTOM LETTER SERIES\n";
                std::cout << "[5] GO BACK\n";
                std::cout << dye::aqua("==================================================================================================\n");
                std::cout << "YOUR INPUT: "; std::cin >> input1;
            } while (!isIntInput(input1, 5));
            //VYHODNOCENI INPUTU SUBMENU HADANI POCITACEM
            if (stoi(input1) <= 4) {
                simulate(stoi(input1));
            }
            //IF INPUT1 == 5 -> NIC 
            break;
        //ZOBRAZENI INFORMACI O TEXTOVEM SOUBORU SE SLOVY
        case 3:
            w.getWordLoader().printReport();
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            break;
        //NEPLATNY INPUT - NEMEL BY NIKDY NASTAT, NEBOT VSECHNY INPUTY JSOU OSETRENY DO{}WHILE(KONTROLA) LOOPEM
        default:
            system("cls");
            std::cout << "Wrong Input...";
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
    
    }
}
//=============================================================

//METODY PRO HADANI POCITACEM==================================

//METODA, KTERA PODLE INPUTU VOLA DANE STRATEGIE HADANI
void Game::simulate(int approachID)
{
    system("cls");
    switch (approachID) {
            case 1:
                system("cls");
                simulateRandom();
                break;
            case 2:
                system("cls");
                simulateBogo();
                break;
            case 3:
                system("cls");
                simulateMostCommonSeries();
                break;
            case 4:
                system("cls");
                simulateUserInputSeries();
                break;
            default:
                std::cout << "Wrong Input...";
                std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
    
}

//NAHODNA SERIE ZNAKU, BEZ OPAKOVANI
void Game::simulateRandom() {
    
    //Word wrd = Word();
    //this->w = wrd;
    
    std::ifstream myfile;
    myfile.open("hangmanVocab.txt");
    
    std::string line;
    
    while (getline(myfile, line)) {
       
        resetVarsBetweenWords(line);

        do {

            char randomChar = generateChar(alphabet,0);
            alphabet.erase(alphabet.find(randomChar), 1);

            tryChar(randomChar);

        } while (!completed());
        
        logResults();
    };
    
    printResults();
    resetVarsBetweenGames();
    printExitPromt();
    

}

//NAHODNA SERIE ZNAKU, OPAKOVANI
void Game::simulateBogo()
{
    //Word wrd = Word();
    //this->w = wrd;

    std::ifstream myfile;
    myfile.open("hangmanVocab.txt");
    
    std::string line;

    while (getline(myfile, line)) {

        resetVarsBetweenWords(line);

        do {
            
            char randomChar = generateChar(alphabet,1); 
          
            tryChar(randomChar);

        } while (!completed());
        
        logResults();
    };

    printResults();
    resetVarsBetweenGames();
    printExitPromt();

}

//SERIE NEJPOUZIVANEJSICH PISMEN ANGLICKE ABECEDY
void Game::simulateMostCommonSeries()
{
    std::string mostCommonSeries = "eariotnslcudpmhgbfywkvxzjq";

    //Word wrd = Word();
    ///this->w = wrd;

    std::ifstream myfile;
    myfile.open("hangmanVocab.txt");
   
    std::string line;
    
    while (getline(myfile, line)) {

        resetVarsBetweenWords(line);
        int currL = 0;

        do {
           
            tryChar(mostCommonSeries[currL]);
            charSeries += mostCommonSeries[currL];
            currL++;

        } while (!completed());
        
        logResults();
    };
   
    printResults();
    resetVarsBetweenGames();
    printExitPromt();
    
}

//SERIE ZADANA UZIVATELEM
//MUSI OBSAHOVAT VSECHNA MALA PISMENA ABECEDY
void Game::simulateUserInputSeries(){

    std::string userSeriesInput;
    do {
        system("cls");
        std::cout << "\n|CUSTOM LETTER SERIES|" << dye::aqua("============================================================================\n");
        std::cout << "ENTER A LETTER SERIES YOU WANT THE COMPUTER TO TRY ON ALL THE WORDS\n";
        std::cout << dye::red("INPUT RULES:\n");
        std::cout << " - INPUT MUST CONTAIN ALL 26 LOWERCASE LETTERS OF ENGLISH ALPHABET\n";
        std::cout << dye::aqua("==================================================================================================\n");
        std::cout << "YOUR INPUT: "; std::cin >> userSeriesInput;
    } while (!isValidSeries(userSeriesInput));

    //Word wrd = Word();
    //this->w = wrd;

    std::ifstream myfile;
    myfile.open("hangmanVocab.txt");
   
    std::string line;
    
    while (getline(myfile, line)) {

        resetVarsBetweenWords(line);
        int currL = 0;

        do {
           
            tryChar(userSeriesInput[currL]);
            charSeries += userSeriesInput[currL];
            currL++;

        } while (!completed());
        
        logResults();
    };
   
    printResults();
    resetVarsBetweenGames();

    printExitPromt();


}
//=============================================================

//POMOCNE METODY PRO ZPRACOVANI DAT Z HADANI POCITAEM==========
void Game::tryChar(char randomChar){
    if (w.guess(randomChar)) {
        for (int i = 0; i < w.getLength(); i++) { 
            if (w.getText()[i] == randomChar) { 
                wordChars[i] = randomChar; } 
        }   
    }
    else {
        mistakes++;
    }
}

void Game::logResults(){
    
    mistakeLog.push_back(mistakes);
    if (mistakes < 6) {
        wins++;
        winWordLog.push_back(w.getText());
    }
    if (mistakeLog[mistakeLog.size() - 1] < 6) {
        std::cout << dye::green(w.getText()) << std::setw(w.getWordLoader().getMaxLength() - w.getLength()) << "" << dye::green(" |GUESS SERIES: ") << dye::green(charSeries) << std::setw(27 - charSeries.length()) << "" << dye::green(" |MISTAKES: ") << dye::green(mistakeLog[mistakeLog.size() - 1]) << std::endl;
    }
    else {
        std::cout << dye::red(w.getText()) << std::setw(w.getWordLoader().getMaxLength() - w.getLength()) << "" << dye::red(" |GUESS SERIES: ") << dye::red(charSeries) << std::setw(27 - charSeries.length()) << "" << dye::red(" |MISTAKES: ") << dye::red(mistakeLog[mistakeLog.size() - 1]) << std::endl;

    }
    mistakeSum += mistakes;
}

void Game::resetVarsBetweenWords(std::string line){
    
    w = Word(line);
    wordChars = NULL;
    wordChars = new char[w.getLength()];
    mistakes = 0;
    alphabet = "abcdefghijklmnopqrstuvwxyz";
    charSeries = "";
}

void Game::resetVarsBetweenGames(){
    
    wins = 0;
    mistakeSum = 0;
    mistakeLog.clear();
    mistakeLog.shrink_to_fit();
    winWordLog.clear();
    winWordLog.shrink_to_fit();
}

char Game::generateChar(std::string alphabet, int delay){
    
    char randomChar = alphabet[(rand() % alphabet.size())];
    charSeries += randomChar;
    return randomChar;

}

void Game::printResults()
{
    
    int minMistakes = INT_MAX;
    int maxMistakes = INT_MIN;
    for (int i = 0; i < mistakeLog.size(); i++) {
        if (mistakeLog[i] < minMistakes) {
            minMistakes = mistakeLog[i];
        }
        if (maxMistakes < mistakeLog[i]) {
            maxMistakes = mistakeLog[i];
        }
    }
    

    std::cout << "\n|RESULTS|" << dye::aqua("=======================================================================================\n");
    std::cout << dye::aqua("OVERALL WINS(LESS THAN 6 MISTAKES) : ") << wins << " = " << ((double)wins / ((double)mistakeLog.size())) * 100 << " % \n\n";
    for (int i = 0; i < winWordLog.size(); i++) { std::cout << "[" << winWordLog[i] << "] \n"; }
    std::cout << dye::aqua("\nMistakes on average: ") << ((double)mistakeSum / (double)(mistakeLog.size())) << std::endl;
    std::cout << dye::aqua("Lowest number of mistakes: ") << minMistakes << std::endl;
    std::cout << dye::aqua("Highest number of mistakes: ") << maxMistakes << std::endl;
    std::cout << dye::aqua("==================================================================================================\n");

}

void Game::printExitPromt()
{
    std::string input;
    do {
        input = "";
        std::cout << "\n|Do you want to|" << dye::aqua("================================================\n");
        std::cout << "\n[1] Exit program\n";
        std::cout << "\n[2] Exit to the menu\n";
        std::cout << dye::aqua("================================================================\n");
        std::cout << "YOUR INPUT: "; std::cin >> input;
        system("cls");
    } while (!isIntInput(input, 2));
    if (stoi(input) == 1) { exit(0); }
}
//=============================================================

//METODA PRO HADANI HRACEM=====================================
void Game::playerGameRun() {
   
//HLAVNI GAME LOOP

    //POKUD UZ JSEM UDELAL 6 CHYB = KONEC
    if (mistakes == 6) {
        //renderHangman(mistakes);
        //renderBlanks();
        renderGameOver();
    }
    //POKUD MAM < 6 CHYB, VYKRESLIM SIBENICI S DANYM POCTEM CHYB
    //VYKRESLIM SLOVO SE ZNAKY KTERE UZ ZNAM(POKUD NEJAKE ZNAM)
    //ZADAM ZNAK / SLOVO
    else {
        renderHangman(mistakes);
        renderBlanks();
        std::string input;
        std::cout << "\nYOUR GUESS: ";
        std::cin >> input;
        //POKUD JE DELKA 1 = ZADAL JSEM ZNAK
        if (input.size() == 1) { 
            char inputC = input[0]; 
            //TREFIL JSEM SE ?
            if (w.guess(inputC)) {
                //DOPLNIM DO OBJEVENYCH ZNAKU VSECHNY INSTANCE ZNAKU
                for (int i = 0; i < w.getLength(); i++) { if (w.getText()[i] == inputC) { wordChars[i] = inputC; } }
                //POKUD JSEM TIMTO UHODNUTIM DOPLNIL VSECHNA ZBYVAJICI PISMENA SLOVA = VYHRA
                if (completed()) {
                    renderWin(); 
                }
                else {
                    playerGameRun();
                }
            }
            //POKUD JSEM SE NETREFIL :[
            else {
                mistakes++;
                playerGameRun();
            }
        }
        //POKUD INPUT JE JINE DELKY NEZ 1 = BERU HO JAKO HADANI CELEHO SLOVA
        else {
            if (w.guess(input)) {
                renderWin();
            }
            else {
                mistakes++;
                playerGameRun();
            }
        }
    }
}
//=============================================================

bool Game::completed()
{
    for (int i = 0; i < w.getLength(); i++) {
        if (wordChars[i] != w.getText()[i]) { return false; }
    }
    return true;
}

bool Game::isIntInput(std::string input,int options)
{
    if (input.length() == 0) { return false; }
    for (int i = 0; i < input.length(); i++) {
        if (!isdigit(input[i])) { return false; }
    }
    
    if (stoi(input) <= 0) { return false; }
    if (stoi(input) > options) { return false; }
    return true;
}

bool Game::isValidSeries(std::string input)
{
    if (input.length() < 26) { return false; }
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < alphabet.size(); i++) {
        if (input.find(alphabet[i]) == std::string::npos) { return false; }
    }
    return true;
}

//METODY VYKRESLOVANI PRVKU HRY================================
void Game::renderHangman(int mistakes) {
	if (mistakes == 0) {
        system("cls");
		const char* hangman0 =
			"   _                          _   \n"
			" _| |_ ______ ______ ______ _| |_ \n"
		    "|_   _|______|______|______|_   _|\n"
			"  |_|                        |_|  \n"
			"  | |                        | |  \n"
			"  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  |_|                        |_|  \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  |_|                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  |_|                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            " | _ |___ ________ ________ ________ ________ ________ ________ \n"
            "|________|________|________|________|________|________|________|\n \n"
            
            "MISTAKES: 0 OUT OF 6\n"
			;

		std::cout << hangman0;
	}
	else if (mistakes == 1) {
        system("cls");
        const char* hangman1 =
            "   _                          _   \n"
            " _| |_ ______ ______ ______ _| |_ \n"
            "|_   _|______|______|______|_   _|\n"
            "  |_|                        |_|  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  |_|                       _\\_/_  \n"
            "  | |                      /  _  \\ \n"
            "  | |                     |  | |  |\n"
            "  | |                     |  | |  |\n"
            "  | |                     |  |_|  |\n"
            "  | |                      \\_____/  \n"
            "  | |                             \n"
            "  |_|                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  |_|                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            " | _ |___ ________ ________ ________ ________ ________ ________ \n"
            "|________|________|________|________|________|________|________|\n \n"

            "MISTAKES: 1 OUT OF 6\n"
            ;
        std::cout << hangman1;
	}
	else if (mistakes == 2) {
        system("cls");
        const char* hangman2 =
            "   _                          _   \n"
            " _| |_ ______ ______ ______ _| |_ \n"
            "|_   _|______|______|______|_   _|\n"
            "  |_|                        |_|  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  |_|                       _\\_/_  \n"
            "  | |                      /  _  \\ \n"
            "  | |                     |  | |  |\n"
            "  | |                     |  | |  |\n"
            "  | |                     |  |_|  |\n"
            "  | |                      \\_____/  \n"
            "  | |                        | |    \n"
            "  |_|                        | |     \n"
            "  | |                        | |     \n"
            "  | |                        |_|     \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  |_|                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            " | _ |___ ________ ________ ________ ________ ________ ________ \n"
            "|________|________|________|________|________|________|________|\n \n"

            "MISTAKES: 2 OUT OF 6\n"
            ;
        std::cout << hangman2;
	}
    else if (mistakes == 3) {
        system("cls");
        const char* hangman3 =
            "   _                          _   \n"
            " _| |_ ______ ______ ______ _| |_ \n"
            "|_   _|______|______|______|_   _|\n"
            "  |_|                        |_|  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  |_|                       _\\_/_  \n"
            "  | |                      /  _  \\ \n"
            "  | |                     |  | |  |\n"
            "  | |                     |  | |  |\n"
            "  | |                     |  |_|  |\n"
            "  | |                      \\_____/  \n"
            "  | |                     ===| |    \n"
            "  |_|                    / / | |     \n"
            "  | |                   / /  | |     \n"
            "  | |                  /_/   |_|     \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  |_|                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            " | _ |___ ________ ________ ________ ________ ________ ________ \n"
            "|________|________|________|________|________|________|________|\n \n"

            "MISTAKES: 3 OUT OF 6\n"
            ;
        std::cout << hangman3;
}
    else if (mistakes == 4) {
        system("cls");
        const char* hangman4 =
            "   _                          _   \n"
            " _| |_ ______ ______ ______ _| |_ \n"
            "|_   _|______|______|______|_   _|\n"
            "  |_|                        |_|  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  |_|                       _\\_/_  \n"
            "  | |                      /  _  \\ \n"
            "  | |                     |  | |  |\n"
            "  | |                     |  | |  |\n"
            "  | |                     |  |_|  |\n"
            "  | |                      \\_____/  \n"
            "  | |                     ===| |===    \n"
            "  |_|                    / / | | \\ \\  \n"
            "  | |                   / /  | |  \\ \\ \n"
            "  | |                  /_/   |_|   \\_\\\n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  |_|                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            " | _ |___ ________ ________ ________ ________ ________ ________ \n"
            "|________|________|________|________|________|________|________|\n \n"

            "MISTAKES: 4 OUT OF 6\n"
            ;
        std::cout << hangman4;
}
    else if (mistakes == 5) {
        system("cls");
        const char* hangman5 =
            "   _                          _   \n"
            " _| |_ ______ ______ ______ _| |_ \n"
            "|_   _|______|______|______|_   _|\n"
            "  |_|                        |_|  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  |_|                       _\\_/_  \n"
            "  | |                      /  _  \\ \n"
            "  | |                     |  | |  |\n"
            "  | |                     |  | |  |\n"
            "  | |                     |  |_|  |\n"
            "  | |                      \\_____/  \n"
            "  | |                     ===| |===    \n"
            "  |_|                    / / | | \\ \\  \n"
            "  | |                   / /  | |  \\ \\ \n"
            "  | |                  /_/  /|_|   \\_\\\n"
            "  | |                      / /     \n"
            "  | |                     / /       \n"
            "  | |                    /_/        \n"
            "  | |                             \n"
            "  |_|                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            " | _ |___ ________ ________ ________ ________ ________ ________ \n"
            "|________|________|________|________|________|________|________|\n \n"

            "MISTAKES: 5 OUT OF 6\n"
            ;
        std::cout << hangman5;
}
    else if (mistakes == 6) {
        system("cls");
        const char* hangman6 =
            "   _                          _   \n"
            " _| |_ ______ ______ ______ _| |_ \n"
            "|_   _|______|______|______|_   _|\n"
            "  |_|                        |_|  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  | |                        | |  \n"
            "  |_|                       _\\_/_  \n"
            "  | |                      /  _  \\ \n"
            "  | |                     |  | |  |\n"
            "  | |                     |  | |  |\n"
            "  | |                     |  |_|  |\n"
            "  | |                      \\_____/  \n"
            "  | |                     ===| |===    \n"
            "  |_|                    / / | | \\ \\  \n"
            "  | |                   / /  | |  \\ \\ \n"
            "  | |                  /_/  /|_|\\  \\_\\\n"
            "  | |                      / / \\ \\ \n"
            "  | |                     / /   \\ \\\n"
            "  | |                    /_/     \\_\\ \n"
            "  | |                             \n"
            "  |_|                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            "  | |                             \n"
            " | _ |___ ________ ________ ________ ________ ________ ________ \n"
            "|________|________|________|________|________|________|________|\n \n"

            "MISTAKES: 6 OUT OF 6\n"
            ;
        std::cout << hangman6;
}


}

void Game::renderBlanks() {
    std::cout << "WORD: ";
    for (int i = 0; i < w.getLength(); i++) {
        if (wordChars[i] == w.getText()[i]) {
            std::cout << dye::light_green("[") << dye::light_green(wordChars[i]) << dye::light_green("]   ");
        }
        else {
            std::cout << dye::yellow("[_]   ");
        }
    }
    std::cout << std::endl;
}

void Game::renderGameOver() {
    system("cls");
    renderHangman(mistakes);
    const char* gameOver =
        "   _____                         ____                   _               \n"
        "  / ____|                       / __ \\                 | |             \n"
        " | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __  | |              \n"
        " | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__| | |         \n"
        " | |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |    |_|            \n"
        "  \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|    (_) \n";

    std::cout << dye::light_red(gameOver);
    std::cout << "\nTHE WORD WAS [" << dye::light_blue(w.getText()) << "]\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    system("cls");
    std::string input;
    do {
        input = "";
        std::cout << "\n|Do you want to|" << dye::aqua("================================================\n");
        std::cout << "\n[1] Exit program\n";
        std::cout << "\n[2] Exit to the menu\n";
        std::cout << dye::aqua("===============================================================\n");
        std::cout << "YOUR INPUT: "; std::cin >> input;
        system("cls");
        std::cout << isIntInput(input, 2);
    } while (!isIntInput(input, 2));
    if (stoi(input) == 1) { exit(0); }
}

void Game::renderWin() {
    system("cls");
    const char* win =
        "__     __                                _\n"
        "\\ \\   / /                               | |        \n"
        " \\ \\_/ /__  _   _ __      _____  _ __   | |         \n"
        "  \\   / _ \\| | | |\\ \\ /\\ / / _ \\| '_ \\  | |          \n"
        "   | | (_) | |_| | \\ V  V / (_) | | | | |_|           \n"
        "   |_|\\___/ \\__,_|  \\_/\\_/ \\___/|_| |_| (_)    \n"
        ;
    std::cout << dye::light_green(win);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    system("cls");
    std::string input;
    do {
        input = "";
        std::cout << "\n|Do you want to|" << dye::aqua("================================================\n");
        std::cout << "\n[1] Exit program\n";
        std::cout << "\n[2] Exit to the menu\n";
        std::cout << dye::aqua("===============================================================\n");
        std::cout << "YOUR INPUT: "; std::cin >> input;
        std::cout << isIntInput(input, 2);
        system("cls");
    } while (!isIntInput(input, 2));
    if (stoi(input) == 1) { exit(0); }
}

void Game::renderLogo()
{
    //system("cls");
        const char* logo =
            " ______ ______ ______ ______ ______ ______ ______ ______\n"
            "|______|______|______|______|______|______|______|______|\n"
            "| |   _    _                                          | |\n"
            "| |  | |  | |	                                      | |\n"
            "| |  | |__| | __ _ _ __   __ _ _ __ ___   __ _ _ __   | |\n"
            "| |  |  __  |/ _` | '_ \\ / _` | '_ ` _ \\ / _` | '_ \\  | |\n"
            "| |  | |  | | (_| | | | | (_| | | | | | | (_| | | | | | |\n"
            "| |  |_|  |_|\\__,_|_| |_|\\__, |_| |_| |_|\\__,_|_| |_| | |\n"
            "| |                       __/ |                       | |\n"
            "| |                      |___/                        | |\n"
            "|_|____ ______ ______ _____________ ______ ______ ____|_|\n"
            "|______|______|______|______|______|______|______|______|\n";
        std::cout << dye::aqua(logo);
}
//=============================================================
