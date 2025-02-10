/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:00:33 by ehedeman          #+#    #+#             */
/*   Updated: 2025/02/10 15:42:51 by ehedeman         ###   ########.fr       */
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
	mergeInsertionSort(this->_vec, 1);
	printTime("vector");
	mergeInsertionSort(this->_deque, 1);
	printTime("deque");
	printTime("[...]");
	printContainer(this->_deque, 0);
	
}

/*------------------------------input processing -----------------------------*/

void		PmergeMe::checkDouble(unsigned int arg)
{
	static std::vector<unsigned int> compare;
	std::vector<unsigned int>::iterator it = compare.begin();
	if (!compare.size())
	{
		compare.push_back(arg);
		return ;
	}
	while ( it != compare.end())
	{
		if (arg == *it)
			throw DoubleNumberException();
		it++;
	}
}

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
		checkDouble(_new);
	}
	if (_s.size() < 2)
		return ;
	std::vector<unsigned int>::iterator it = _s.begin();
	if (_s.size() % 2)
		odd_number = *(_s.end() - 1);
	while (it != _s.end())
	{
		unsigned int nbr;
		nbr = *it;
		_deque.push_back(nbr);
		_vec.push_back(nbr);
		it++;
	}
}

/*------------------------------helper functions-----------------------------*/

template <typename T>  T	next(T start, int steps)
{
	std::advance(start, steps);
	return (start);
}

template <typename T> bool _operator(T lv, T rv) { return *lv < *rv; }

long PmergeMe::_jn(long n) { return round((pow(2, n + 1) + pow(-1, n)) / 3); }

template <typename T> void PmergeMe::_copy(T &container, std::vector<int> &copy)
{
	typename T::iterator container_it = container.begin();
	std::vector<int>::iterator copy_it = copy.begin();
	while (copy_it != copy.end())
	{
		*container_it = *copy_it;
		container_it++;
		copy_it++;
	}
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
	if (container.size() < 6)
	{
		while (it != container.end())
		{
			std::cout << *it << " ";
			it++;
		}
	}
	else
	{
		for (size_t i = 0; i < 5; i++)
		{
			std::cout << *it << " ";
			it++;
		}
		std::cout << "[...]";
	}
	std::cout << std::endl;
}

/*-----------------------------sorting functions-------------------------------*/

template <typename T> void	PmergeMe::jacobsthal_number(std::vector<T> &main, \
	std::vector<T> &to_append)
{

	int _pjn = _jn(1);
	int	_in = 0;
	for (int k = 2;; k++)
	{
		int _cjn = _jn(k);
		int _jnd = _cjn - _pjn;
		int _os = 0;
		if (_jnd > static_cast<int>(to_append.size()))
			break ;
		int _not = _jnd;
		typename std::vector<T>::iterator pendIt = \
			next(to_append.begin(), _jnd - 1);
		typename std::vector<T>::iterator bountIt = \
			next(main.begin(), _cjn + _in);
		while (_not)
		{
			typename std::vector<T>::iterator idx = \
				std::upper_bound(main.begin(), bountIt, *pendIt,\
					 _operator<T>);
			typename std::vector<T>::iterator inserted = \
				main.insert(idx, *pendIt);
			_not--;
			pendIt = to_append.erase(pendIt);
			std::advance(pendIt, -1);
	
			_os += (inserted - main.begin() == _cjn + _in);
			bountIt = next(main.begin(), _cjn + _in - _os);
		}
		_pjn = _cjn;
		_in += _jnd;
		_os = 0;
	}
}


