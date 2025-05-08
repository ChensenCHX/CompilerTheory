#pragma once
#include <vector>
#include <string>
#include "Lexer.h"

namespace SimpleCC {
	enum class StmtProperty
	{
		
	};


	using AnalysisInfo = std::string;
}

class SimpleCC::Analyzer {
private:
	std::vector<SimpleCC::AnalysisInfo> analyseInfo;
	std::vector<SimpleCC::TokenInfo> tokens;
	size_t currPos;
	SimpleCC::TokenInfo getNext();
	SimpleCC::TokenInfo getCurr();
	void goBack();

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

	bool findE();
	bool findE1();
	bool findT();
	bool findT1();
	bool findF();
	bool findC();
	bool findB();
	bool findB1();

public:
	Analyzer(const std::vector<SimpleCC::TokenInfo>& tokens) : tokens(tokens), currPos(0) {};
	bool analyseAll();
	const std::vector<SimpleCC::AnalysisInfo>& getInfo();
};
