#include "Word.h"
#include<fstream>


void Word::setText(std::string _text)
{
	text = _text;
	length = text.length();
}

std::string Word::getText()
{
	return text;
}

int Word::getLength()
{
	return length;
}

void Word::printSourceReport()
{
	wl.printReport();
}

WordLoader Word::getWordLoader() {
	return wl;
}

bool Word::guess(char inputC)
{
	for (int i = 0; i < text.length(); i++) {
		if (text[i] == inputC) { return true; }
	}
	return false;
}

bool Word::guess(std::string inputStr)
{
	return (inputStr == text);
}

//construcotry=============================
Word::Word()
{
	wl = WordLoader();
	setText(wl.loadRandomWord());
}

Word::Word(std::string desiredWord)
{
	setText(desiredWord);
}

Word::Word(int desiredLength)
{
	WordLoader wl = WordLoader();
	setText(wl.loadRandomWord(desiredLength));
}
//construcotry______________________________