/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:53:21 by ehedeman          #+#    #+#             */
/*   Updated: 2025/01/31 14:32:01 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN(std::string _input): input(_input)
{
	rpnMain();
}

RPN::RPN(const RPN &src):input(src.getInput())
{
	*this = src;
}

RPN::~RPN()
{
}

RPN		&RPN::operator=(const RPN &src)
{
	if (this == &src)	//nothing because there is nothing to assign (except stack
		return (*this);//but thats highly stupid if the input doesnt align)
	return (*this);
}

const std::string				RPN::getInput()const{return (this->input);}
std::stack<int>					RPN::getStack()const{return (this->stack);}

void							RPN::rpnMain()
{
	float result = 0;
	int first_nbr = 0;
	int second_nbr = 0;
	int op = 0;
	checkFormat();
	for (size_t i = 0; i < this->input.length(); i++)
	{
		readInput(i);
		op = isOperator(input[i]);
		if (op >= 0)
		{
			assignNumbers(first_nbr, second_nbr);
			switch (op)
			{
			case DIVISION:
				if (!second_nbr)
					throw TryingToDivideByZero();
				result = first_nbr / second_nbr;
				break;
			case MULTIPLICATION:
				result = first_nbr * second_nbr;
				break;
			case ADDITION:
				result = first_nbr + second_nbr;
				break;
			case SUBTRACTION:
				result = first_nbr - second_nbr;
				break;
			default:
				std::cout << "symbol case " << op << " not recognized." << std::endl;
			}
			this->stack.push(result);
		}
	}
	std::cout << result << std::endl;
}

void							RPN::checkFormat()
{
	for (size_t i = 0; i < this->input.length(); i++)
	{
		if (!isValid(input[i]))
			throw WrongFormatException();
	}
}

void							RPN::readInput(size_t &i)
{
	while (this->input[i])
	{
		std::string str = "";
		if (input[i] == ' ')
			i++;
		while (this->input[i] && this->input[i] != ' ' && isOperator(input[i]) < 0)
		{
			str += this->input[i];
			i++;
		}
		if (str != "")
			this->stack.push(strtoi(str));
		if (input[i] == ' ')
		{
			i++;
			if (isOperator(input[i]) >= 0)
				break ;
		}
		else if (isOperator(input[i]) >= 0)
			break ; 
		else
			i++;
	}
}

void							RPN::assignNumbers(int &first_nbr, int &second_nbr)
{
	if (this->stack.size())
	{
		second_nbr = this->stack.top();
		this->stack.pop();
		if (this->stack.size())
		{
			first_nbr = this->stack.top();
			this->stack.pop();
		}
		else
			throw NotAccordingToRPNRules();
	}
	else
		throw NotAccordingToRPNRules();
}

int								RPN::isOperator(char c)
{
	for (size_t i = 0; i < strlen(OPERATORS); i++)
	{
		if (c == OPERATORS[i])
			return i;
	}
	return -1;
}

bool							RPN::isValid(char c)
{
	for (size_t i = 0; i < strlen(VALID_CHARACTERS); i++)
	{
		if (c == VALID_CHARACTERS[i] || isdigit(c))
			return true;
	}
	return false;
}
int								RPN::strtoi(std::string str)
{
	char	**ptr = new char *;
	int		value = 0;
	value = std::strtof(str.c_str(), ptr);
	if (**ptr && (strlen(*ptr) != 1))
		throw ConvertNumberException();
	return value;
}

const char						*RPN::WrongFormatException::what() const throw()
{
	return "Error: wrong format.";
}

const char						*RPN::NotAccordingToRPNRules::what() const throw()
{
	return "Error: RPN rules broken.";
}

const char						*RPN::TryingToDivideByZero::what() const throw()
{
	return "Error: Trying to divide by zero.";
}
const char						*RPN::ConvertNumberException::what() const throw()
{
	return "Error: Invalid number convert.";
}