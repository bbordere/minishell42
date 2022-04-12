/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 14:46:39 by bbordere          #+#    #+#             */
/*   Updated: 2022/04/12 14:16:59 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lexer/lexer.h"

int		ft_isop(int type)
{
	return (type == R_OUT || type == R_APPEND
		|| type == PIPE || type == D_AND || type == D_PIPE);
}

size_t	ft_tab_size(t_token	**tokens)
{
	size_t	len;

	len = 0;
	while (tokens[len])
		len++;
	return (len);	
}

int	ft_check_op(t_token **tokens, size_t	i)
{
	if (i == 0 || i == ft_tab_size(tokens) - 1)
		return (0);
	return ((tokens[i - 1]->type == WORD || tokens[i - 1]->type == ARGS || tokens[i - 1]->type == VAR || tokens[i - 1]->type == T_FILE)
		&& (tokens[i + 1]->type == WORD || tokens[i + 1]->type == ARGS || tokens[i + 1]->type == VAR || tokens[i + 1]->type == T_FILE));
}

int	ft_check_here_doc(t_token **tokens, size_t i)
{
	if (i == ft_tab_size(tokens) - 1)
		return (0);
	return (tokens[i + 1]->type == WORD || tokens[i + 1]->type == VAR || tokens[i + 1]->type == T_FILE);
}

int	ft_check_grammar(t_token **tokens)
{
	size_t	i;
	int		res;

	if (ft_tab_size(tokens) == 1)
		return (1);
	i = 0;
	res = 1;
	while (tokens[i])
	{
		if (ft_isop(tokens[i]->type))
			res = ft_check_op(tokens, i);
		else if (tokens[i]->type == R_HERE_DOC || tokens[i]->type == R_IN)
			res = ft_check_here_doc(tokens, i);
		i++;
		if (!res)
			return (0);
	}
	return (1);
}

int	ft_isbuiltin(char *val)
{
	char	**tab;

	tab = ft_lexer(val);
	if (!tab || !*tab)
		return (0);
	if (ft_strnstr(tab[0], "echo", ft_strlen(tab[0]))
		|| ft_strnstr(tab[0], "cd", ft_strlen(tab[0]))
		|| ft_strnstr(tab[0], "pwd", ft_strlen(tab[0]))
		|| ft_strnstr(tab[0], "export", ft_strlen(tab[0]))
		|| ft_strnstr(tab[0], "unset", ft_strlen(tab[0]))
		|| ft_strnstr(tab[0], "env", ft_strlen(tab[0]))
		|| ft_strnstr(tab[0], "exit", ft_strlen(tab[0])))
	{
		ft_free((void **)tab);
		return (1);
	}
	ft_free((void **)tab);
	return (0);
}

void	ft_check_builtin(t_token **tokens)
{
	size_t	i;

	i = 0;
	while (tokens[i])
	{
		if (ft_isbuiltin(tokens[i]->val))
			tokens[i]->type = CMD;
		i++;
	}
}


// int main(int argc, char **av)
// {
// 	char *str = av[1];
// 	char **tab;
// 	t_token	**tabo;

// 	tab = ft_lexer(str);
// 	tabo = ft_tokenize(tab);

// 	char **joined;
// 	joined = ft_join(tabo);
// 	int i = 0;
// 	// printf("\t\t%lu\n", ft_block_count(str));
// 	t_token	**te = ft_tokenize(joined);
// 	ft_check_grammar(te) ? printf("OK\n") : printf("KO\n");
// 	ft_check_builtin(te);
// 	while (te[i])
// 	{
// 		printf("%d %s\n", te[i]->type, te[i]->val);
// 		i++;
// 	}

// 	ft_free((void **)joined);
// 	ft_free((void **)te);
// 	ft_free((void **)tabo);
// 	ft_free((void **)tab);
// 	return 0;
// }