#include <sstream>
#include "Analyzer.h"
using namespace SimpleCC;

TokenInfo Analyzer::getNext() {
	if (currPos < tokens.size()) return tokens[currPos++];
	else currPos = tokens.size() + 1;
	return { Token::ENDED, "\0" };
}

TokenInfo Analyzer::getCurr() {
	if (currPos < tokens.size()) return tokens[currPos];
	return { Token::ENDED, "\0" };
}

void Analyzer::goBack() {
	if (currPos <= tokens.size()) currPos--;
}

//	总的来说 我们需要这些
//	S	->	E|C|B

//	算数部分
//	E	->	TE1
//	E1	->	+TE1|-TE1|null
//	T	->	FT1
//	T1	->	*FT1|/FT1|null
//	F	->	(E)|id

//	条件部分
//	C	->	(C)
//			E >= E|E <= E|E == E|E != E|E > E|E < E

//	布尔部分
//	B	->	(B) B1|not B B1|C B1
//	B1	->	and B B1|or B B1|null

bool Analyzer::findE() {
	if (!findT()) return false;
	if (!findE1()) return false;
	analyseInfo.push_back("using E	->	TE1");
	return true;
}
bool Analyzer::findE1() {
	auto nextTok = getNext();
	if (nextTok.first == Token::OP_ADD) {
		if (!findT()) return false;
		if (!findE1()) return false;
		analyseInfo.push_back("using E1	->	+TE1");
		return true;
	}
	else if (nextTok.first == Token::OP_SUB) {
		if (!findT()) return false;
		if (!findE1()) return false;
		analyseInfo.push_back("using E1	->	-TE1");
		return true;
	}
	else {
		goBack();
		analyseInfo.push_back("using E1	->	null");
		return true;
	}
}
bool Analyzer::findT() {
	if (!findF()) return false;
	if (!findT1()) return false;
	analyseInfo.push_back("using T	->	FT1");
	return true;
}
bool Analyzer::findT1() {
	auto nextTok = getNext();
	if (nextTok.first == Token::OP_MUL) {
		if (!findF()) return false;
		if (!findT1()) return false;
		analyseInfo.push_back("using T1	->	*FT1");
		return true;
	}
	else if (nextTok.first == Token::OP_DIV) {
		if (!findF()) return false;
		if (!findT1()) return false;
		analyseInfo.push_back("using T1	->	/FT1");
		return true;
	}
	else {
		goBack();
		analyseInfo.push_back("using T1	->	null");
		return true;
	}
}
bool Analyzer::findF() {
	auto nextTok = getNext();
	if (nextTok.first == Token::OP_LSBRACK) {
		if (!findE()) return false;
		nextTok = getNext();
		if (nextTok.first == Token::OP_RSBRACK) {
			analyseInfo.push_back("using F	->	(E)");
			return true;
		}
		else {
			return false;
		}
	}
	else if (nextTok.first == Token::IDENTIFIER || nextTok.first == Token::FLOATING || nextTok.first == Token::INTERGER || nextTok.first == Token::HEXINTERGER || nextTok.first == Token::OCTINTERGER) {
		analyseInfo.push_back("using F	->	id");
		return true;
	}
	else {
		return false;
	}
}
bool Analyzer::findC() {
	auto nextTok = getNext();
	if (nextTok.first == Token::OP_LSBRACK) {
		if (!findC()) return false;
		nextTok = getNext();
		if (nextTok.first == Token::OP_RSBRACK) {
			analyseInfo.push_back("using C	->	(C)");
			return true;
		}
		else {
			return false;
		}
	}
	else {
		goBack();
		if (!findE()) return false;
		nextTok = getNext();
		if (nextTok.first == Token::OP_EQ || nextTok.first == Token::OP_GT || nextTok.first == Token::OP_GE || nextTok.first == Token::OP_LS || nextTok.first == Token::OP_LE) {
			if (!findE()) return false;
			analyseInfo.push_back("using C	->	E <=> E");
			return true;
		}
		else {
			return false;
		}
	}
}
bool Analyzer::findB() {
	auto nextTok = getNext();
	if (nextTok.first == Token::OP_LSBRACK) {
		if (!findB()) return false;
		nextTok = getNext();
		if (nextTok.first == Token::OP_RSBRACK) {
			if (!findB1()) return false;
			analyseInfo.push_back("using B	->	(B) B1");
			return true;
		}
		else {
			return false;
		}
	}
	else if (nextTok.first == Token::KEYWORD_NOT) {
		if (!findB())return false;
		if (!findB1())return false;
		analyseInfo.push_back("using B	->	not B B1");
		return true;
	}
	else {
		goBack();
		if (!findC())return false;
		if (!findB1())return false;
		analyseInfo.push_back("using B	->	C B1");
		return true;
	}
}
bool Analyzer::findB1() {
	auto nextTok = getNext();
	if (nextTok.first == Token::KEYWORD_AND) {
		if (!findB()) return false;
		if (!findB1()) return false;
		analyseInfo.push_back("using B1	->	and B B1");
		return true;
	}
	else if (nextTok.first == Token::KEYWORD_OR) {
		if (!findB()) return false;
		if (!findB1()) return false;
		analyseInfo.push_back("using B1	->	or B B1");
		return true;
	}
	else {
		goBack();
		analyseInfo.push_back("using B1	->	null");
		return true;
	}
}

bool Analyzer::analyseAll() {
	bool Esucc = false, Csucc = false, Bsucc = false;
	if (findE()) Esucc = true; else analyseInfo.push_back("FAILED findE.");
	currPos = 0; analyseInfo.push_back("\n\n--seprate--\n\n");
	if (findC()) Csucc = true; else analyseInfo.push_back("FAILED findC.");
	currPos = 0; analyseInfo.push_back("\n\n--seprate--\n\n");
	if (findB()) Bsucc = true; else analyseInfo.push_back("FAILED findB.");
	
	if (Esucc || Csucc || Bsucc) return true;
	else return false;
}

const std::vector<AnalysisInfo>& Analyzer::getInfo() {
	return analyseInfo;
}
