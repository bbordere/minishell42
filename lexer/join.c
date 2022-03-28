/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 16:08:45 by bbordere          #+#    #+#             */
/*   Updated: 2022/03/28 17:08:54 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	ft_islimit(int type)
{
	return (type == PIPE || type == D_PIPE || type == AND || type == D_AND
		|| type == I_PAR || type == O_PAR || type == R_IN || type == R_OUT
		|| type == R_HERE_DOC || type == R_APPEND);
}

size_t	ft_count_join(t_token **tokens)
{
	size_t	i;
	size_t	nb;

	i = 0;
	nb = 0;
	while (tokens[i])
	{
		if (tokens[i + 1] && !ft_islimit(tokens[i]->type))
		{
			while (tokens[i + 1] && !ft_islimit(tokens[i]->type))
				i++;
			nb++;
		}
		nb++;
		i++;
	}
	
	return (nb);
}

char	**ft_join(t_token **tokens)
{
	size_t	nb;
	size_t	i;
	size_t	j;
	char	**res;
	char	*temp;

	nb = ft_count_join(tokens);
	res = malloc(sizeof(char *) * (nb + 1));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	printf("%lu\n", nb);
	temp = NULL;
	while (tokens[i])
	{
		if (tokens[i + 1] && !ft_islimit(tokens[i]->type))
		{
			while (tokens[i + 1] && !ft_islimit(tokens[i]->type))
			{
				temp = ft_strjoin(ft_strjoin(temp, tokens[i]->val), " ");
				i++;
			}
		}
		else
		{
			temp = tokens[i]->val;
			i++;
		}
		printf("%s\n", temp);
		i++;
	}
	res[j] = NULL;
	return (res);
}

int main(int argc, char **av)
{
	char *str = av[1];
	char **tab;
	t_token	**tabo;

	tab = ft_lexer(str);
	tabo = ft_tokenize(tab);

	char **joined;
	joined = ft_join(tabo);
	int i = 0;
	// printf("\t\t%lu\n", ft_block_count(str));
	while (joined[i])
	{
		printf("%s\n", joined[i]);
		i++;
	}
	ft_free(tab);
	ft_free(tabo);
	return 0;
}
