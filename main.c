/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 10:28:52 by bbordere          #+#    #+#             */
/*   Updated: 2022/04/05 15:40:41 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/lexer.h"

void	*ft_free_tokens(t_token **tokens)
{
	size_t	i;

	i = 0;
	if (!tokens)
		return (NULL);
	while (tokens[i])
	{
		free(tokens[i]->val);
		i++;
	}
	ft_free((void **)tokens);
	tokens = NULL;
	return (NULL);
}

int main(int ac, char **av, char **env)
{
	t_data *data;
	t_token **tokens;
	t_token	**final;
	char	**tab;
	char	*input;
	char	**joined;

	data = ft_init_data(env);

	input = NULL;
	tab = NULL;
	tokens = NULL;
	final = NULL;
	joined = NULL;

	while (1)
	{
		input = NULL;
		tab = NULL;
		tokens = NULL;
		final = NULL;
		joined = NULL;

		
		input = readline("minishell > ");
		// input = ft_strdup("$$$?");

		if (!input)
			break ;
		tab = ft_lexer(input);
		tokens = ft_tokenize(tab);
		ft_expand(tokens, data->env);
		joined = ft_join(tokens);
		final = ft_tokenize(joined);
		
		if (!ft_check_grammar(final))
			printf("Error syntax !\n");
		else
		{
			ft_check_builtin(final);
			
			int z = 0;
			while(final[z])
				printf("%s\n", final[z++]->val);
		}
		ft_free(tab);
		ft_free(joined);
		ft_free_tokens(tokens);
		ft_free_tokens(final);

	}
	printf("exit\n");
	int i;
	i = 0;
	if (tokens)
		while (tokens[i])
			free(tokens[i++]->val);
	i = 0;
	if (final)
		while (final[i])
			free(final[i++]->val);
	ft_lstdel_all(data->env);
	free(data);
	if (joined)
		ft_free((void **)joined);
	if (final)
		ft_free((void **)final);
	if (tokens)
		ft_free((void **)tokens);
	if (tab)
		ft_free((void **)tab);
	return 0;
}


// int main(int argc, char const *argv[])
// {
// 	printf("%lu",ft_size_var("&TEST&TEST&", 0));
// 	return 0;
// }
