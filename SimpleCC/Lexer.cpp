#include "Lexer.h"
#include <iostream>
#include <cassert>
using namespace SimpleCC;

bool Lexer::isNumber(char ch) { return ch >= '0' && ch <= '9'; }
bool Lexer::isAlphabet(char ch) { return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'); }

char Lexer::getNext() {
	if (currPos < oriText.length()) return oriText[currPos++];
	else currPos = oriText.length() + 1;
	return EOF;
}

void Lexer::goBack() {
	if (currPos <= oriText.length()) currPos--;
}

/*
	构造DFA
*/


std::vector<std::pair<SimpleCC::Token, std::string>> Lexer::parseAll() {
	enum parseState {
		Start,				//开始
		End,				//分析完成一份
		Error,				//分析出一个错误

		ID_A_ST,			//id识别(开头)
		ID_A,				//id识别
		ID_B,				//区分id与保留字

		Number_IoF,			//普通整型或浮点
		Number_HIoOIoF,		//Hex|Oct整型或浮点	以及特殊情况: 整数0
		Number_F_E,			//浮点,处理尾数并等待可能出现的E
		Number_F_T,			//浮点,处理纯尾数(等待可能存在的+-号与至少一个数)
		Number_F_TT,		//浮点,处理纯尾数(等待至少一个数)
		Number_F_TD,		//浮点,处理纯尾数(可结束)
		Number_HI,			//Hex整型
		Number_OI,			//Oct整型

		Operator_Assign,	//运算符=
		Operator_Equal,		//运算符==
		Operator_Greater,	//运算符>
		Operator_GreaterEq,	//运算符>=
		Operator_Less,		//运算符<
		Operator_LessEq,	//运算符<=

	};

	parseState state = Start;
	std::string buffer;
	std::vector<std::pair<SimpleCC::Token, std::string>> result;
	char ch; bool parsing = true;
	while (parsing) {
		switch (state) {
			case Start:
				ch = getNext();

				//	判别id|关键字
				if (isAlphabet(ch) || ch == '_') state = ID_A, buffer.push_back(ch);
				//	判别各种数字字面量
				else if (isNumber(ch) && ch != '0') state = Number_IoF, buffer.push_back(ch);
				else if (isNumber(ch) && ch == '0') state = Number_HIoOIoF, buffer.push_back(ch);
				//	判别各种运算符
				else if (ch == '+') state = End, buffer.push_back(ch), result.push_back({ SimpleCC::Token::OP_ADD, buffer });
				else if (ch == '-') state = End, buffer.push_back(ch), result.push_back({ SimpleCC::Token::OP_SUB, buffer });
				else if (ch == '*') state = End, buffer.push_back(ch), result.push_back({ SimpleCC::Token::OP_MUL, buffer });
				else if (ch == '/') state = End, buffer.push_back(ch), result.push_back({ SimpleCC::Token::OP_DIV, buffer });
				else if (ch == '(') state = End, buffer.push_back(ch), result.push_back({ SimpleCC::Token::OP_LSBRACK, buffer });
				else if (ch == ')') state = End, buffer.push_back(ch), result.push_back({ SimpleCC::Token::OP_RSBRACK, buffer });
				else if (ch == ';') state = End, buffer.push_back(ch), result.push_back({ SimpleCC::Token::OP_SEPRATOR, buffer });
				else if (ch == '=') state = Operator_Assign, buffer.push_back(ch);
				else if (ch == '>') state = Operator_Greater, buffer.push_back(ch);
				else if (ch == '<') state = Operator_Less, buffer.push_back(ch);
				//	结束
				else if (ch == EOF) parsing = false;
				break;
			
			




			

			case ID_A:
				ch = getNext();
				if (isAlphabet(ch) || ch == '_' || isNumber(ch)) state = ID_A, buffer.push_back(ch);
				else state = ID_B;
				break;

			case ID_B:
				if (SimpleCC::Keywords[buffer] != SimpleCC::Token::INVALID) result.push_back({ SimpleCC::Keywords[buffer], buffer });
				else result.push_back({ SimpleCC::Token::IDENTIFIER, buffer });
				goBack();	//进入这个分支会多吞一个字符 吐掉
				state = End;
				break;

			case Number_IoF:
				ch = getNext();
				if (isNumber(ch)) state = Number_IoF, buffer.push_back(ch);
				else if (ch == '.') state = Number_F_E, buffer.push_back(ch);
				else if (ch == 'e' or ch == 'E') state = Number_F_T, buffer.push_back(ch);
				else state = End, result.push_back({ SimpleCC::Token::INTERGER, buffer }), goBack();	//进入这个分支会多吞一个字符 吐掉
				break;

			case Number_F_E:
				ch = getNext();
				if (isNumber(ch)) state = Number_F_E, buffer.push_back(ch);
				else if (ch == 'e' or ch == 'E') state = Number_F_T, buffer.push_back(ch);
				else state = End, result.push_back({ SimpleCC::Token::FLOATING, buffer }), goBack();	//进入这个分支会多吞一个字符 吐掉
				break;

			case Number_F_T:
				ch = getNext();
				if (isNumber(ch)) state = Number_F_TD, buffer.push_back(ch);
				else if(ch == '+' || ch == '-') state = Number_F_TT, buffer.push_back(ch);
				else state = Error, buffer.push_back(ch), goBack();	//进入这个分支会多吞一个字符 吐掉 E后没数字是错误的情况, 进Error(顺便把导致错误的字符压进去)
				break;
			
			case Number_F_TT:
				ch = getNext();
				if (isNumber(ch)) state = Number_F_TD, buffer.push_back(ch);
				else state = Error, buffer.push_back(ch), goBack();	//进入这个分支会多吞一个字符 吐掉 E+/E-后没数字是错误的情况, 进Error(顺便把导致错误的字符压进去)
				break;

			case Number_F_TD:
				ch = getNext();
				if (isNumber(ch)) state = Number_F_TD, buffer.push_back(ch);
				else state = End, result.push_back({ SimpleCC::Token::FLOATING, buffer }), goBack();	//进入这个分支会多吞一个字符 吐掉
				break;

			case Number_HIoOIoF:
				ch = getNext();
				if (ch == 'x') state = Number_HI, buffer.push_back(ch);
				else if (ch == '.') state = Number_F_E, buffer.push_back(ch);
				else if (ch == 'e' or ch == 'E') state = Number_F_T, buffer.push_back(ch);
				else if (isNumber(ch) && ch < '8' && ch != '0') state = Number_OI, buffer.push_back(ch);
				else if (!isNumber(ch)) state = End, result.push_back({ SimpleCC::Token::INTERGER, buffer }), goBack();	//整数0的情况 进入这个分支会多吞一个字符 吐掉
				else state = Error, buffer.push_back(ch), goBack();	//进入这个分支会多吞一个字符 吐掉 这里出现8或者9是错误情况, 进Error(顺便把导致错误的字符压进去)
				break;

			case Number_HI:
				ch = getNext();
				if (isNumber(ch) || ch >= 'A' && ch <= 'F' || ch >= 'a' && ch <= 'f') state = Number_HI, buffer.push_back(ch);
				else state = End, result.push_back({ SimpleCC::Token::HEXINTERGER, buffer }), goBack();	//进入这个分支会多吞一个字符 吐掉
				break;

			case Number_OI:
				ch = getNext();
				if (isNumber(ch) && ch < '8') state = Number_OI, buffer.push_back(ch);
				else state = End, result.push_back({ SimpleCC::Token::OCTINTERGER, buffer }), goBack();	//进入这个分支会多吞一个字符 吐掉
				break;

			case Operator_Assign:
				ch = getNext();
				if (ch == '=')	state = Operator_Equal, buffer.push_back(ch);
				else state = End, result.push_back({ SimpleCC::Token::OP_ASSIGN, buffer }), goBack();	//进入这个分支会多吞一个字符 吐掉
				break;

			case Operator_Equal:
				ch = getNext();
				if (ch == '=')	state = Error, buffer.push_back(ch), goBack();	//进入这个分支会多吞一个字符 吐掉 ===是错误的情况, 进Error(顺便把导致错误的字符压进去)
				else state = End, result.push_back({ SimpleCC::Token::OP_EQ, buffer }), goBack();	//进入这个分支会多吞一个字符 吐掉
				break;

			case Operator_Greater:
				ch = getNext();
				if (ch == '=')	state = Operator_GreaterEq, buffer.push_back(ch);
				else state = End, result.push_back({ SimpleCC::Token::OP_GT, buffer }), goBack();	//进入这个分支会多吞一个字符 吐掉
				break;

			case Operator_GreaterEq:
				// 留一下 万一以后需要>==之类的严格类型比较算符呢.jpg
				state = End, result.push_back({ SimpleCC::Token::OP_GE, buffer });
				break;

			case Operator_Less:
				ch = getNext();
				if (ch == '=')	state = Operator_LessEq, buffer.push_back(ch);
				else state = End, result.push_back({ SimpleCC::Token::OP_LS, buffer }), goBack();	//进入这个分支会多吞一个字符 吐掉
				break;

			case Operator_LessEq:
				// 留一下 万一以后需要<==之类的严格类型比较算符呢.jpg
				state = End, result.push_back({ SimpleCC::Token::OP_LE, buffer });
				break;





			case End:
				buffer.clear();
				state = Start;
				break;

			case Error:
				result.push_back({ SimpleCC::Token::INVALID, buffer });
				buffer.clear();
				state = Start;
				break;

			default:
				assert(false, "This should not happend.\n");
				break;
		}
	}
	return result;
}
