#include<fstream>
#ifndef WORDLOADER_H_
#define WORDLOADER_H_

class WordLoader {
private:

	int maxLength;
	int minLength;
	int wordCount;
	std::string fileName;

public:
	//CONSTRUCTOR
	WordLoader();
	//CONSTRUCTOR
	
	std::string loadRandomWord();
	std::string loadRandomWord(int desiredLength);
	
	int getMaxLength();
	int getMinLength();
	int getWordCount();
	void printReport();
	void printAll();

};


#endif
