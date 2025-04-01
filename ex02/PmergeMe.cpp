/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:00:33 by ehedeman          #+#    #+#             */
/*   Updated: 2025/04/01 13:48:31 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

/*------------------------------de/-constructors------------------------------*/

PmergeMe::PmergeMe(const int _argc, char **_argv): argv(_argv), \
	argc(_argc), odd_number(-1)
{
	getSorted();
}

PmergeMe::~PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe &src): argv(src.getArgv()), \
	argc(src.getArgc()), odd_number(-1)
{
	*this = src;
}

PmergeMe		&PmergeMe::operator=(const PmergeMe &src)
{
	if (this == &src)
		return *this;
	return *this;
}

/*----------------getter\setter nessecary for copy constrcutor----------------*/
char			**PmergeMe::getArgv()const{return (this->argv);}
int				PmergeMe::getArgc()const{return(this->argc);}

/*------------------------------central function------------------------------*/

void			PmergeMe::getSorted()
{
	readArgs();
	std::string str = "\0";
	printContainer(this->_s, 1, str, DEBUG_OFF);
	
	/*--------------------------------Vector----------------------------------*/

	gettimeofday(&t_vec_start, NULL);
	mergeInsertionSort(this->_vec, 1);

	/*---------------------------------Deque----------------------------------*/
	gettimeofday(&t_deq_start, NULL);
	mergeInsertionSort(this->_deque, 1);

	/*---------------------------------Print----------------------------------*/

	printContainer(this->_vec, 0, "vector", DEBUG_OFF);
	//printContainer(this->_deque, 0, "deque", DEBUG_OFF);
	printTime("vector", t_vec_start, t_vec_end);
	printTime("deque", t_deq_start, t_deq_end);

	/*--------------------------------Debug-----------------------------------*/

	std::cout << std::endl << "Full containers sorted: " << std::endl;
	printContainer(this->_deque, 0, "deque", DEBUG_ON);
	printContainer(this->_vec, 0, "vector", DEBUG_ON);
}

/*------------------------------input processing -----------------------------*/

void			PmergeMe::checkDouble(unsigned int arg)
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

void			PmergeMe::readArgs()
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

/*-----------------------------template functions-----------------------------*/

template <typename T>  T	next_iterator(T start, int steps)
{
	std::advance(start, steps);
	return (start);
}

template <typename T> bool _operator(T lv, T rv) { return *lv < *rv; }

long PmergeMe::_jn(long n) { return round((pow(2, n + 1) + pow(-1, n)) / 3); }

template <typename T> void	PmergeMe::swapPairs(T it, int PL)
{
	T start = next_iterator(it, -PL + 1);
	T end = next_iterator(start, PL);

	while (start != end)
	{
		std::iter_swap(start, next_iterator(start, PL));
		start++;
	}
}

template <typename T>	bool	PmergeMe::isSorted(T first, T second)
{
	if (first > second)
		return false;
	return true;
}

void						PmergeMe::printTime(std::string type, timeval start, timeval end)
{
	std::cout								<<
	"time to process a range of: "			<<	this->_s.size()	<<
	" elements with std::"	+	type 	+		" : "			<<
	(end.tv_usec - start.tv_usec)		<<	" us "			<<
	std::endl;
}

