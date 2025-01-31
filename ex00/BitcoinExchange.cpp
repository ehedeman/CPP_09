/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:31:14 by ehedeman          #+#    #+#             */
/*   Updated: 2025/01/31 11:07:37 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:31:33 by ehedeman          #+#    #+#             */
/*   Updated: 2025/01/30 15:01:43 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"


BitcoinExchange::BitcoinExchange(std::string _filename): filename(_filename)
{
	this->year  	= 0;
	this->month 	= 0;
	this->day		= 0;
	this->value 	= 0;
	this->course	= 0;
	this->BitCoinExchangeMain();
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &src): filename(src.getFilename())
{
	*this = src;
}

BitcoinExchange		&BitcoinExchange::operator=(const BitcoinExchange &src)
{
	if (this == &src)
		return (*this);
	else
	{	
		this->year		= src.getYear();
		this->month		= src.getMonth();
		this->day		= src.getDay();
		this->value		= src.getValue();
		this->course	= src.getCourse();
		this->fileData	= src.getFileData();
		this->csvData	= src.getCSVData();
	}
	return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
}



int									BitcoinExchange::getYear()const{return (this->year);}
int									BitcoinExchange::getMonth()const{return (this->month);}
int									BitcoinExchange::getDay()const{return (this->day);}
float								BitcoinExchange::getValue()const{return (this->value);}
float								BitcoinExchange::getCourse()const{return (this->course);}
const std::string					BitcoinExchange::getFilename()const{return (this->filename);}
std::list<std::string>				BitcoinExchange::getFileData()const{return (this->fileData);}
std::list<std::string>				BitcoinExchange::getCSVData()const{return (this->csvData);}
std::list<std::string>::iterator	BitcoinExchange::getIterator()const{return (this->it);}


void								BitcoinExchange::rightFormat(std::list<std::string>	&list)
{
	std::list<std::string>::iterator iterator = list.begin();

	std::string str = *iterator;
	for (size_t i = 0; i < strlen(HEADLINE); i++)
	{
		if (!str[i])
			throw WrongFormatException();
		else if (str[i] != HEADLINE[i])
			throw WrongFormatException();
	}
	iterator++;
	while (iterator != list.end())
	{
		std::string str = *iterator;
		for (size_t i = 0; i < strlen(FORMAT); i++)
		{
			if (!str[i])
				throw WrongFormatException();
			else if (i < 4 && !isdigit(str[i]))
				throw WrongFormatException();
			else if ((i > 4 && i < 7) && !isdigit(str[i]))
				throw WrongFormatException();
			else if ((i > 7 && i < 10) && !isdigit(str[i]))
				throw WrongFormatException();
			else if (i > 9 && (str[i] != FORMAT[i]))
				throw WrongFormatException();
		}
		iterator++;
	}
}

std::list<std::string>::iterator	BitcoinExchange::findClosest(std::string date)
{
	std::list<std::string>::iterator closest_ptr = this->csvData.begin();
	std::list<std::string>::iterator iterator = this->csvData.begin();
	int closest_save = 0;
	
	closest_ptr++;
	while (iterator != this->csvData.end())
	{
		std::string str = *iterator;
		int			closest = 0;
		size_t			i = 0;
		while (i < date.length() && date[i] == str[i])
		{
			closest++;
			i++;
		}
		if (closest > closest_save)
		{
			closest_save = closest;
			closest_ptr = iterator;	
		}
		else if (closest == closest_save)
		{
			if (date[i] > str[i])
				closest_ptr = iterator;
		}
		iterator++;
	}
	return (closest_ptr);
}

float	BitcoinExchange::findValueCSV(std::string date)
{
	std::list<std::string>::iterator iterator = this->csvData.begin();
	std::string _value;
	std::string entry;
	date = date.substr(0, 10);
	while (iterator != this->csvData.end())
	{
		if (date == *iterator)
			break ;
		iterator++;
	}
	if (iterator == this->csvData.end())
		entry = *this->findClosest(date);
	else
		entry = *iterator;
	for (size_t i = 11; i < entry.length(); i++)	//value starts after i = 10
		_value += entry[i];
	char	**ptr = new char *;
	float _new_value = std::strtof(_value.c_str(), ptr);
	if (**ptr && (strlen(*ptr) != 1))
	{
		std::cout << "error" << std::endl;
		return (-1);
	}
	_new_value = this->value * _new_value;
	return (_new_value);
}

float	BitcoinExchange::calculateCourse(std::string current)
{
	current = current.substr(0, 10);
	for (size_t i = 0; i < current.length(); i++)
	{
		if (current[i] == ',')
			current[i] = '-';
	}
	float csv_value = this->findValueCSV(current);
	return csv_value;
}
//course
void	BitcoinExchange::printList(std::list<std::string> &list)
{
	this->it = list.begin();
	while (this->it != list.end())
	{
		std::cout << *this->it << std::endl; 
		this->it++;
	}
}


void	BitcoinExchange::badInput(std::string &complete)
{
	std::string _year;
	std::string _month;
	std::string _day;

	int i = 0;
	while (complete[i] && complete[i] != ',')
	{
		_year += complete[i];
		i++;	
	}
	if (!complete[i])
		std::cout << "Error: bad input => " + complete << std::endl;	//macht sinn
	else
	{
		i++;		//skip komma lol
		while (complete[i] && complete[i] != ',')
		{
			_month += complete[i];
			i++;	
		}
		if (!complete[i])
			std::cout << "Error: bad input => " + _year <<
			"-" + this->month << std::endl;	//das nicht so
		else
		{
			i++;		//skip komma lol
			while (complete[i] && complete[i] != ',')
			{
				_day += complete[i];
				i++;
			}
			std::cout << "Error: bad input => " + _year << "-" + _month <<
			"-" + _day << std::endl; 
		}
	}
}


int		BitcoinExchange::checkDay()
{
	if (this->day == 29 && this->month == 2 && (this->year % 4))		// 29. feburar und kein schaltjahr
		return (1);
	else if (this->day < 1)
		return (1);
	else if (this->day > 31)
		return (1);
	else if (this->day > 30 && !(this->month % 2))		// monat = feburat, april etc und tag == 31
		return (1);
	return (0);
}
void	BitcoinExchange::setDate(std::string temp, int &i, std::string &date)
{
	while (temp[i] && temp[i] != ',')
	{
		date += temp[i];
		i++;
	}
	i++; //damit es nicht auf komma endet
}

float	BitcoinExchange::checkValue(std::string &complete, int i)
{
	std::string str_value;

	while (complete[i])
	{
		str_value += complete[i];
		i++;
	}
	char	**ptr = new char *;
	float _value = std::strtof(str_value.c_str(), ptr);
	if (**ptr && (strlen(*ptr) != 1))
	{
		std::cout	<< "error";
		return -1;
	}
	if (_value < 0)
	{
		std::cout << "Error: negative value." << std::endl;
		return -1;
	}
	else if (_value > 1000)
	{
		std::cout << "Error: value too large." << std::endl;
		return -1;
	}
	return (_value);
}
int		BitcoinExchange::readFile()//filestuff
{
	std::list<std::string>	list;
	std::fstream			file;
	std::string				buff;

	file.open("data.csv", std::ios::in);
	if (file.is_open())
	{
		while (std::getline(file, buff))
			this->csvData.push_back(buff);
		file.close();
	}
	else
	{
		std::cout << "Error: could not open file." << std::endl;
		return (-1);
	}
	file.open(this->filename.c_str(), std::ios::in);
	if (file.is_open())
	{
		while (std::getline(file, buff))
			list.push_back(buff);
		file.close();
		rightFormat(list);
		this->dataInit(list);
		// this->printList(list);
		// std::cout << std::endl;
		// this->printList(this->fileData);
		// std::cout << std::endl;
		// this->printList(this->csvData);
	}
	else
	{
		std::cout << "Error: could not open file." << std::endl;
		return (1);
	}
	return (0);
}


void		BitcoinExchange::dataInit(std::list<std::string> &list)
{
	this->it = list.begin();
	this->it++;					//to skip the "date | value"

	while (this->it != list.end())
	{
		std::string temp = *this->it;
		std::string _new;
		size_t i = 0;
		int decimal = 0;

		while (i < temp.length())
		{
			while (temp[i])
			{
				if (temp[i] == '.' && !decimal)
				{
					_new += temp[i];
					decimal++;
					i++;
				}
				else if (!isdigit(temp[i]))
					break ;
				_new += temp[i];
				i++;
			}
			if (!temp[i])
				break ;
			else if (temp[i] == '-' || temp[i] == ' ' || temp[i] == '|')
			{
				_new += ',';
				while (temp[i] && !isdigit(temp[i]))
				{
					if (temp[i] == '|' && temp[i + 2]  == '-' && isdigit(temp[i + 3]))
						_new += '-';
					i++;
				}
			}
			if (!isdigit(temp[i]))
				i++;
		}
		if (_new[0])
			this->fileData.push_back(_new);
		it++;
	}
}

void	BitcoinExchange::BitCoinExchangeMain()
{
	if (this->readFile())
			return ;
	it = this->fileData.begin();
	while (it != this->fileData.end())
	{
		std::string temp = *it;
		std::string s_year;
		std::string s_month;
		std::string s_day;
		int i = 0;
		
		this->setDate(temp, i, s_year);
		this->setDate(temp, i, s_month);
		this->setDate(temp, i, s_day);
		char	**ptr = new char *;
		year = (int)std::strtod(s_year.c_str(), ptr);
		if (**ptr && (strlen(*ptr) != 1))
			std::cout	<< "error";
		month = (int)std::strtod(s_month.c_str(), ptr);
		if (**ptr && (strlen(*ptr) != 1))
			std::cout	<< "error";
		day = (int)std::strtod(s_day.c_str(), ptr);
		if (**ptr && (strlen(*ptr) != 1))
			std::cout	<< "error";
		if (year < 2009 || year > 2022)
			this->badInput(*it);
		else if (month < 1 || month > 12)
			this->badInput(*it);
		else if (this->checkDay())
			this->badInput(*it);
		else if (this->checkValue(temp, i) >= 0)
		{
			this->value = this->checkValue(temp, i);
			float _course =  this->calculateCourse(temp);
			if (_course < 0)
				continue ;
			std::cout << s_year << "-" << s_month << "-" << s_day <<
			" => " << this->value << " = " << _course << std::endl;
		}
		it++;
	}
}	//date()
	
const char* BitcoinExchange::WrongFormatException::what() const throw()
{
	return "Error: wrong format.";
}