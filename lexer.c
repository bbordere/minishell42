/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 12:07:21 by bbordere          #+#    #+#             */
/*   Updated: 2022/03/23 17:00:12 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/includes/libft.h"
#include <stdio.h>



int		ft_issep(int c)
{
	return (c == '\"' || c == '\''); //c == '{' || c == '}' || c == '(' || c == ')'
}


size_t	ft_word_size(char *str, size_t i)
{
	size_t	size;

	size = 0;
	if (ft_issep(str[i]))
	{
		i++;
		while (str[i + size] && !ft_issep(str[i + size]))
			size++;
		size += 2;
	}
	else
		while (str[i + size] && !ft_isspace(str[i + size]))
			size++;
	return (size);	
}


size_t	ft_block_count(char *str)
{
	size_t	i;
	size_t	nb;

	i = 0;
	nb = 0;
	while (str[i])
	{
		while(str[i] && ft_isspace(str[i]))
			i++;
		if (ft_issep(str[i++]))
		{
			while (str[i] && !ft_issep(str[i++]));
			nb++;
		}
		else
		{
			nb++;
			while (str[i] && !ft_isspace(str[i++]));
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
	size_t	i;
	size_t	j;

	i = -1;
	j = 0;
	res = malloc((ft_block_count(str) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	while (++i < ft_block_count(str))
	{
		while (str[j] && ft_isspace(str[j]) && !ft_issep(str[j]))
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

enum	e_tokens
{
	WORD = 1,
	PIPE = 2,
	CMD = 3,
	ARGS = 4,
	D_PIPE = 5,
	AND = 6,
	D_AND = 7,
	I_PAR = 8,
	O_PAR = 9

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

int main(int argc, char const *argv[])
{
	char str[] = "ceci est un braquage || ( \"ceci est un test\"456 456   (te   st)     &&  |            )                   \'ete\'456                     ";
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

