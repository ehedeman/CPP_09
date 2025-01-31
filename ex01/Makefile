# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/14 14:53:54 by ehedeman          #+#    #+#              #
#    Updated: 2025/01/31 13:42:42 by ehedeman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = RPN

SRC =	main.cpp	\
		RPN.cpp

OBJS_DIR = objs/
OBJS = $(SRC:.cpp=.o)
OBJS_PREFIXED = $(addprefix $(OBJS_DIR), $(OBJS))

INCLUDES =	RPN.hpp

CXX = c++

RM = rm -rf
MKDIR = mkdir -p
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic

all: $(NAME)

$(NAME): $(OBJS_PREFIXED)
	$(CXX) $(CXXFLAGS) $(OBJS_PREFIXED) -o $(NAME)

$(OBJS_DIR)%.o : %.cpp $(INCLUDES)
	$(MKDIR) $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) -f $(NAME)
#	$(RM) file.txt

re: fclean all

leaks: all
	valgrind --leak-check=full	\
		 --show-leak-kinds=all ./$(NAME)

.PHONY: all clean re fclean leaks