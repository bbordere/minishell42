/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:57:41 by bbordere          #+#    #+#             */
/*   Updated: 2022/03/22 15:41:35 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <readline/history.h>

int main(int argc, char const *argv[])
{
	char	*str;
	while (1)
	{
		str = readline("\e[1m\e[34mminishell > \e[0m");
		if (!str)
		{
			printf("\nGood Bye !\n");
			break;
		}
		printf("%s\n", str);
		free(str);
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