template <typename T> void	PmergeMe::printContainer(T &array, \
	int mode, std::string type, int debug)
{
	typename T::iterator it = array.begin();

	if (mode)
		std::cout << "Before: ";
	else
		std::cout << "After: ";
	if (array.size() < 6 || debug == DEBUG_ON)
	{
		while (it != array.end())
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
	std::cout << " " + type << std::endl;
}

/*------------------------------vector functions-----------------------------*/

void	PmergeMe::mergeInsertionSort(std::vector<unsigned int> &array, \
	int sorting_level)
{
	typedef std::vector<unsigned int>::iterator Iterator;

	int pair_units = array.size() / sorting_level;
	if (pair_units < 2)
		return ;

	bool is_odd = pair_units % 2 == 1;

	Iterator start = array.begin();
	Iterator last = next_iterator(array.begin(), sorting_level * (pair_units));
	Iterator end = next_iterator(last, -(is_odd * sorting_level));

	int			steps = 2 * sorting_level;


	for (Iterator it = start; it != end; std::advance(it, steps))
	{
		Iterator _this = next_iterator(it, sorting_level - 1);
		Iterator _next = next_iterator(it, sorting_level * 2 - 1);
		if (*_this > *_next)
		{
			swapPairs(_this, sorting_level);
		}
	}

	mergeInsertionSort(array, sorting_level * 2);


	std::vector<Iterator> main;
	std::vector<Iterator> to_append;

	insert(array, main, to_append, sorting_level, pair_units, is_odd, end);
	_copy(array, sorting_level, main);
	gettimeofday(&t_vec_end, NULL);
}

void		PmergeMe::insert(std::vector<unsigned int> &array, \
	std::vector<std::vector<unsigned int>::iterator> &main, \
	std::vector<std::vector<unsigned int>::iterator> &to_append, \
	int sorting_level, int pair_units, bool is_odd, \
	std::vector<unsigned int>::iterator end)
{
	typedef std::vector<unsigned int>::iterator Iterator;
	main.insert(main.end(), next_iterator(array.begin(), sorting_level - 1));
	main.insert(main.end(), next_iterator(array.begin(), sorting_level * 2 - 1));
	
	for (int i = 4; i <= pair_units; i += 2)
	{
		to_append.insert(to_append.end(), next_iterator(array.begin(), \
			sorting_level * (i - 1) - 1));
		main.insert(main.end(), next_iterator(array.begin(), sorting_level * i - 1));
	}

	is_odd = is_odd;
	end = end;
	// if (is_odd)
	// 	to_append.insert(to_append.end(), next_iterator(end, sorting_level - 1));
	
	jacobsthal_number(main, to_append);

	for (size_t i = 0; i < to_append.size(); i++)
	{
		typename std::vector<Iterator>::iterator _cp = \
			next_iterator(to_append.begin(), i);
		typename std::vector<Iterator>::iterator _cb = \
			next_iterator(main.begin(), main.size() - to_append.size() + i);
		typename std::vector<Iterator>::iterator idx = \
			std::upper_bound(main.begin(), _cb, *_cp, _operator<Iterator>);
		main.insert(idx, *_cp);
	}
}

void PmergeMe::_copy(std::vector<unsigned int> &array, int sorting_level, \
	std::vector<std::vector<unsigned int>::iterator> &main)
{
	typedef std::vector<unsigned int>::iterator Iterator;
	std::vector<unsigned int> copy;

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
	Iterator array_it = array.begin();
	Iterator copy_it = copy.begin();
	while (copy_it != copy.end())
	{
		*array_it = *copy_it;
		array_it++;
		copy_it++;
	}
}

void	PmergeMe::jacobsthal_number(std::vector<std::vector<unsigned int>::iterator> &main, \
	std::vector<std::vector<unsigned int>::iterator> &to_append)
{
	typedef std::vector<unsigned int>::iterator Iterator;

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
		std::vector<Iterator>::iterator pendIt = \
			next_iterator(to_append.begin(), _jnd - 1);
		std::vector<Iterator>::iterator bountIt = \
			next_iterator(main.begin(), _cjn + _in);
		while (_not)
		{
			std::vector<Iterator>::iterator idx = \
				std::upper_bound(main.begin(), bountIt, *pendIt,\
					 _operator<Iterator>);
				std::vector<Iterator>::iterator inserted = \
				main.insert(idx, *pendIt);
			_not--;
			pendIt = to_append.erase(pendIt);
			std::advance(pendIt, -1);
	
			_os += (inserted - main.begin() == _cjn + _in);
			bountIt = next_iterator(main.begin(), _cjn + _in - _os);
		}
		_pjn = _cjn;
		_in += _jnd;
		_os = 0;
	}
}

/*-------------------------------deque functions-----------------------------*/

