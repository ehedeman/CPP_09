/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:30:06 by ehedeman          #+#    #+#             */
/*   Updated: 2025/01/29 14:40:01 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"


void	CSV(std::list<std::string> &list, std::list<std::string> &csv)
{
	std::list<std::string>::iterator it;

	it = list.begin();
	while (it != list.end())
	{
		std::string temp = *it;
		std::string _new;
		size_t i = 0;
		while (i < temp.length())
		{
			while (temp[i] && isdigit(temp[i]))
			{
				_new += temp[i];
				i++;
			}
			if (!temp[i])
				break ;
			else if (temp[i] == '-' || temp[i] == ' ' || temp[i] == '|')
			{
				_new += ',';
				while (temp[i] && !isdigit(temp[i]))
					i++;
			}
			if (!isdigit(temp[i]))
				i++;
		}
		if (_new[0])
			csv.push_back(_new);
		it++;
	}
}

void	print_list(std::list<std::string> &list)
{
	std::list<std::string>::iterator it;

	it = list.begin();
	while (it != list.end())
	{
		std::cout << *it << std::endl; 
		it++;
	}
}

int	fileStuff(char *filename, std::list<std::string> &list, \
	std::list<std::string> &csv)
{
	std::fstream	file;
	std::string		buff;

	file.open(filename, std::ios::in);
	if (file.is_open())
	{
		while (std::getline(file, buff))
			list.push_back(buff);
		file.close();
		CSV(list, csv);
		print_list(list);
		std::cout << std::endl;
		print_list(csv);
	}
	else
	{
		std::cout << "Error: could not open file." << std::endl;
		return (1);
	}
	return (0);
}

void	bad_input(std::string &complete)
{
	std::string year;
	std::string month;
	std::string day;

	int i = 0;
	while (complete[i] && complete[i] != ',')
	{
		year += complete[i];
		i++;	
	}
	if (!complete[i])
		std::cout << "Error: bad input => " + complete;
	else
	{
		while (complete[i] && complete[i] != ',')
		{
			month += complete[i];
			i++;	
		}
		if (!complete[i])
			std::cout << "Error: bad input => " + complete;
		else
		{
			while (complete[i] && complete[i] != ' ')
			{
				day += complete[i];
				i++;
			}
			std::cout << "Error: bad input => " + year + "-" + month + "-" + day << std::endl; 
		}
	}
}

int check_day(int day, int month, int year)
{	
	if (day == 29 && month == 2 && (year % 4))		// 29. feburar und kein schaltjahr
		return (1);
	else if (day < 1)
		return (1);
	else if (day > 31)
		return (1);
	else if (day > 30 && !(month % 2))		// monat = feburat, april etc und tag == 31
		return (1);
	return (0);
}

void	set_date(std::string temp, int &i, std::string &date)
{
	while (temp[i] != ',')
	{
		date += temp[i];
		i++;
	}
}

void	date(std::list<std::string> &csv)
{
	std::list<std::string>::iterator it;
	int year;
	int month;
	int day;

	it = csv.begin();
	while (it != csv.end())
	{
		std::string temp = *it;
		std::string s_year;
		std::string s_month;
		std::string s_day;
		int i = 0;

		set_date(temp, i, s_year);
		set_date(temp, i, s_month);
		set_date(temp, i, s_day);
	
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
		if (year < 2009 || year > 2024)
			bad_input(*it);
		if (month < 1 || month > 12)
			bad_input(*it);
		if (check_day(day, month, year))
			bad_input(*it);
		std::cout << s_year + "-" + s_month + "-" + s_day<< std::endl; 
		it++;
	}
	
}



void	value(std::list<std::string> &csv)
{
	(void)csv;
}

int main(int argc, char **argv)
{
	std::list<std::string> list;
	std::list<std::string> csv;
	if (argc != 2)
	{
		std::cout << "Invalid argument number." << std::endl;
		return (0);
	}
	char  *filename = argv[1];
	if (fileStuff(filename, list, csv))
		return (0);
	date(csv);
	value(csv);
}