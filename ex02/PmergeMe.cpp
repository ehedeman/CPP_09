/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:00:33 by ehedeman          #+#    #+#             */
/*   Updated: 2025/06/26 13:55:17 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

/*------------------------------de/-constructors------------------------------*/

PmergeMe::PmergeMe(const int _argc, char **_argv): argv(_argv), \
	argc(_argc)
{
	getSorted();
}

PmergeMe::~PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe &src): argv(src.getArgv()), \
	argc(src.getArgc())
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

	printContainer(this->_vec, 0, "", DEBUG_OFF); //subject wanted it like this
	// printContainer(this->_vec, 0, "vector", DEBUG_OFF);
	// printContainer(this->_deque, 0, "deque", DEBUG_OFF);
	printTime("vector", t_vec_start, t_vec_end);
	printTime("deque", t_deq_start, t_deq_end);

	/*--------------------------------Debug-----------------------------------*/

	// std::cout << std::endl << "Full containers sorted: " << std::endl;
	// printContainer(this->_deque, 0, "deque", DEBUG_ON);
	// printContainer(this->_vec, 0, "vector", DEBUG_ON);
	// std::cout << "Vector sorted: ";
	// if (checkSorted(_vec))
	// 	std::cout << "true" << std::endl;
	// std::cout << "Deque sorted: ";
	// if (checkSorted(_deque))
	// 	std::cout << "true" << std::endl;
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
	compare.push_back(arg);
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
		long nbr = strtol(arg.c_str(), NULL, 10);
    	if ((nbr == 0 && arg != "0") || nbr > INT_MAX || errno == ERANGE)
			throw InvalidInputException();
		unsigned int	_new = atoi(arg.c_str());
		checkDouble(_new);
		this->_s.push_back(_new);
	}
	// if (_s.size() < 2)
	// 	return ;
	std::vector<unsigned int>::iterator it = _s.begin();
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

long PmergeMe::_jacobsthal_nbr(long n) { return round((pow(2, n + 1) + pow(-1, n)) / 3); }

