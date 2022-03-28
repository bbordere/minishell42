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

int	ft_init_op(t_token *token, char *val)
{
	if (!ft_strncmp(val, "<", ft_strlen(val)))
		token->type = R_IN;
	else if (!ft_strncmp(val, "<<", ft_strlen(val)))
		token->type = R_HERE_DOC;
	else if (!ft_strncmp(val, ">", ft_strlen(val)))
		token->type = R_OUT;
	else if (!ft_strncmp(val, ">>", ft_strlen(val)))
		token->type = R_APPEND;
	else if (!ft_strncmp(val, "&", ft_strlen(val)))
		token->type = AND;
	else if (!ft_strncmp(val, "&&", ft_strlen(val)))
		token->type = D_AND;
	else if (ft_strnstr(val, "*", ft_strlen(val)) && !ft_issep(*val))
		token->type = WILDCARD;
	if (token->type == T_NULL)
		return (0);
	return (1);
}

t_token	*ft_init_token(char *val)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->val = val;
	token->type = T_NULL;
	if (ft_init_op(token, val))
		return (token);
	if (!ft_strncmp(val, "|", ft_strlen(val)))
		token->type = PIPE;
	else if (!ft_strncmp(val, "||", ft_strlen(val)))
		token->type = D_PIPE;
	else if (!ft_strncmp(val, "(", 1))
		token->type = I_PAR;
	else if (!ft_strncmp(val, ")", 1))
		token->type = O_PAR;
	else if (!ft_strncmp(val, "\"", 1) || !ft_strncmp(val, "\'", 1))
		token->type = ARGS;
	else if (!ft_strncmp(val, "$", 1))
		token->type = VAR;
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
	res[i] = '\0';
	return (res);
}
