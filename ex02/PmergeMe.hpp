/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:00:25 by ehedeman          #+#    #+#             */
/*   Updated: 2025/02/05 15:33:46 by ehedeman         ###   ########.fr       */
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
#include <time.h>

class PmergeMe
{
private:
	// std::deque<std::pair<unsigned int, unsigned int> > 	pair_deque;		//sorting in pairs as deque
	// std::vector<std::pair<unsigned int, unsigned int> >	pair_vec;		//sorting in pairs as vector
	std::deque<unsigned int> _deque;					//sorting in pairs as deque
	std::vector<unsigned int> _vec;					//sorting in pairs as vector
	std::vector<unsigned int>							_s;			//for printing input and putting everything together in the end
	char												**argv;
	const int											argc;
	timeval												start;				//saving the starting time of the program
	timeval												end_data_management;//saves the end time of data processing
	int													odd_number;			//saves odd number for end if nessecary. else its -1

public:
	PmergeMe(const int _argc, char **_argv);
	PmergeMe(const PmergeMe &src);
	~PmergeMe();

	PmergeMe	&operator=(const PmergeMe &src);
	
	void							getSorted();
	void							readArgs();
	template <typename T>	void	printContainer(T &container, int mode);
	void							printTime(std::string type);
	void							mergeInsertionSort(std::vector<unsigned int>&array, int sorting_level);
	void							mergeInsertionSort(std::deque<unsigned int>&array, int sorting_level);
	// template <typename T>	void	swapNumbers(std::pair<T, T> &pair);
	template <typename T>	void	swapPairs(T it, int PL);
	template <typename T>	bool	isSorted(T first, T second);
	template <typename T>	T		next(T start, size_t steps);
	//template <typename T>	bool	isPairsSorted(std::pair<T, T> &pair1, std::pair<T, T> &pair2);
	// template <typename T> int	getPairLevel(T &container);
	// template <typename T> void	sortByLevel(T &container, int pairlevel);
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
};
