/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:30:06 by ehedeman          #+#    #+#             */
/*   Updated: 2025/01/31 11:52:23 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"


int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Invalid argument number." << std::endl;
		return (0);
	}
	std::string filename = argv[1];
	try
	{
		BitcoinExchange	b(filename);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}