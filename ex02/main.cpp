/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:15:01 by ehedeman          #+#    #+#             */
/*   Updated: 2025/02/03 12:33:55 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Not enough arguments." << std::endl;
        return 0;
    }
    try
    {
       PmergeMe p(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    

    
}