void	PmergeMe::mergeInsertionSort(std::vector<unsigned int> &array, \
	int sorting_level)
{
	typedef std::vector<unsigned int>::iterator Iterator;

	int pair_units = array.size() / sorting_level;
	if (pair_units < 2)
		return ;

	bool is_odd = pair_units % 2 == 1;

	Iterator start = array.begin();
	Iterator last = next(array.begin(), sorting_level * (pair_units));
	Iterator end = next(last, -(is_odd * sorting_level));

	int			steps = 2 * sorting_level;


	for (Iterator it = start; it != end; std::advance(it, steps))
	{
		Iterator _this = next(it, sorting_level - 1);
		Iterator _next = next(it, sorting_level * 2 - 1);
		if (*_this > *_next)
			swapPairs(_this, sorting_level);
	}

	mergeInsertionSort(array, sorting_level * 2);

	std::vector<Iterator> main;
	std::vector<Iterator> to_append;

	jacobsthal_number(main, to_append);

	main.insert(main.end(), next(array.begin(), sorting_level - 1));
	main.insert(main.end(), next(array.begin(), sorting_level * 2 - 1));
	
	for (int i = 4; i <= pair_units; i += 2)
	{
		to_append.insert(to_append.end(), next(array.begin(), \
			sorting_level * (i - 1) - 1));
		main.insert(main.end(), next(array.begin(), sorting_level * i - 1));
	}
		
	
		
	for (size_t i = 0; i < to_append.size(); i++)
	{
		typename std::vector<Iterator>::iterator _cp = \
			next(to_append.begin(), i);
		typename std::vector<Iterator>::iterator _cb = \
			next(main.begin(), main.size() - to_append.size() + i);
		typename std::vector<Iterator>::iterator idx = \
			std::upper_bound(main.begin(), _cb, *_cp, _operator<Iterator>);
		main.insert(idx, *_cp);
	}
		

	
	if (odd_number >= 0)
	{
		std::vector<unsigned int> temp;
		temp.push_back(odd_number);
		Iterator temp_it = temp.begin();
		typename std::vector<Iterator>::iterator idx = 
			std::upper_bound(main.begin(), main.end(), \
				temp_it, _operator<Iterator>);
		main.insert(idx, temp_it);
	}

	std::vector<int> copy;
	copy.reserve(array.size());
	for (typename std::vector<Iterator>::iterator it = main.begin(); \
		it != main.end(); it++)
	{
		for (int i = 0; i < sorting_level; i++)
		{
			Iterator pair_start = *it;
			std::advance(pair_start, -sorting_level + i + 1);
			copy.insert(copy.end(), *pair_start);
		}
	}
	_copy(array, copy);
}

void		PmergeMe::mergeInsertionSort(std::deque<unsigned int> &array, \
	int sorting_level)
{
	typedef std::deque<unsigned int>::iterator Iterator;

	int pair_units = array.size() / sorting_level;
	if (pair_units < 2)
		return ;

	bool is_odd = pair_units % 2 == 1;

	Iterator start = array.begin();
	Iterator last = next(array.begin(), sorting_level * (pair_units));
	Iterator end = next(last, -(is_odd * sorting_level));

	int			steps = 2 * sorting_level;


	for (Iterator it = start; it != end; std::advance(it, steps))
	{
		Iterator _this = next(it, sorting_level - 1);
		Iterator _next = next(it, sorting_level * 2 - 1);
		if (*_this > *_next)
			swapPairs(_this, sorting_level);
	}

	mergeInsertionSort(array, sorting_level * 2);

	std::vector<Iterator> main;
	std::vector<Iterator> to_append;

	main.insert(main.end(), next(array.begin(), sorting_level - 1));
	main.insert(main.end(), next(array.begin(), sorting_level * 2 - 1));
	
	for (int i = 4; i <= pair_units; i += 2)
	{
		to_append.insert(to_append.end(), next(array.begin(), \
			sorting_level * (i - 1) - 1));
		main.insert(main.end(), next(array.begin(), sorting_level * i - 1));
	}
		
	jacobsthal_number(main, to_append);
		
	for (size_t i = 0; i < to_append.size(); i++)
	{
		typename std::vector<Iterator>::iterator _cp = \
			next(to_append.begin(), i);
		typename std::vector<Iterator>::iterator _cb = \
			next(main.begin(), main.size() - to_append.size() + i);
		typename std::vector<Iterator>::iterator idx = \
			std::upper_bound(main.begin(), _cb, *_cp, \
				_operator<Iterator>);
		main.insert(idx, *_cp);
	}
		

	
	if (odd_number >= 0)
	{
		std::deque<unsigned int> temp;
		temp.push_back(odd_number);
		Iterator temp_it = temp.begin();
		typename std::vector<Iterator>::iterator idx = 
			std::upper_bound(main.begin(), main.end(), \
				temp_it, _operator<Iterator>);
		main.insert(idx, temp_it);
	}

	std::vector<int> copy;
	copy.reserve(array.size());
	for (typename std::vector<Iterator>::iterator it = main.begin(); \
		it != main.end(); it++)
	{
		for (int i = 0; i < sorting_level; i++)
		{
			Iterator pair_start = *it;
			std::advance(pair_start, -sorting_level + i + 1);
			copy.insert(copy.end(), *pair_start);
		}
	}
	_copy(array, copy);
}

/*-----------------------------Custom Exceptions------------------------------*/

const char		*PmergeMe::InvalidInputException::what() const throw()
{
	return "Error: Invalid input.";
}

const char		*PmergeMe::UnableToGetNumberException::what() const throw()
{
	return "Error: Unable to get number from input.";
}

const char		*PmergeMe::DoubleNumberException::what() const throw()
{
	return "Error: Double value.";
}