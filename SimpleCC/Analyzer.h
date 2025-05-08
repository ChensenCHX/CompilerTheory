#pragma once
#include <vector>
#include <string>
#include "Lexer.h"

namespace SimpleCC {
	enum class StmtProperty
	{

	};


	using AnalysisInfo = std::pair<SimpleCC::StmtProperty, std::vector<std::string>>;
}

class SimpleCC::Analyzer {
private:
	size_t currPos;

public:
	Analyzer();
	void analyseAll(const std::vector<SimpleCC::TokenInfo>& tokens);
};
