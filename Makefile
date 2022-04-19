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

# CFLAGS = -g3 -lrealine -Ofast libft/libft.a

CFLAGS = -Werror

FILES =  $(wildcard lexer/*.c) parser/parser.c expansions/expansion.c pipe/pipe.c main.c

OBJS = $(FILES:.c=.o)

%.o: %.c
	@printf "\033[0;33mCompiling : %-33.33s\r" $@
	@$(CC) $(CFLAGS) -c $< -o $@

NAME = minishell

$(NAME): $(OBJS)
	@ $(MAKE) -C libft all --no-print-directory
	@ printf '\n\033[0;33mCompiling minishell\033[0m\n'
	@ $(CC) $(CFLAGS) $(OBJS) -o $(NAME) -g3 -lreadline -Ofast libft/libft.a
	@ printf '\033[0;32mminishell compiled sucessfully !\033[0m\n'

all: $(NAME)

clean:
	@ rm -f $(OBJS)
	@ $(MAKE) -C libft clean --no-print-directory
	@ printf '\033[0;32mclean done\033[0m\n'

fclean: clean
	@ rm -f $(NAME)
	@ $(MAKE) -C libft fclean --no-print-directory
	@ printf '\033[0;32mfclean done\033[0m\n'

re: fclean all

.PHONY: all clean fclean re

.SILENT: $(OBJS)