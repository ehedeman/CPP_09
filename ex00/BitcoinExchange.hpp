/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:31:33 by ehedeman          #+#    #+#             */
/*   Updated: 2025/01/30 15:46:31 by ehedeman         ###   ########.fr       */
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

	std::list<std::string>::iterator	findClosest(std::string date);
	float								findValueCSV(std::string date);
	float								calculateCourse(std::string current);//course
	void								printList(std::list<std::string> &list);
	int									readFile();//filestuff
	void								badInput(std::string &complete);
	int									checkDay();
	void								setDate(std::string temp, int &i, std::string &date);
	float								checkValue(std::string &complete, int i);
	float								calculateMultipliedCourse();
	
	void								dataInit(std::list<std::string> &list);

	void								BitCoinExchangeMain();	//date()
};
	