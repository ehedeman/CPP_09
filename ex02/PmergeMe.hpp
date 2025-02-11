/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:00:25 by ehedeman          #+#    #+#             */
/*   Updated: 2025/02/11 12:07:46 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
#include <cstring>
#include <sys/time.h>
#include <ctime>
#include <cmath>
#include <time.h>

class PmergeMe
{
private:
	std::deque<unsigned int>		_deque;				//sorting in pairs as deque
	std::vector<unsigned int>		_vec;				//sorting in pairs as vector
	std::vector<unsigned int>		_s;					//for printing input and putting everything together in the end
	char							**argv;
	const int						argc;
	timeval							start;				//saving the starting time of the program
	timeval							end_data_management;//saves the end time of data processing
	int								odd_number;			//saves odd number for end if nessecary. else its -1

public:
									PmergeMe(const int _argc, char **_argv);
									PmergeMe(const PmergeMe &src);
									~PmergeMe();

	PmergeMe						&operator=(const PmergeMe &src);
	
	/*---------------------------getter\setter nessecary for copy constrcutor-------------------*/
	char							**getArgv()const;
	int								getArgc()const;
	/*----------------------------------------central function----------------------------------*/
	void							getSorted();
	
	/*----------------------------------------input processing ---------------------------------*/
	void							readArgs();
	
	/*---------------------------------------helper functions-----------------------------------*/
	template <typename T>	void	printContainer(T &container, int mode);
	void							printTime(std::string type);
	template <typename T> void		_copy(T &array, int sorting_level, \
									std::vector<typename T::iterator> &main);
	template <typename T>	void	swapPairs(T it, int PL);
	template <typename T>	bool	isSorted(T first, T second);
	void							checkDouble(unsigned int arg);
	long 							_jn(long n);

	/*--------------------------------------sorting functions-----------------------------------*/
	void							mergeInsertionSort(std::vector<unsigned int> &array, \
									int sorting_level);
	void							mergeInsertionSort(std::deque<unsigned int> &array, \
									int sorting_level);

	/*-----------------------------------sorting helper functions-------------------------------*/
	template <typename T> void		jacobsthal_number(std::vector<T> &main, \
									std::vector<T> &to_append);
	template <typename T> void		insert(T &array, std::vector<typename T::iterator> &main, \
									std::vector<typename T::iterator> &to_append, \
									int sorting_level, int pair_units);


	/*--------------------------------------Custom Exceptions-----------------------------------*/
	class InvalidInputException : public std::exception
	{
		public:
			const char* 			what() const throw();
	};
	class UnableToGetNumberException : public std::exception
	{
		public:
			const char* 			what() const throw();
	};
	class DoubleNumberException : public std::exception
	{
		public:
			const char* 			what() const throw();
	};
};