template <typename T> void	PmergeMe::swapPairs(T it, int sorting_level)
{
	T start = next_iterator(it, -sorting_level + 1);
	T end = next_iterator(start, sorting_level);

	while (start != end)
	{
		std::iter_swap(start, next_iterator(start, sorting_level));
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
	"Time to process a range of: "			<<	this->_s.size()	<<
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
	if (array.size() < 2)
	{
		gettimeofday(&t_vec_end, NULL);	
		return ;
	}
		typedef std::vector<unsigned int>::iterator Iterator;

	int pair_amount = array.size() / sorting_level;
	if (pair_amount < 2)
		return ;

	bool is_odd = pair_amount % 2 == 1;

	Iterator start = array.begin();
	Iterator last = next_iterator(array.begin(), sorting_level * (pair_amount));	
	//last number to process (not including possible odd pair/number)
	Iterator end = next_iterator(last, -(is_odd * sorting_level));
	//needs to be dependant on odd pair number otherwise segfault cuz it tries to jump to nonexistent memory
	int			steps = 2 * sorting_level; 
	//determining how many numbers to skip when moving on to next


	for (Iterator it = start; it != end; std::advance(it, steps))
	{
		Iterator _this = next_iterator(it, sorting_level - 1);
		Iterator _next = next_iterator(it, steps - 1);
		if (!isSorted(*_this, *_next))
		{
			swapPairs(_this, sorting_level);
		}
	}
	
	
	mergeInsertionSort(array, steps);

	std::vector<Iterator> main;
	std::vector<Iterator> to_append;

	insert(array, main, to_append, sorting_level, pair_amount, is_odd, end);
	_copy(array, sorting_level, main);
	gettimeofday(&t_vec_end, NULL);	
}

void		PmergeMe::insert(std::vector<unsigned int> &array, \
	std::vector<std::vector<unsigned int>::iterator> &main, \
	std::vector<std::vector<unsigned int>::iterator> &to_append, \
	int sorting_level, int pair_amount, bool is_odd, std::vector<unsigned int>::iterator end)
{
	typedef std::vector<unsigned int>::iterator Iterator;

	main.insert(main.end(), next_iterator(array.begin(), sorting_level - 1));
	main.insert(main.end(), next_iterator(array.begin(), sorting_level * 2 - 1));
	//takes the first two pairs
	for (int i = 4; i <= pair_amount; i += 2)
	{
		to_append.insert(to_append.end(), next_iterator(array.begin(), \
			sorting_level * (i - 1) - 1));
		main.insert(main.end(), next_iterator(array.begin(), sorting_level * i - 1));
	}

	//!!!main at this point is already sorted
	// to_append isnt
	

	if (is_odd)
    {
        to_append.insert(to_append.end(), next_iterator(end, sorting_level - 1));
    }
	
	jacobsthal_number(main, to_append);	//binary search sorting optimized with jacobsthal sequence 

	for (size_t i = 0; i < to_append.size(); i++)
	{
		typename std::vector<Iterator>::iterator _current_append = \
			next_iterator(to_append.begin(), i);
		typename std::vector<Iterator>::iterator _current_bound = \
			next_iterator(main.begin(), main.size() - to_append.size() + i + is_odd);
		typename std::vector<Iterator>::iterator idx = \
			std::upper_bound(main.begin(), _current_bound, *_current_append, _operator<Iterator>); 
			//_operator is the compare factor, so tells it what to do
		main.insert(idx, *_current_append);
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
//bound = grenze bzw. hier obergrenze
//wird immer wieder neu berechnet damit alles passt
void	PmergeMe::jacobsthal_number(std::vector<std::vector<unsigned int>::iterator> &main, \
	std::vector<std::vector<unsigned int>::iterator> &to_append)
{
	typedef std::vector<unsigned int>::iterator Iterator;

	int prev_j_nbr = _jacobsthal_nbr(1);	//gotta start somewhere so jn(1) as first previous
	int	_amount_inserts = 0;	//makes sense
	for (int k = 2;; k++)	//k = 2 because jn(1) is already the previous and k gets used as jn(k)
	{
		int _current_j_nbr = _jacobsthal_nbr(k);	//get the jn
		int _j_difference = _current_j_nbr - prev_j_nbr; //get the difference of currnet and prev (that will be the amount of inserted objects)
		int _offset = 0;	//offset = 0 cause new iteration of loop
		if (_j_difference > static_cast<int>(to_append.size())) // if the difference is larger than array (makes sense)
			break ;
		int _counter = _j_difference;	//we iterate counter times so as many times as the difference is large	
		std::vector<Iterator>::iterator pendIt = \
			next_iterator(to_append.begin(), _j_difference - 1);	//the iterator of pend gets set to the differnece -1
		std::vector<Iterator>::iterator bountIt = \
			next_iterator(main.begin(), _current_j_nbr + _amount_inserts);//iterator of bound gets set to curr jn + the amount of inserts we had so far
		
		// the jn helps finding the range of numbers to insert in one iteration.
		// that way there is an order in which to insert, which saves us comparisons
		// if we have e.g 1b 1a 2a as objects and we want to insert 3b, its much faster
		// than if we take the whole of 1b 1a 2a 3a 5a 2a 6a 7a because instead of comparing
		// 3b to all a objects in main we just need to compare it to the range we know it belongs
		// into. because of the nature of this algorithm we know that 3b is most likely larger than
		// 2a, but anything after 3a is bigger anyways.
	
		while (_counter)	//binary search insertion parrrttt
		{
			std::vector<Iterator>::iterator idx = \
				std::upper_bound(main.begin(), bountIt, *pendIt,\
					 _operator<Iterator>);	// upper_bound is a function that uses binary search
			std::vector<Iterator>::iterator inserted = \
				main.insert(idx, *pendIt);	//so binary search is being used to find where to insert
			_counter--;
			pendIt = to_append.erase(pendIt);
			std::advance(pendIt, -1);
	
			_offset += (inserted - main.begin() == _current_j_nbr + _amount_inserts);
			// offset is nessecary due to rounding errors
			bountIt = next_iterator(main.begin(), _current_j_nbr + _amount_inserts - _offset);
		}
		prev_j_nbr = _current_j_nbr;
		_amount_inserts += _j_difference;
		_offset = 0;
	}
}

/*-------------------------------deque functions-----------------------------*/

void		PmergeMe::mergeInsertionSort(std::deque<unsigned int> &array, \
	int sorting_level)
{
	if (array.size() < 2)
	{
		gettimeofday(&t_deq_end, NULL);	
		return ;
	}
	typedef std::deque<unsigned int>::iterator Iterator;

	int pair_amount = array.size() / sorting_level;
	if (pair_amount < 2)
		return ;

	bool is_odd = pair_amount % 2 == 1;
	Iterator start = array.begin();
	Iterator last = next_iterator(array.begin(), sorting_level * (pair_amount));
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

	insert(array, main, to_append, sorting_level, pair_amount, is_odd, end);
	//copying all objects from main to our original so it gets updated for all recursions
	// recursion waits for the "child" to finish. so we can safely copy without having to worry about
	// messing up data for the other recursion steps
	_copy(array, sorting_level, main);
	gettimeofday(&t_deq_end, NULL);
}

void		PmergeMe::insert(std::deque<unsigned int> &array, std::deque<std::deque<unsigned int>::iterator> &main, \
	std::deque<std::deque<unsigned int>::iterator> &to_append, \
	int sorting_level, int pair_amount, bool is_odd, std::deque<unsigned int>::iterator end)
{
	typedef std::deque<unsigned int>::iterator Iterator;

	main.insert(main.end(), next_iterator(array.begin(), sorting_level - 1));
	main.insert(main.end(), next_iterator(array.begin(), sorting_level * 2 - 1));
	
	for (int i = 4; i <= pair_amount; i += 2)
	{
		to_append.insert(to_append.end(), next_iterator(array.begin(), \
			sorting_level * (i - 1) - 1));
		main.insert(main.end(), next_iterator(array.begin(), sorting_level * i - 1));
	}

	if (is_odd)
    {
        to_append.insert(to_append.end(), next_iterator(end, sorting_level - 1));
    }

	jacobsthal_number(main, to_append);

	for (size_t i = 0; i < to_append.size(); i++)
	{
		typename std::deque<Iterator>::iterator _current_append = \
			next_iterator(to_append.begin(), i);
		typename std::deque<Iterator>::iterator _current_bound = \
			next_iterator(main.begin(), main.size() - to_append.size() + i + is_odd);
		typename std::deque<Iterator>::iterator idx = \
			std::upper_bound(main.begin(), _current_bound, *_current_append, _operator<Iterator>);
		main.insert(idx, *_current_append);
	}
	//if there are no jn left, but still objects in to_append then we insert them here,
	//starting from the back and in reverse order to still minimise the amount of inserts.
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

	int prev_j_nbr = _jacobsthal_nbr(1);
	int	_amount_inserts = 0;
	for (int k = 2;; k++)
	{
		int _current_j_nbr = _jacobsthal_nbr(k);
		int _j_difference = _current_j_nbr - prev_j_nbr;
		int _offset = 0;
		if (_j_difference > static_cast<int>(to_append.size()))
			break ;
		int _counter = _j_difference;
		std::deque<Iterator>::iterator pendIt = \
			next_iterator(to_append.begin(), _j_difference - 1);
		std::deque<Iterator>::iterator bountIt = \
			next_iterator(main.begin(), _current_j_nbr + _amount_inserts);
		while (_counter)
		{
			std::deque<Iterator>::iterator idx = \
					std::upper_bound(main.begin(), bountIt, *pendIt,\
					_operator<Iterator>);
			std::deque<Iterator>::iterator inserted = \
					main.insert(idx, *pendIt);
			_counter--;
			pendIt = to_append.erase(pendIt);
			std::advance(pendIt, -1);
			
			//offset for specific case where inserted nbr is exactly at index of where bound should be
			//to avoid having more comparisons than nessecary
			_offset += (inserted - main.begin() == _current_j_nbr + _amount_inserts);
			bountIt = next_iterator(main.begin(), _current_j_nbr + _amount_inserts /*- _offset*/);
		}
		prev_j_nbr = _current_j_nbr;
		_amount_inserts += _j_difference;
		_offset = 0;
	}
}

/*-----------------------------Custom Exceptions------------------------------*/

const char		*PmergeMe::InvalidInputException::what() const throw()
{
	return "Error: Invalid input.";
}

const char		*PmergeMe::DoubleNumberException::what() const throw()
{
	return "Error: Double value.";
}

/*-------------------------------------Debug----------------------------------*/

template<typename T> bool PmergeMe::checkSorted(T array)
{
	if (array.size() <= 1)
		return true;
	typename T::iterator _current = array.begin();
	typename T::iterator _compare = array.begin() + 1;
	
	std::map<unsigned int, unsigned int>	wrong;
	std::vector<int>						wrong_index;
	int count = 0;

	for (size_t i = 0; i < array.size() - 1; i++)
	{
		if (*_current > *_compare)
		{
			wrong_index.push_back(i);
			wrong[*_current] = *_compare;
			count++;
		}
		_current++;
		_compare++;
	}
	if (count)
	{
		std::cout << "Amount of wrongly sorted: " << count << std::endl;
		std::vector<int>::iterator idx = wrong_index.begin();
		std::map<unsigned int, unsigned int>::iterator it = wrong.begin();
		for (; it != wrong.end(); it++)
		{
			std::cout << "Index " << *idx << ": ";
			std::cout << it->first << " " << it->second << std::endl;
			idx++;
		}
		return false;
	}
	return true;
}