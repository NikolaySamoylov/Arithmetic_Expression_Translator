#include <iostream>
#include "stack.h"
#include <string>

enum term_type {
	NUMBER,
	OPERATOR,
	OPENBRACKET,
	CLOSEBRACKET
};

class Term {
protected:
	term_type t;
public:
	term_type get_type()
	{
		return t;
	}
	virtual void print() = 0;
};

class Number: public Term {
public:
	double value;
	Number(double num)
	{
		t = term_type::NUMBER;
		value = num;
	}
	void print() override
	{
		std::cout << value;
	}
};

class Operator : public Term {
public:
	char op;
	Operator(char oper)
	{
		t = term_type::OPERATOR;
		op = oper;
	}
	bool priority() 
	{
		if (op == '+' || op == '-')
			return false;
		if (op == '*' || op == '/')
			return true;
	}
	void print() override
	{
		std::cout << op;
	}
};

class OpenBracket : public Term {
public:
	OpenBracket()
	{
		t = term_type::OPENBRACKET;
	}
	void print() override
	{
		std::cout << '(';
	}
};

class CloseBracket : public Term {
public:
	CloseBracket()
	{
		t = term_type::CLOSEBRACKET;
	}
	void print() override
	{
		std::cout << ')';
	}
};

class Expression {
protected:
	std::string expression;  // исходное выражение
	Vector<Term*> lexemes;  // массив лексем исходного выражения
	Vector<Term*> postfix_notation; // постфиксная запись исходного выражения
public:
	//конструктор с параметром
	Expression(std::string s)
	{
		expression = s;
	}
	//деструктор
	~Expression()
	{
		for (size_t i = 0; i < lexemes.size(); i++)
			delete lexemes[i];
	}
	// лексический анализ
	bool lexical_analysis()
	{
		std::string s = expression;
		bool res = true;
		while (s.length())
		{
			if (s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/')
			{
				Term* l = new Operator(s[0]);
				lexemes.push_back(l);
				s.erase(0, 1);
			}
			else if (s[0] == '0' || s[0] == '1' || s[0] == '2' || s[0] == '3' || s[0] == '4'
				|| s[0] == '5' || s[0] == '6' || s[0] == '7' || s[0] == '8' || s[0] == '9')
			{
				size_t len;
				Term* l = new Number(std::stod(s, &len));
				lexemes.push_back(l);
				s.erase(0, len);
			}
			else if (s[0] == '(')
			{
				Term* l = new OpenBracket();
				lexemes.push_back(l);
				s.erase(0, 1);
			}
			else if (s[0] == ')')
			{
				Term* l = new CloseBracket();
				lexemes.push_back(l);
				s.erase(0, 1);
			}
			else
			{
				if (s[0] != '=')
					res = false;
				break;
			}
		}
		return res;
	}
	// синтаксический анализ
	bool syntactic_analysis()
	{
		bool res = true;
		int bracket_control = 0;
		term_type last_lex = lexemes[lexemes.size() - 1]->get_type();
		for (size_t i = 0; i < lexemes.size() - 1; i++)
		{
			term_type nl = lexemes[i + 1]->get_type();
			switch (lexemes[i]->get_type())
			{
			case term_type::OPERATOR:
			{
				if (nl == term_type::OPERATOR || nl == term_type::CLOSEBRACKET)
					res = false;
				break;
			}
			case term_type::NUMBER:
			{
				if (nl == term_type::NUMBER || nl == term_type::OPENBRACKET)
					res = false;
				break;
			}
			case term_type::OPENBRACKET:
			{
				if (nl == term_type::OPERATOR || nl == term_type::CLOSEBRACKET)
					res = false;
				else
					bracket_control++;
				break;
			}
			case term_type::CLOSEBRACKET:
			{
				if (nl == term_type::NUMBER || nl == term_type::OPENBRACKET)
					res = false;
				else
					bracket_control--;
				break;
			}
			}
		}
		if (last_lex == term_type::OPERATOR || last_lex == term_type::OPENBRACKET)
			res = false;
		if (last_lex == term_type::CLOSEBRACKET)
			bracket_control--;
		if (bracket_control != 0)
			res = false;
		return res;
	}
	// преобразование в постфиксную запись 
	void go_to_postfix_notation()
	{
		Stack<Term*> lexemes_stack;
		for (int i = 0; i < lexemes.size(); i++)
		{
			switch (lexemes[i]->get_type())
			{
			case term_type::OPERATOR:
			{
				while (!lexemes_stack.empty() && lexemes_stack.top()->get_type() == term_type::OPERATOR 
					&& static_cast<Operator*>(lexemes_stack.top())->priority() >=
					static_cast<Operator*>(lexemes[i])->priority())
					{
						postfix_notation.push_back(lexemes_stack.top());
						lexemes_stack.pop();
					}
				lexemes_stack.push(lexemes[i]);
				break;
			}
			case term_type::NUMBER:
			{
				postfix_notation.push_back(lexemes[i]);
				break;
			}
			case term_type::OPENBRACKET:
			{
				lexemes_stack.push(lexemes[i]);
				break;
			}
			case term_type::CLOSEBRACKET:
			{
				while (lexemes_stack.top()->get_type() != term_type::OPENBRACKET)
				{
					postfix_notation.push_back(lexemes_stack.top());
					lexemes_stack.pop();
				}
				lexemes_stack.pop();
				break;
			}
			}
		}
		while (!lexemes_stack.empty())
		{
			postfix_notation.push_back(lexemes_stack.top());
			lexemes_stack.pop();
		}
	}
	// вычисление выражения в постфиксной записи
	double calculation()
	{
		Stack<double> arguments;
		lexical_analysis();
		go_to_postfix_notation();
		for (size_t i = 0; i < postfix_notation.size(); i++)
			switch (postfix_notation[i]->get_type())
			{
			case term_type::OPERATOR:
			{
				double second_argument = arguments.top();
				arguments.pop();
				double first_argument = arguments.top();
				arguments.pop();
				switch (static_cast<Operator*>(postfix_notation[i])->op)
				{
				case '+':
				{
					arguments.push(first_argument + second_argument);
					break;
				}
				case '-':
				{
					arguments.push(first_argument - second_argument);
					break;
				}
				case '*':
				{
					arguments.push(first_argument * second_argument);
					break;
				}
				case '/':
				{
					if (second_argument != 0.0)
						arguments.push(first_argument / second_argument);
					else
						throw "Division by zero!";
					break;
				}
				}
				break;
			}
			case term_type::NUMBER:
			{
				arguments.push(static_cast<Number*>(postfix_notation[i])->value);
				break;
			}
			}
		return arguments.top();
	}
	// транслятор арифметических выражений
	void translator()
	{
		double answer;
		if (lexical_analysis() == false)
			throw "Wrong lexical_analysis!";
		if (syntactic_analysis() == false)
			throw "Wrong syntactic_analysis!";
		std::cout << "The source expression:" << std::endl;
		for (size_t i = 0; i < lexemes.size(); i++)
			lexemes[i]->print();
		std::cout << std::endl;
		go_to_postfix_notation();
		std::cout << "The postfix notation:" << std::endl;
		for (size_t i = 0; i < postfix_notation.size(); i++)
			postfix_notation[i]->print();
		std::cout << std::endl << "The answer:" << std::endl;
		answer = calculation();
		std::cout << answer;
	}
};