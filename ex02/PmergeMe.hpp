/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:00:25 by ehedeman          #+#    #+#             */
/*   Updated: 2025/03/27 14:30:58 by ehedeman         ###   ########.fr       */
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

#define DEBUG_ON 1
#define DEBUG_OFF 0

class PmergeMe
{
private:
	std::deque<unsigned int>				_deque;				//sorting in pairs as deque
	std::vector<unsigned int>				_vec;				//sorting in pairs as vector
	std::vector<unsigned int>				_s;					//for printing input and putting everything together in the end
	char									**argv;
	const int								argc;
	timeval									t_vec_start;
	timeval									t_vec_end;
	timeval									t_deq_start;
	timeval									t_deq_end;
	int										odd_number;			//saves odd number for end if nessecary. else its -1
	
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
	template <typename T>	void	printContainer(T &container, int mode, std::string type, \
									int debug);
	void							printTime(std::string type, timeval start, timeval end);
	void 							_copy(std::vector<unsigned int> &array, int sorting_level, \
										std::vector<std::vector<unsigned int>::iterator> &main);
	void							_copy(std::deque<unsigned int> &array, int sorting_level, \
										std::deque<std::deque<unsigned int>::iterator> &main);
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
	void							jacobsthal_number(std::vector<std::vector<unsigned int>::iterator> &main, \
										std::vector<std::vector<unsigned int>::iterator> &to_append);
	void							jacobsthal_number(std::deque<std::deque<unsigned int>::iterator> &main, \
										std::deque<std::deque<unsigned int>::iterator> &to_append);									
	void							insert(std::vector<unsigned int> &array, std::vector<std::vector<unsigned int>::iterator> &main, \
										std::vector<std::vector<unsigned int>::iterator> &to_append, \
										int sorting_level, int pair_units, bool is_odd, \
										std::vector<unsigned int>::iterator end);
	void							insert(std::deque<unsigned int> &array, std::deque<std::deque<unsigned int>::iterator> &main, \
										std::deque<std::deque<unsigned int>::iterator> &to_append, \
										int sorting_level, int pair_units, bool is_odd, \
										std::deque<unsigned int>::iterator);
	template <typename T>  T		next(T start, int steps);

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

