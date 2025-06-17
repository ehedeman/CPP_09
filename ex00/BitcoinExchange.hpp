/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:31:33 by ehedeman          #+#    #+#             */
/*   Updated: 2025/03/27 11:20:13 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <iostream>
#include <fstream>
#include <list>
#include <cstring>
#include <string>
#include <cstdlib>
#include <limits>
#include <float.h>
#define FORMAT "0000-00-00"
#define HEADLINE "date | value"

class BitcoinExchange
{
private:
	int year;
	int month;
	int day;
	float value;
	float course;
	const std::string filename;
	std::list<std::string> fileData;
	std::list<std::string> csvData;
	std::list<std::string>::iterator it;

public:
										BitcoinExchange(std::string _filename);
										BitcoinExchange(const BitcoinExchange &src);
										~BitcoinExchange();

	BitcoinExchange						&operator=(const BitcoinExchange &src);

	int									getYear()const;
	int									getMonth()const;
	int									getDay()const;
	float								getValue()const;
	float								getCourse()const;
	const std::string					getFilename()const;
	std::list<std::string>				getFileData()const;
	std::list<std::string>				getCSVData()const;
	std::list<std::string>::iterator	getIterator()const;

	void								rightFormat(std::list<std::string>	&list);
	std::list<std::string>::iterator	findClosest(std::string date);
	float								findValueCSV(std::string date);
	float								calculateCourse(std::string current);
	void								printList(std::list<std::string> &list);
	int									readFile();
	void								badInput(std::string &complete);
	int									checkDay();
	void								setDate(std::string temp, int &i, std::string &date);
	float								checkValue(std::string &complete, int i);
	float								calculateMultipliedCourse();
	
	void								dataInit(std::list<std::string> &list);

	void								BitCoinExchangeMain();
	class WrongFormatException : public std::exception
	{
		public:
			const char* what() const throw();
	};
};
	