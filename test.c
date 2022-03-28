/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:57:41 by bbordere          #+#    #+#             */
/*   Updated: 2022/03/28 14:16:26 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <readline/history.h>

#include "lexer/lexer.h"

int	ft_check_option(char **args)
{
	int	i;

	i = -1;
	while (args[++i])
		if (!strcmp(args[i], "-n") && i == 0)
			return (1);
	return (0);
}

int	ft_echo(char **args)
{
	int	n;
	int	limit;
	int	i;

	if (!args)
	{
		printf("\n");
		return (1);
	}
	n = ft_check_option(args);
	limit = 0;
	while (args[limit])
		limit++;
	i = 0;
	if (n)
		i = 1;
	while (i < limit)
	{
		printf("%s", args[i]);
		i++;
		if (args[i])
			printf(" ");
	}
	if (!n)
		printf("\n");
	return (1);
}

int main(int argc, char const *argv[])
{
	char	*str;
	char	*input;
	char	buf[4096];
	char	**tab;
	char	*temp;

	while (1)
	{
		str = ft_strdup(getcwd(buf, 4096));
		temp = ft_strjoin(str, " \e[1m\e[34mminishell > \e[0m");
		input = readline(temp);
		if (!input)
		{
			printf("\nGood Bye !\n");
			break;
		}
		tab = ft_lexer(input);
		ft_echo(tab);
		ft_free(tab);
		free(temp);
		free(input);
	}
	// str = malloc(2500);
	// getcwd(str,2500);
	// printf("%s\n", str);
	// chdir("../");
	// getcwd(str,2500);
	// printf("%s\n", str);

	// char *file = "search.txt";
	// struct stat *buf;

	// buf = malloc(sizeof(struct stat));
	// stat(file, buf);
	// int	size = buf->st_size;
	// printf("%d\n", size);
	return 0;
}
/*

PARSER 

	CMD
	ARGS

CMD
	NAME
	PATH
	INPUT
	OUTPUT

ARGS
	TAB [ARG, END]


minishel > ls -lsa | wc -l

			ls; -lsa; |; wc; -l;

*/