/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:53:32 by ehedeman          #+#    #+#             */
/*   Updated: 2025/01/31 14:31:57 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <limits>
#include <float.h>
#include <math.h>
#include <stack>
#define OPERATORS "/*+-"
#define VALID_CHARACTERS "/*+- "
#define MAX_NUMBERS_IN_A_ROW 2
class RPN
{
private:
	const std::string		input;
	std::stack<int>			stack;
public:
									RPN(std::string _input);
									RPN(const RPN &src);
									~RPN();
	RPN								&operator=(const RPN &src);

	typedef enum 					s_operator
	{
		DIVISION,
		MULTIPLICATION,
		ADDITION,
		SUBTRACTION
	}								t_operator;	
	const std::string				getInput()const;
	std::stack<int>					getStack()const;

	void							rpnMain();
	void							checkFormat();
	void							readInput(size_t &i);
	void							assignNumbers(int &first_nbr, int &second_nbr);
	int								isOperator(char c);
	bool							isValid(char c);
	int								strtoi(std::string str);
	class WrongFormatException : public std::exception
	{
		public:
			const char* what() const throw();
	};
	class NotAccordingToRPNRules : public std::exception
	{
		public:
			const char* what() const throw();
	};
	class TryingToDivideByZero : public std::exception
	{
		public:
			const char* what() const throw();
	};
		class ConvertNumberException : public std::exception
	{
		public:
			const char* what() const throw();
	};
};