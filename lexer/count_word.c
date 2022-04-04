/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:07:19 by bbordere          #+#    #+#             */
/*   Updated: 2022/03/25 11:07:19 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	ft_count_word_sep(char *str, size_t *i, size_t *nb)
{
	char	sep;

	sep = str[*i];
	(*i)++;
	while (str[*i] && str[*i] != sep)
		(*i)++;
	(*i)++;
	(*nb)++;
}

void	ft_count_word_spec(char *str, size_t *i, size_t *nb)
{
	char	sep;

	if (!ft_isspace(str[*i]))
	{
		sep = str[(*i)];
		(*nb)++;
	}
	if (ft_isspecchar(str[(*i) + 1]) && str[(*i) + 1] == sep)
		(*i)++;
	(*i)++;
}

void	ft_count_word(char *str, size_t *i, size_t *nb)
{
	while (str[*i] && !ft_isspace(str[*i]) && !ft_isspecchar(str[*i])
		&& !ft_issep(str[*i]) && !ft_ispar(str[*i]))
		(*i)++;
	(*nb)++;
}

void	ft_count_var(char *str, size_t *i, size_t *nb)
{
	(*i)++;
	while (str[*i] && !ft_isspace(str[*i]) && !ft_isspecchar(str[*i])
		&& !ft_issep(str[*i]) && !ft_ispar(str[*i]))
		(*i)++;
	(*nb)++;
}

size_t	ft_block_count(char *str)
{
	size_t	i;
	size_t	nb;

	i = 0;
	nb = 0;
	while (str[i])
	{
		ft_skip_spaces(str, &i);
		if (str[i] == '$' || (str[i] == '&' && str[i + 1] != '&' && !ft_issep(str[i + 1]) && !ft_isspace(str[i + 1]) && !ft_isspecchar(str[i + 1]) && !ft_ispar(str[i + 1])))
			ft_count_var(str, &i, &nb);
		else if (str[i] && ft_ispar(str[i]))
		{
			nb++;
			i++;
		}
		else if (str[i] && ft_issep(str[i]))
			ft_count_word_sep(str, &i, &nb);
		else if (str[i] && ft_isspecchar(str[i]))
			ft_count_word_spec(str, &i, &nb);
		else
			ft_count_word(str, &i, &nb);
		ft_skip_spaces(str, &i);
	}
	return (nb);
}
