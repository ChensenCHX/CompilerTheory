#pragma once

#include <string>
#include <vector>
#include "SimpleCC.h"

namespace SimpleCC {
	using TokenInfo = std::pair<SimpleCC::Token, std::string>;
}

class SimpleCC::Lexer {
private:
	std::string oriText;
	std::string sepText;
	
	size_t currPos;
public:
	Lexer(std::string text) : oriText(text), currPos(0) {};
	char getNext();
	void goBack();
	bool isNumber(char ch);
	bool isAlphabet(char ch);
	
	std::vector<SimpleCC::TokenInfo> parseAll();
};
