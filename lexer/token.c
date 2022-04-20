/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:08:42 by bbordere          #+#    #+#             */
/*   Updated: 2022/03/25 11:08:42 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	ft_isfulldollar(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '$' && !ft_issep(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_init_op(t_token *token, char *val)
{
	if (!ft_strncmp(val, "<", ft_strlen(val)) && ft_strlen(val) == 1)
		token->type = R_IN;
	else if (!ft_strncmp(val, "<<", ft_strlen(val)) && ft_strlen(val) == 2)
		token->type = R_HERE_DOC;
	else if (!ft_strncmp(val, ">", ft_strlen(val)) && ft_strlen(val) == 1)
		token->type = R_OUT;
	else if (!ft_strncmp(val, ">>", ft_strlen(val)) && ft_strlen(val) == 2)
		token->type = R_APPEND;
	else if (!ft_strncmp(val, "&", ft_strlen(val)))
		token->type = WORD;
	else if (!ft_strncmp(val, "&&", ft_strlen(val)) && ft_strlen(val) == 2)
		token->type = D_AND;
	else if (ft_strnstr(val, "\"", ft_strlen(val)))
		token->type = D_QUOTE;
	else if (ft_strnstr(val, "\'", ft_strlen(val)))
		token->type = S_QUOTE;
	else if (ft_strnstr(val, "*", ft_strlen(val)))
		token->type = WILDCARD;
	else if (ft_strnstr(val, "$", ft_strlen(val)) && !ft_isfulldollar(val))
		token->type = VAR;
	return (token->type == T_NULL);
}

t_token	*ft_init_token(char *val)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->val = ft_strdup(val);
	token->type = T_NULL;
	if (!ft_init_op(token, val))
		return (token);
	if (!ft_strncmp(val, "|", ft_strlen(val)))
		token->type = PIPE;
	else if (!ft_strncmp(val, "||", ft_strlen(val)))
		token->type = D_PIPE;
	else if (!ft_strncmp(val, "(", 1))
		token->type = I_PAR;
	else if (!ft_strncmp(val, ")", 1))
		token->type = O_PAR;
	else
		token->type = WORD;
	return (token);
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
	res = malloc(sizeof(t_token *) * (size + 1));
	if (!res)
		return (NULL);
	while (tab[i])
	{
		res[i] = ft_init_token(tab[i]);
		i++;
	}
	res[size] = NULL;
	return (res);
}
