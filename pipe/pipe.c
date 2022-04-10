/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 23:58:05 by bbordere          #+#    #+#             */
/*   Updated: 2022/04/11 00:50:56 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lexer/lexer.h"

int		ft_ispipelines(int type)
{
	return (type == WORD || type == ARGS || type == PIPE || type == T_FILE);
}

int	ft_isredir(int type)
{
	return (type == R_APPEND || type == R_HERE_DOC || type == R_IN || type == R_OUT);
}

int	ft_contain_pipes(t_token **tokens)
{
	size_t	i;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == PIPE)
			return (1);
		i++;
	}
	return (0);
}

size_t	ft_count_pipelines(t_token **tokens)
{
	size_t	nb;
	size_t	i;

	nb = 0;
	if (!ft_contain_pipes(tokens))
		return (1);
	i = 0;
	while (tokens[i])
	{
		if (tokens[i] && ft_ispipelines(tokens[i]->type))
		{
			while(tokens[i + 1] && ft_ispipelines(tokens[i + 1]->type))
				i++;
			if (tokens[i + 1] && ft_isredir(tokens[i + 1]->type))
				nb--;
			nb++;
		}
		i++;
	}
	return (nb);
}

t_token	*ft_tokendup(t_token *token)
{
	t_token *res;

	res = ft_init_token(token->val);
	if (!res)
		return (NULL);
	res->type = token->type;
	return (res);
}

t_list	**ft_get_pipelines(t_token **tokens)
{
	t_list	**res;
	size_t	i;
	size_t	j;

	res = malloc(sizeof(t_list *) * (ft_count_pipelines(tokens) + 1));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	t_list *temp;
	while (tokens[i])
	{
		if (tokens[i]->type == R_IN)
			i++;
		if (tokens[i] && ft_ispipelines(tokens[i]->type))
		{
			res[j] = NULL;
			while(tokens[i] && (ft_ispipelines(tokens[i]->type) || ft_isredir(tokens[i]->type)))
			{
				if (tokens[i] && tokens[i]->type != PIPE && !ft_isredir(tokens[i]->type))
					ft_lstadd_back(&res[j], ft_lstnew(ft_tokendup(tokens[i])));
				i++;
			}
			if (i >= ft_tab_size(tokens))
				i--;
		}	
		i++;
		j++;	
	}
	res[j] = NULL;
	return (res);	
}

// void	ft_expand(t_token **tokens, t_list **env)

// int main(int argc, char **av)
// {
// 	t_token **tokens;

// 	tokens = ft_tokenize(ft_join(ft_tokenize(ft_lexer(av[1]))));
// 	int z = 0;
// 	printf("---%lu---\n", ft_count_pipelines(tokens));
// 	ft_expand(tokens);
// 	ft_get_pipelines(tokens);
// 	// while (tokens[z])
// 	// {
// 	// 	printf("%s\n", tokens[z]->val);
// 	// 	z++;
// 	// }
		
// 	return 0;
// }