/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:45:38 by bbordere          #+#    #+#             */
/*   Updated: 2022/04/20 16:33:47 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansions.h"

void	*ft_expand_return_code(char *str)
{
	char	*res;

	res = ft_strjoin2(ft_strdup("{RETURN CODE}"), ft_strdup(&str[1]));
	return (res);
}

char	**ft_init_expand(char **res, char *str, t_temp *temp, t_list	**env)
{
	char	**vars;

	vars = ft_extract_var(str);
	if (!vars)
		return (NULL);
	*res = NULL;
	ft_init_temp(temp, vars, str, env);
	return (vars);
}

char	*ft_copy_quotes(char *res, t_temp *temp)
{
	res = ft_strjoin2(res, ft_get_str(&temp->str[(temp->i++)], 0));
	while (temp->str[temp->i] && temp->str[temp->i] != '\'')
		temp->i++;
	temp->i++;
	return (res);
}

char	*ft_expand_str(t_list **env, char *str)
{
	char	**vars;
	char	*res;
	t_temp	temp;

	vars = ft_init_expand(&res, str, &temp, env);
	if (!vars)
		return (NULL);
	while (str[temp.i])
	{
		if (str[temp.i] == '\"')
			res = ft_str_var(res, &temp);
		else if (str[temp.i] == '\'')
			res = ft_copy_quotes(res, &temp);
		else if (str[temp.i + 1] && str[temp.i] == '$'
			&& str[temp.i + 1] != '$' && !ft_issep(str[temp.i + 1]))
			res = ft_var(res, &temp);
		else
			res = ft_charjoin(res, str[temp.i++]);
	}
	ft_free((void **)temp.vars);
	free(temp.str);
	return (res);
}

void	ft_expand(t_token **tokens, t_list **env)
{
	size_t	i;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == VAR || tokens[i]->type == D_QUOTE
			|| tokens[i]->type == IN_FILE || tokens[i]->type == OUT_A_FILE
			|| tokens[i]->type == OUT_FILE || tokens[i]->type == S_QUOTE)
		{
			tokens[i]->val = ft_expand_str(env, tokens[i]->val);
			tokens[i]->type = WORD;
		}
		// else if (tokens[i]->type == S_QUOTE)
		// {
		// 	tokens[i]->val = ft_get_str(tokens[i]->val, 1);
		// }
		i++;
	}	
}
