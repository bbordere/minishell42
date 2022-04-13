/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 16:08:45 by bbordere          #+#    #+#             */
/*   Updated: 2022/04/13 11:20:11 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	ft_islimit(int type)
{
	return (type == PIPE || type == D_PIPE || type == D_AND
		|| type == I_PAR || type == O_PAR || type == R_IN || type == R_OUT
		|| type == R_HERE_DOC || type == R_APPEND || type == T_FILE);
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
			while (tokens[i] && !ft_islimit(tokens[i]->type))
				i++;
		else
			i++;
		nb++;
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
	while (j < nb)
	{
		temp = NULL;
		if (tokens[i] && !ft_islimit(tokens[i]->type))
		{
			while (tokens[i] && !ft_islimit(tokens[i]->type))
				temp = ft_strjoin(ft_strjoin(temp, tokens[i++]->val), " ");
			if (temp[ft_strlen(temp) - 1] == ' ')
				temp[ft_strlen(temp) - 1] = '\0';
		res[j++] = ft_strdup(temp);
		free(temp);
		}
		else
			res[j++] = ft_strdup(tokens[i++]->val);
	}
	res[j] = NULL;
	return (res);
}

// char	**ft_join(t_token **tokens)
// {
// 	size_t	nb;
// 	size_t	i;
// 	size_t	j;
// 	char	**res;
// 	char	*temp;

// 	nb = ft_count_join(tokens);
// 	printf("%lu\n", nb);
// 	res = malloc(sizeof(char *) * (nb + 1));
// 	if (!res)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	while (j < nb)
// 	{
// 		temp = NULL;
// 		if (tokens[i] && tokens[i]->type == VAR)
// 			while (tokens[i] && tokens[i]->type == VAR)
// 				temp = ft_strjoin(temp, tokens[i++]->val);
// 		else if (tokens[i] && !ft_islimit(tokens[i]->type))
// 		{
// 			while (tokens[i] && !ft_islimit(tokens[i]->type))
// 			{
// 				if (tokens[i]->type == VAR)
// 					temp = ft_strjoin(temp, tokens[i++]->val);
// 				else
// 					temp = ft_strjoin(ft_strjoin(temp, tokens[i++]->val), " ");
// 			}	
// 			if (temp[ft_strlen(temp) - 1] == ' ')
// 				temp[ft_strlen(temp) - 1] = '\0';
// 		}
// 		else
// 			temp = ft_strdup(tokens[i++]->val);
// 		res[j++] = ft_strdup(temp);
// 		free(temp);		
// 	}
// 	res[j] = NULL;
// 	return (res);
// }

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
// 	while (te[i])
// 	{
// 		ft_printf("%d %s\n", te[i]->type, te[i]->val);
// 		i++;
// 	}
// 	ft_free(joined);
// 	ft_free(te);
// 	ft_free(tabo);
// 	ft_free(tab);
// 	return 0;
// }
