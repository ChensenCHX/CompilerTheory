#pragma once

#include <string>
#include <optional>
#include <vector>
#include <map>

namespace SimpleCC {
	class Lexer;
	enum class Token {
		INVALID,		//	�Ƿ�̬
		IDENTIFIER,		//	��ʶ��
		INTERGER,		//	����������
		HEXINTERGER,	//	ʮ����������
		OCTINTERGER,	//	�˽�������
		FLOATING,		//	����������
		OP_ADD,			//	+�����
		OP_SUB,			//	-�����
		OP_MUL,			//	*�����
		OP_DIV,			//	/�����
		OP_GT,			//	>�����
		OP_LS,			//	<�����
		OP_EQ,			//	==�����
		OP_GE,			//	>=�����
		OP_LE,			//	<=�����
		OP_ASSIGN,		//	=�����(��ֵ)
		KEYWORD_IF,		//	if
		KEYWORD_THEN,	//	then
		KEYWORD_ELSE,	//	else
		KEYWORD_WHILE,	//	while
		KEYWORD_DO,		//	do


	};

	static std::map<std::string, Token> Keywords{
		{ "if",		Token::KEYWORD_IF },
		{ "then",	Token::KEYWORD_THEN },
		{ "else",	Token::KEYWORD_ELSE },
		{ "while",	Token::KEYWORD_WHILE },
		{ "do",		Token::KEYWORD_DO }
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