#pragma once

#include <string>
#include <optional>
#include <vector>
#include <map>

namespace SimpleCC {
	class Lexer;
	enum class Token {
		INVALID,		//	非法态
		IDENTIFIER,		//	标识符
		INTERGER,		//	整型字面量
		HEXINTERGER,	//	十六进制整型
		OCTINTERGER,	//	八进制整型
		FLOATING,		//	浮点字面量
		OP_ADD,			//	+运算符
		OP_SUB,			//	-运算符
		OP_MUL,			//	*运算符
		OP_DIV,			//	/运算符
		OP_GT,			//	>运算符
		OP_LS,			//	<运算符
		OP_EQ,			//	==运算符
		OP_GE,			//	>=运算符
		OP_LE,			//	<=运算符
		OP_ASSIGN,		//	=运算符(赋值)
		OP_LSBRACK,		//	(左小括号
		OP_RSBRACK,		//	)右小括号
		OP_SEPRATOR,	//	;分号分隔符
		KEYWORD_IF,		//	if
		KEYWORD_THEN,	//	then
		KEYWORD_ELSE,	//	else
		KEYWORD_WHILE,	//	while
		KEYWORD_DO,		//	do
		KEYWORD_END,	//	end
		KEYWORD_AND,	//	and
		KEYWORD_OR,		//	or
		KEYWORD_NOT,	//	not

	};

	static std::map<std::string, Token> Keywords{
		{ "if",		Token::KEYWORD_IF },
		{ "then",	Token::KEYWORD_THEN },
		{ "else",	Token::KEYWORD_ELSE },
		{ "while",	Token::KEYWORD_WHILE },
		{ "do",		Token::KEYWORD_DO },
		{ "end",	Token::KEYWORD_END },
		{ "and",	Token::KEYWORD_AND },
		{ "or",		Token::KEYWORD_OR },
		{ "not",	Token::KEYWORD_NOT },
	};
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
	
	std::vector<std::pair<SimpleCC::Token, std::string>> parseAll();
};