void		PmergeMe::mergeInsertionSort(std::deque<unsigned int> &array, \
	int sorting_level)
{
	typedef std::deque<unsigned int>::iterator Iterator;

	int pair_units = array.size() / sorting_level;
	if (pair_units < 2)
		return ;

	bool is_odd = pair_units % 2 == 1;
	Iterator start = array.begin();
	Iterator last = next_iterator(array.begin(), sorting_level * (pair_units));
	Iterator end  = next_iterator(last, -(is_odd * sorting_level));

	int			steps = 2 * sorting_level;

	
	for (Iterator it = start; it != end; std::advance(it, steps))
	{
		Iterator _this = next_iterator(it, sorting_level - 1);
		Iterator _next = next_iterator(it, sorting_level * 2 - 1);
		if (*_this > *_next)
		{
			swapPairs(_this, sorting_level);
		}
	}
	mergeInsertionSort(array, sorting_level * 2);

	std::deque<Iterator> main;
	std::deque<Iterator> to_append;

	insert(array, main, to_append, sorting_level, pair_units, is_odd, end);
	_copy(array, sorting_level, main);
	gettimeofday(&t_deq_end, NULL);
}

void		PmergeMe::insert(std::deque<unsigned int> &array, std::deque<std::deque<unsigned int>::iterator> &main, \
	std::deque<std::deque<unsigned int>::iterator> &to_append, \
	int sorting_level, int pair_units, bool is_odd, std::deque<unsigned int>::iterator end)
{
	typedef std::deque<unsigned int>::iterator Iterator;

	main.insert(main.end(), next_iterator(array.begin(), sorting_level - 1));
	main.insert(main.end(), next_iterator(array.begin(), sorting_level * 2 - 1));
	
	for (int i = 4; i <= pair_units; i += 2)
	{
		to_append.insert(to_append.end(), next_iterator(array.begin(), \
			sorting_level * (i - 1) - 1));
		main.insert(main.end(), next_iterator(array.begin(), sorting_level * i - 1));
	}

	is_odd = is_odd;
	end = end;
	// if (is_odd)
	// 	to_append.insert(to_append.end(), next_iterator(end, sorting_level - 1));

	jacobsthal_number(main, to_append);

	for (size_t i = 0; i < to_append.size(); i++)
	{
		typename std::deque<Iterator>::iterator _cp = \
			next_iterator(to_append.begin(), i);
		typename std::deque<Iterator>::iterator _cb = \
			next_iterator(main.begin(), main.size() - to_append.size() + i);
		typename std::deque<Iterator>::iterator idx = \
			std::upper_bound(main.begin(), _cb, *_cp, _operator<Iterator>);
		main.insert(idx, *_cp);
	}
}

void PmergeMe::_copy(std::deque<unsigned int> &array, int sorting_level, \
	std::deque<std::deque<unsigned int>::iterator> &main)
{
	typedef std::deque<unsigned int>::iterator Iterator;
	std::deque<unsigned int> copy;

	for (typename std::deque<Iterator>::iterator it = main.begin(); \
		it != main.end(); it++)
	{
		for (int i = 0; i < sorting_level; i++)
		{
			Iterator pair_start = *it;
			std::advance(pair_start, -sorting_level + i + 1);
			copy.insert(copy.end(), *pair_start);
		}
	}
	Iterator array_it = array.begin();
	Iterator copy_it = copy.begin();
	while (copy_it != copy.end())
	{
		*array_it = *copy_it;
		array_it++;
		copy_it++;
	}
}

void	PmergeMe::jacobsthal_number(std::deque<std::deque<unsigned int>::iterator> &main, \
	std::deque<std::deque<unsigned int>::iterator> &to_append)
{
	typedef std::deque<unsigned int>::iterator Iterator;

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
		std::deque<Iterator>::iterator pendIt = \
			next_iterator(to_append.begin(), _jnd - 1);
		std::deque<Iterator>::iterator bountIt = \
			next_iterator(main.begin(), _cjn + _in);
		while (_not)
		{
			std::deque<Iterator>::iterator idx = \
					std::upper_bound(main.begin(), bountIt, *pendIt,\
					_operator<Iterator>);
			std::deque<Iterator>::iterator inserted = \
					main.insert(idx, *pendIt);
			_not--;
			pendIt = to_append.erase(pendIt);
			std::advance(pendIt, -1);
	
			_os += (inserted - main.begin() == _cjn + _in);
			bountIt = next_iterator(main.begin(), _cjn + _in - _os);
		}
		_pjn = _cjn;
		_in += _jnd;
		_os = 0;
	}
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