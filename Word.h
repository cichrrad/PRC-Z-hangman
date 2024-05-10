#include <string>
#include "WordLoader.h"
#ifndef WORD_H_
#define WORD_H_

class Word {
private:
	
	std::string text;
	int length;
	WordLoader wl;
	void setText(std::string _text);		//zmena slova na custom slovo
	
public:

	std::string getText();		
	int getLength();
	void printSourceReport();				//vola metodu tridy wordloader
	WordLoader getWordLoader();				//vrati wordloader

	bool guess(char inputC);				//hada znak
	bool guess(std::string inputStr);		//hada cele slovo

	//CONSTRUCTORY
	Word();
	Word(std::string desiredWord);
	Word(int desiredLength);
	//CONSTRUCTORY
};

#endif