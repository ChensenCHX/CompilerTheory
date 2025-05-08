#pragma once
#include <map>
#include <string>

namespace SimpleCC {
	class Lexer;
	
	class Analyzer;

	enum class Token {
		INVALID,			//	非法态
		ENDED,				//	已结束(用于特殊处理)
		IDENTIFIER,			//	标识符
		INTERGER,			//	整型字面量
		HEXINTERGER,		//	十六进制整型
		OCTINTERGER,		//	八进制整型
		FLOATING,			//	浮点字面量
		OP_ADD,				//	+运算符
		OP_SUB,				//	-运算符
		OP_MUL,				//	*运算符
		OP_DIV,				//	/运算符
		OP_GT,				//	>运算符
		OP_LS,				//	<运算符
		OP_EQ,				//	==运算符
		OP_GE,				//	>=运算符
		OP_LE,				//	<=运算符
		OP_NE,				//	!=运算符
		OP_ASSIGN,			//	=运算符(赋值)
		OP_LSBRACK,			//	(左小括号
		OP_RSBRACK,			//	)右小括号
		OP_SEPRATOR,		//	;分号分隔符
		OP_SPLIT,			//	,分割符
		KEYWORD_IF,			//	if
		KEYWORD_THEN,		//	then
		KEYWORD_ELSE,		//	else
		KEYWORD_WHILE,		//	while
		KEYWORD_DO,			//	do
		KEYWORD_END,		//	end
		KEYWORD_AND,		//	and
		KEYWORD_OR,			//	or
		KEYWORD_NOT,		//	not
		KEYWORD_FOR,		//	for
		KEYWORD_BREAK,		//	break
		KEYWORD_CONTINUE,	//	continue
		KEYWORD_RETURN,		//	return
	};
	static std::map<std::string, Token> Keywords{
		{ "if",			Token::KEYWORD_IF },
		{ "then",		Token::KEYWORD_THEN },
		{ "else",		Token::KEYWORD_ELSE },
		{ "while",		Token::KEYWORD_WHILE },
		{ "do",			Token::KEYWORD_DO },
		{ "end",		Token::KEYWORD_END },
		{ "and",		Token::KEYWORD_AND },
		{ "or",			Token::KEYWORD_OR },
		{ "not",		Token::KEYWORD_NOT },
		{ "for",		Token::KEYWORD_FOR },
		{ "break",		Token::KEYWORD_BREAK },
		{ "continue",	Token::KEYWORD_CONTINUE },
		{ "return",		Token::KEYWORD_RETURN },
	};
}
