#include "WordLoader.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>


WordLoader::WordLoader()
{

	this->fileName = "hangmanVocab.txt";
	std::ifstream myfile;
	myfile.open(fileName);
	this->maxLength = 0;
	this->minLength = INT_MAX;
	this->wordCount = 0;

	std::string currLine;

	while (getline(myfile, currLine)) {

		if (currLine.length() > maxLength) { maxLength = currLine.length(); }
		if (currLine.length() < minLength && currLine.length() > 0) { minLength = currLine.length(); }
		wordCount++;
	}

}

std::string WordLoader::loadRandomWord()
{
	srand(time(NULL)+rand()%wordCount);
	
	int wordIndex = rand() % wordCount ;
	std::ifstream myfile;
	myfile.open(fileName);
	std::string retWord = "";
	
	while (wordIndex != 0) {
		getline(myfile, retWord);
		wordIndex--;
	}
	return retWord;
}

std::string WordLoader::loadRandomWord(int desiredLength)
{
	std::vector<std::string> filteredWordList;
	
	std::ifstream myfile;
	myfile.open(fileName);
	std::string wrd;
	while (getline(myfile,wrd)) {
		if (wrd.length() == desiredLength) { filteredWordList.push_back(wrd); }
	}
	
	if (filteredWordList.size() == 0) {
		std::cout << "NO WORD OF SIZE " << desiredLength << " IN FILE \"" <<fileName<< "\"\n";
		int input;
		std::cout << "CHOOSE ANOTHER SIZE: "; std::cin >> input;
		return loadRandomWord(input);
	}
	srand(time(NULL));
	return filteredWordList[rand() % filteredWordList.size()];
}

int WordLoader::getMaxLength()
{
	return maxLength;
}

int WordLoader::getMinLength()
{
	return minLength;
}

int WordLoader::getWordCount()
{
	return wordCount;
}

void WordLoader::printReport()
{
	system("cls");
	std::cout << "\n=========================================================\n";
	std::cout << "FILENAME: " << fileName << std::endl;
	std::cout << "WORDS TOTAL: " << wordCount << std::endl;
	std::cout << "SMALLEST WORD SIZE: " << minLength << std::endl;
	std::cout << "BIGGEST WORD SIZE: " << maxLength << std::endl;
	std::cout << "=========================================================\n";
}

void WordLoader::printAll() {
	
	std::ifstream myfile;
	myfile.open(fileName);
	std::string wrd;
	while (getline(myfile, wrd)) {
		std::cout << wrd << std::endl; 
	}
	
}
