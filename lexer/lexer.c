/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 12:07:21 by bbordere          #+#    #+#             */
/*   Updated: 2022/03/25 13:45:15 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

size_t	ft_word_size(char *str, size_t i)
{
	size_t	size;
	char	sep;

	size = 0;
	if (ft_issep(str[i]))
	{
		sep = str[i];
		i++;
		while (str[i + size] && str[i + size] != sep)
			size++;
		size += 2;
	}
	else if (ft_ispar(str[i]) || (ft_isspecchar(str[i])
			&& !ft_isspecchar(str[i + 1])))
		return (1);
	else if (ft_isspecchar(str[i]) && ft_isspecchar(str[i + 1]))
		return (2);
	else
		while (str[i + size] && !ft_isspace(str[i + size])
			&& !ft_ispar(str[i + size]) && !ft_isspecchar(str[i + size]))
			size++;
	return (size);
}

void	ft_skip_sep(char *str, size_t *i)
{
	char	sep;

	sep = str[*i];
	while (str[*i] && str[*i] != sep)
		(*i)++;
}

void	*ft_fill_tab(char *str, size_t *i, size_t *j,char **res)
{
	char	*temp;

	if (ft_issep(str[*j]))
		ft_skip_sep(str, j);
	else
		while (str[*j] && ft_isspace(str[*j]) && !ft_issep(str[*j])
			&& !ft_ispar(str[*j]))
			(*j)++;
	temp = ft_substr(str, *j, ft_word_size(str, *j));
	if (!temp)
		return (NULL); //free all tab + return NULL
	res[*i] = temp;
	*j += ft_word_size(str, *j);
	(*i)++;
}

char	**ft_lexer(char *str)
{
	char	**res;
	size_t	i;
	size_t	j;
	char	*temp;

	i = 0;
	j = 0;
	res = malloc((ft_block_count(str) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	while (i < ft_block_count(str))
		ft_fill_tab(str, &i, &j, res);
	res[i] = '\0';
	return (res);
}




int main(int argc, char **av)
{
	char *str = av[1];
	char **tab;
	t_token	**tabo;

	tab = ft_lexer(str);
	tabo = ft_tokenize(tab);
	int i = 0;
	// printf("%lu\n", ft_block_count(str));
	while (tabo[i])
	{
		printf("%d %s\n", tabo[i]->type, tabo[i]->val);
		i++;
	}
	return 0;
}

// [ceci, est, un, brea, "  ee ", w]

