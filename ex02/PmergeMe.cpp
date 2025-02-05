/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:00:33 by ehedeman          #+#    #+#             */
/*   Updated: 2025/02/05 15:37:35 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

/*------------------------------de/-constructors------------------------------*/

PmergeMe::PmergeMe(const int _argc, char **_argv): argv(_argv), \
	argc(_argc), odd_number(-1)
{
	gettimeofday(&start, NULL);
	getSorted();
}

PmergeMe::~PmergeMe()
{
}

/*------------------------------central function------------------------------*/

void							PmergeMe::getSorted()
{
	readArgs();
	printContainer(this->_s, 1);
	gettimeofday(&end_data_management, NULL);
	mergeInsertionSort(this->_deque, 1);
	printContainer(this->_deque, 0);
	// mergeInsertionSort(this->_vec, 1);
	printTime("[...]");
}

/*------------------------------input processing -----------------------------*/

void		PmergeMe::readArgs()
{
	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		for (size_t j = 0; j < arg.length(); j++)
		{
			if (!isdigit(arg[j]))
				throw InvalidInputException();
		}
		char			**ptr = new char *;
		unsigned int	_new = std::strtof(arg.c_str(), ptr);
		if (**ptr && (strlen(*ptr) != 1))
			throw UnableToGetNumberException();
		this->_s.push_back(_new);
	}
	if (_s.size() < 2)
		return ;
	std::vector<unsigned int>::iterator it = _s.begin();
	if (_s.size() % 2)
		odd_number = *(_s.end() - 1);
	while (it != _s.end())
	{
		std::pair<unsigned int, unsigned int> _pair;
		unsigned int nbr;
		if (it + 1 != _s.end())
		{
			nbr = *it;
			_deque.push_back(nbr);
			_vec.push_back(nbr);
			_pair.first = *it;
			it++;
			nbr = *it;
			_pair.second = *it;
			// pair_deque.push_back(_pair);
			// pair_vec.push_back(_pair);
			_deque.push_back(nbr);
			_vec.push_back(nbr);
		}
		it++;
	}
}

/*------------------------------sorting functions-----------------------------*/

template <typename T> 
void debugPrint(T &container, std::string type)
{
	typename T::iterator it = container.begin();
	std::cout << type;
	while (it != container.end())
	{
		std::pair<unsigned int, unsigned int> pair = *it;
		std::cout << "["<<pair.first<<","<<pair.second<<"] ";
		it++;
	}
	std::cout << std::endl;
}

void						PmergeMe::mergeInsertionSort\
	(std::vector<unsigned int> &array, int sorting_level)
{
	if (!array.size())
		return ;
	sorting_level++;
	//debugPrint(array, "vector: ");
//	std::vector<unsigned int>::iterator it = array.begin();
	// while (it != array.end())
	// {
	// 	//std::pair<unsigned int, unsigned int> pair = *it;
	// 	if (!isNumbersSorted(pair))
	// 		swapNumbers(*it);
	// 	it++;
	// }
	//debugPrint(array, "vector: ");
	printTime("vector");
}

void						PmergeMe::mergeInsertionSort (std::deque<unsigned int> &array, int sorting_level)
{
	int pair_units = array.size() / sorting_level;
	if (pair_units < 2)
		return ;
	
	std::deque<unsigned int>::iterator start = array.begin();
	std::deque<unsigned int>::iterator last = next(array.begin(), sorting_level * (pair_units));
	std::deque<unsigned int>::iterator end = next(last, -(sorting_level));

	int steps = 2 * sorting_level;
	std::deque<unsigned int>::iterator	i = start;
	size_t								segfault_check = 0;
	while (i != end)
	{
		std::deque<unsigned int>::iterator _this = next(i, sorting_level - 1);
		std::deque<unsigned int>::iterator _next = next(i, sorting_level * 2 - 1);
		if (*_this > *_next)
			swapPairs(_this, sorting_level);
		if (segfault_check + steps >= array.size())
			break ;
		segfault_check += steps;
		std::advance(i, steps);
	}
	mergeInsertionSort(array, sorting_level * 2);
}

