#include "Lexer.h"
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
	����DFA
*/


std::vector<std::pair<SimpleCC::Token, std::string>> Lexer::parseAll() {
	enum parseState {
		Start,			//��ʼ
		End,			//�������һ��
		Error,			//������һ������

		ID_A_ST,		//idʶ��(��ͷ)
		ID_A,			//idʶ��
		ID_B,			//����id�뱣����

		Number_IoF,		//��ͨ���ͻ򸡵�
		Number_HIoOIoF,	//Hex|Oct���ͻ򸡵�	�Լ��������: ����0
		Number_F_E,		//����,����β�����ȴ����ܳ��ֵ�E
		Number_F_T,		//����,����β��(�ȴ����ܴ��ڵ�+-��������һ����)
		Number_F_TT,	//����,����β��(�ȴ�����һ����)
		Number_F_TD,	//����,����β��(�ɽ���)
		Number_HI,		//Hex����
		Number_OI,		//Oct����

	};

	parseState state = Start;
	std::string buffer;
	std::vector<std::pair<SimpleCC::Token, std::string>> result;
	char ch; bool parsing = true;
	while (parsing) {
		switch (state) {
			case Start:
				ch = getNext();
				isAlphabet(ch);
				if (isAlphabet(ch) || ch == '_') state = ID_A, buffer.push_back(ch);
				else if (isNumber(ch) && ch != '0') state = Number_IoF, buffer.push_back(ch);
				else if (isNumber(ch) && ch == '0') state = Number_HIoOIoF, buffer.push_back(ch);
				// else if (ch == '.') state = Number_F_E, buffer.push_back(ch);	//��C/C++��������.15 -> 0.15����ʽ�����Լ�15. -> 15.0����ʽ����
				else if (ch == EOF) parsing = false;
				//TODO




				break;

			case ID_A:
				ch = getNext();
				if (isAlphabet(ch) || ch == '_' || isNumber(ch)) state = ID_A, buffer.push_back(ch);
				else state = ID_B;
				break;
			case ID_B:
				if (SimpleCC::Keywords[buffer] != SimpleCC::Token::INVALID) result.push_back({ SimpleCC::Keywords[buffer], buffer });
				else result.push_back({ SimpleCC::Token::IDENTIFIER, buffer });
				goBack();	//���������֧�����һ���ַ� �µ�
				state = End;
				break;

			case Number_IoF:
				ch = getNext();
				if (isNumber(ch)) state = Number_IoF, buffer.push_back(ch);
				else if (ch == '.') state = Number_F_E, buffer.push_back(ch);
				else if (ch == 'e' or ch == 'E') state = Number_F_T, buffer.push_back(ch);
				else state = End, result.push_back({ SimpleCC::Token::INTERGER, buffer }), goBack();	//���������֧�����һ���ַ� �µ�
				break;

			case Number_F_E:
				ch = getNext();
				if (isNumber(ch)) state = Number_F_E, buffer.push_back(ch);
				else if (ch == 'e' or ch == 'E') state = Number_F_T, buffer.push_back(ch);
				else state = End, result.push_back({ SimpleCC::Token::FLOATING, buffer }), goBack();	//���������֧�����һ���ַ� �µ�
				break;

			case Number_F_T:
				ch = getNext();
				if (isNumber(ch)) state = Number_F_TD, buffer.push_back(ch);
				else if(ch == '+' || ch == '-') state = Number_F_TT, buffer.push_back(ch);
				else state = Error, buffer.push_back(ch), goBack();	//���������֧�����һ���ַ� �µ� E��û�����Ǵ�������, ��Error(˳��ѵ��´�����ַ�ѹ��ȥ)
				break;
			
			case Number_F_TT:
				ch = getNext();
				if (isNumber(ch)) state = Number_F_TD, buffer.push_back(ch);
				else state = Error, buffer.push_back(ch), goBack();	//���������֧�����һ���ַ� �µ� E+/E-��û�����Ǵ�������, ��Error(˳��ѵ��´�����ַ�ѹ��ȥ)
				break;

			case Number_F_TD:
				ch = getNext();
				if (isNumber(ch)) state = Number_F_TD, buffer.push_back(ch);
				else state = End, result.push_back({ SimpleCC::Token::FLOATING, buffer }), goBack();	//���������֧�����һ���ַ� �µ�
				break;

			case Number_HIoOIoF:
				ch = getNext();
				if (ch == 'x') state = Number_HI, buffer.push_back(ch);
				else if (ch == '.') state = Number_F_E, buffer.push_back(ch);
				else if (ch == 'e' or ch == 'E') state = Number_F_T, buffer.push_back(ch);
				else if (isNumber(ch) && ch < '8' && ch != '0') state = Number_OI, buffer.push_back(ch);
				else if (!isNumber(ch)) state = End, result.push_back({ SimpleCC::Token::INTERGER, buffer }), goBack();	//����0����� ���������֧�����һ���ַ� �µ�
				else state = Error, buffer.push_back(ch), goBack();	//���������֧�����һ���ַ� �µ� �������8����9�Ǵ������, ��Error(˳��ѵ��´�����ַ�ѹ��ȥ)
				break;

			case Number_HI:
				ch = getNext();
				if (isNumber(ch) || ch >= 'A' && ch <= 'F' || ch >= 'a' && ch <= 'f') state = Number_HI, buffer.push_back(ch);
				else state = End, result.push_back({ SimpleCC::Token::HEXINTERGER, buffer }), goBack();	//���������֧�����һ���ַ� �µ�
				break;

			case Number_OI:
				ch = getNext();
				if (isNumber(ch) && ch < '8') state = Number_OI, buffer.push_back(ch);
				else state = End, result.push_back({ SimpleCC::Token::OCTINTERGER, buffer }), goBack();	//���������֧�����һ���ַ� �µ�
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
				break;
		}
	}
	return result;
}