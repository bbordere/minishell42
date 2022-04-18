# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/18 21:36:22 by bbordere          #+#    #+#              #
#    Updated: 2022/04/18 21:36:22 by bbordere         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc

CFLAGS = -g3 -lrealine -Ofast libft/libft.a

FILES =  $(wildcard lexer/*.c) parser/parser.c expansions/expansions.c pipe/pipe.c

OBJS = $(FILES:.c=.o)

%.o: %.c
	@printf "\033[0;33mCompiling : %-33.33s\r" $@
	@$(CC) $(CFLAGS) -c $< -o $@

NAME = minishell

$(NAME): $(OBJS)
	@ printf '\n\033[0;33mCompiling minishell\033[0m\n'
	@ $(CC) $(CFLAGS) $(OBJS) -o $(NAME) 
	@ printf '\033[0;32mminishell compiled sucessfully !\033[0m\n'

all: $(NAME)

clean:
	@ rm -f $(OBJS)
	@ printf '\033[0;32mclean done\033[0m\n'

fclean: clean
	@ rm -f $(NAME)
	@ printf '\033[0;32mfclean done\033[0m\n'

re: fclean all

.PHONY: all clean fclean re

.SILENT: $(OBJS)