/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 21:48:56 by bbordere          #+#    #+#             */
/*   Updated: 2022/03/22 21:48:56 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int	ft_check_option(char **args)
{
	int	i;

	i = -1;
	while (args[++i])
		if (!strcmp(args[i], "-n") && i == 0)
			return (1);
	return (0);
}

char	**ft_split(char *str, char sep)
{
	int	len;
	int	nb;
	int		i;
	char	**res;
	char	**temp;

	i = -1;
	nb = 1;
	len = strlen(str);
	while (++i < len)
	{
		if (str[i] == sep)
		{
			str[i] = '\0';
			nb++;
		}
	}
	res = malloc(sizeof(char *) * (nb + 1));
	if (!res)
		return (NULL);
	temp = res;
	*res = str;
	i = -1;
	while (++i < len)
	{
		while(str[i + 1] == '\0' && i < len - 2)
			i++;
		if (str[i] == '\0')
		{
			res++;
			*res = &(str[i + 1]);
		}
	}
	*(res+1) = '\0';
	return (temp);
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
	limit = -1;
	while (args[++limit])
;
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
}

int	main(int ac, char **av)
{
	// char *tab[] = {"est", "test", "teest", "-n", "test",NULL};
	// ft_echo(av + 1);
	char **tab;

	char str[] = "ceci est un test";
	// str_separate(str, ' ', &tab);
	tab = ft_split(av[1], ' ');
	int i = 0;
	ft_echo(tab);
	free(tab);
}