/*-----------------------------helper functions-------------------------------*/

template <typename T> void sortTest(T start, T end)
{
	unsigned int _start;
	unsigned int _start1;
	while (start != end)
	{
		_start = *start;
		_start1 = *(start + 1);
		if (_start > _start1)
		{
			unsigned int save = _start;
			_start = _start1;
			_start1 = save;
		}
		start++;
	}
	std::cout << _start1 << " " << _start << std::endl;
}

template <typename T>  T	PmergeMe::next(T start, size_t steps)
{
	std::advance(start, steps);
	return (start);
}

template <typename T> void	PmergeMe::swapPairs(T it, int PL)
{
	T start = next(it, -PL + 1);
	T end = next(start, PL);

	while (start != end)
	{
		std::iter_swap(start, next(start, PL));
		start++;
	}
}

template <typename T>	bool	PmergeMe::isSorted(T first, T second)
{
	if (first > second)
		return false;
	return true;
}

void						PmergeMe::printTime(std::string type)
{
	timeval now;
	gettimeofday(&now, NULL);
	std::cout								<<
	"time to process a range of: "			<<	this->_s.size()	<<
	" elements with std::"	+	type 	+		" : "			<<
	(now.tv_usec - this->start.tv_usec)		<<	" us "			<<
	std::endl;
}

template <typename T> void	PmergeMe::printContainer(T &container, \
	int mode)
{
	typename T::iterator it = container.begin();

	if (mode)
		std::cout << "Before: ";
	else
		std::cout << "After: ";
	// if (container.size() < 6)
	// {
	while (it != container.end())
	{
		std::cout << *it << " ";
		it++;
	}
	// }
	// else
	// {
	// 	for (size_t i = 0; i < 5; i++)
	// 	{
	// 		std::cout << *it << " ";
	// 		it++;
	// 	}
	// 	std::cout << "[...]";
	// }
	std::cout << std::endl;
}

// template <typename T> void	PmergeMe::sortByLevel(T &container, int pairlevel)
// {
// 	typename T::iterator	it = container.begin();
// 	int						steps = 1;			//so we can jump to the next pair
// 	size_t					count = 0;
// 	while (pairlevel)
// 	{
// 		it = container.begin();
// 		while (count <= (container.size() / steps))
// 		{
// 			std::pair<unsigned int, unsigned int> pair1 = *it;
// 			std::pair<unsigned int, unsigned int> pair2 = *(it + 1);
// 			if (!isPairsSorted(pair1, pair2))
// 				swapPairs(*it, *(it + 1));
// 			count++;
// 		}
// 		steps++;
// 		pairlevel--;
// 	}
// 	it = container.begin();
// 	while (it != container.end())
// 	{
// 		std::pair<unsigned int, unsigned int> pair = *it;
// 		_s.push_back(pair.first);	
// 		_s.push_back(pair.second);
// 		it++;
// 	}
// }

// template <typename T> void	PmergeMe::swapNumbers(std::pair<T, T> &pair)
// {
// 	unsigned int save = pair.first;
// 	pair.first = pair.second;
// 	pair.second = save;
// }

// template <typename T>	bool	PmergeMe::isPairsSorted(std::pair<T, T> 
// 	&pair1,std::pair<T, T> &pair2)
// {
// 	if (pair1.first > pair2.first)
// 		return false;
// 	return true;
// }

// template <typename T> int	PmergeMe::getPairLevel(T &container)
// {
// 	size_t i = container.size();
// 	int count = 0;
// 	while (i)
// 	{
// 		i /= 2;
// 		count++;
// 	}
// 	return (count);
// }

/*-----------------------------Custom Exceptions------------------------------*/

const char		*PmergeMe::InvalidInputException::what() const throw()
{
	return "Error: Invalid input.";
}

const char		*PmergeMe::UnableToGetNumberException::what() const throw()
{
	return "Error: Unable to get number from input.";
}