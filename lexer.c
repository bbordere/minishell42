/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 12:07:21 by bbordere          #+#    #+#             */
/*   Updated: 2022/03/23 22:07:53 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/includes/libft.h"
#include <stdio.h>



int		ft_issep(int c)
{
	return (c == '\"' || c == '\'');
}

int		ft_ispar(int c)
{
	return (c == '(' || c == ')');
}

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
	else if (str[i] == '(')
		return (1);
	else if (str[i] == ')')
		return (1);
	else
		while (str[i + size] && !ft_isspace(str[i + size]) && !ft_ispar(str[i + size]))
			size++;
	return (size);	
}


size_t	ft_block_count(char *str)
{
	size_t	i;
	size_t	nb;
	char	sep;

	i = 0;
	nb = 0;
	while (str[i])
	{
		while(str[i] && ft_isspace(str[i]))
			i++;
		if (ft_ispar(str[i]))
		{
			nb++;
			i++;
		}
		else if (ft_issep(str[i++]))
		{
			sep = str[i - 1];
			while (str[i] && str[i++] != sep);
			nb++;
		}
		else
		{
			nb++;
			while (str[i] && !ft_ispar(str[i]) && !ft_isspace(str[i++]));
		}
		while(str[i] && ft_isspace(str[i]))
			i++;
	}
	return (nb);
}

char	**ft_lexer(char *str)
{
	char	**res;
	char	*temp;
	char	sep;
	size_t	i;
	size_t	j;

	i = -1;
	j = 0;
	res = malloc((ft_block_count(str) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	while (++i < ft_block_count(str))
	{
		if (ft_issep(str[j]))
		{
			sep = str[j];
			while (str[j] && str[j] != sep)
				j++;
		}
		else if (str[j] == '(' && j != 0)
			j++;
		else
			while (str[j] && ft_isspace(str[j]) && !ft_issep(str[j]) && !ft_ispar(str[j]))
				j++;
		temp = ft_substr(str, j, ft_word_size(str, j));
		if (!temp)
			return (NULL);
		res[i] = temp;
		j += ft_word_size(str, j);		
	}
	res[i] = '\0';
	return (res);	
}

enum	e_type
{
	WORD = 1,
	PIPE = 2,
	CMD = 3,
	ARGS = 4,
	D_PIPE = 5,
	AND = 6,
	D_AND = 7,
	I_PAR = 8,
	O_PAR = 9,
	VAR = 10
};

typedef struct s_token
{
	char	*val;
	int		type;
}	t_token;

t_token	*ft_init_token(char *val)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->val = val;
	if (!ft_strncmp(val, "|", ft_strlen(val)))
		token->type = PIPE;
	else if (!ft_strncmp(val, "||", ft_strlen(val)))
		token->type = D_PIPE;
	else if (!ft_strncmp(val, "(", 1))
		token->type = I_PAR;
	else if (!ft_strncmp(val, ")", 1))
		token->type = O_PAR;
	else if (!ft_strncmp(val, "&&", ft_strlen(val)))
		token->type = D_AND;
	else if (!ft_strncmp(val, "&", ft_strlen(val)))
		token->type = AND;
	else if (!ft_strncmp(val, "\"", 1))
		token->type = ARGS;
	else if (!ft_strncmp(val, "$", 1))
		token->type = VAR;
	else
		token->type = WORD;
}

t_token	**ft_tokenize(char **tab)
{
	t_token	**res;
	size_t	size;
	size_t	i;

	size = 0;
	i = 0;
	while (tab[size])
		size++;
	res = malloc(sizeof(t_token *) * size);
	if (!res)
		return (NULL);
	while (tab[i])
	{
		res[i] = ft_init_token(tab[i]);
		i++;
	}
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
	while (tabo[i])
	{
		printf("%d %s\n", tabo[i]->type, tabo[i]->val);
		i++;
	}
	
	// printf("%lu", ft_word_size(str, 0));
	
	return 0;
}

// [ceci, est, un, brea, "  ee ", w]

