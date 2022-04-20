/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 10:28:52 by bbordere          #+#    #+#             */
/*   Updated: 2022/04/20 17:07:26 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/lexer.h"
#include "expansions/expansions.h"
#include "parser/parser.h"
#include <readline/readline.h>
#include <readline/history.h>

void	ft_lstdel_all(t_list **lst)
{
	t_list	*temp;

	if (!lst)
		return ;
	temp = *lst;
	while (lst && *lst)
	{
		(*lst) = (*lst)->next;
		free(temp->content);
		free(temp);
		temp = *lst;
	}
	free(lst);
}

t_list	**ft_init_env(t_list **env, char **envp)
{
	size_t	i;
	t_list	*temp;
	char	*value;
	int		lenght;

	env = malloc(sizeof(t_list));
	if (!env)
		return (NULL);
	*env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		ft_lstadd_back(env, ft_lstnew(envp[i]));
		i++;
	}
	temp = *env;
	while (temp)
	{
		value = temp->content;
		lenght = ft_strlen(value);
		temp->content = ft_strdup(value);
		temp = temp->next;
	}
	return (env);
}

t_data	*ft_init_data(char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);//modifier secu err
	data->env = ft_init_env(data->env, envp);//secu malloc
	return (data);
}

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

void	ft_lstprint(t_list *lst)
{
	while (lst)
	{
		printf("%s {%d} |%p| -> ", ((t_token *)lst->content)->val, ((t_token *)lst->content)->type, lst);
		lst = lst->next;
	}
	printf("NULL\n");
}

void	ft_update_type(t_token **tokens)
{
	size_t	i;

	i = 0;
	while (tokens[i])
	{
		if (i != 0 && tokens[i - 1])
		{
			if (tokens[i - 1]->type == R_OUT)
				tokens[i]->type = OUT_FILE;
			else if (tokens[i - 1]->type == R_IN)
				tokens[i]->type = IN_FILE;
			else if (tokens[i - 1]->type == R_HERE_DOC)
				tokens[i]->type = DELIMITER;
			else if (tokens[i - 1]->type == R_APPEND)
				tokens[i]->type = OUT_A_FILE;
		}
		i++;
	}
	
}

int main(int ac, char **av, char **env)
{
	t_data *data;
	t_token **tokens;
	t_token	**final;
	char	**tab;
	char	*input;
	char	**joined;
	t_list	**pipes;

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
		// ft_putstr_fd("Prompt > ", 0);
		// input = get_next_line(0);
		if (!input)
			break ;
		if (ft_strncmp(input, "\n", ft_strlen(input)))
		{
			add_history(input);
			tab = ft_lexer(input);
			tokens = ft_tokenize(tab);
			ft_check_builtin(tokens);
			if (!ft_check_grammar(tokens))
			{
				ft_free((void **)tab);
				ft_free_tokens(tokens);
				free(input);
				continue ;
			}
			ft_update_type(tokens);
			ft_expand(tokens, data->env);
			joined = ft_join(tokens);		
			final = ft_tokenize(joined);
			ft_update_type(final);
			ft_check_builtin(final);
			int o = 0;
			while (final[o])
			{
				printf("%s %d\n", final[o]->val, final[o]->type);
				o++;
			}			
			ft_free((void **)tab);
			ft_free((void **)joined);
			ft_free_tokens(tokens);
			ft_free_tokens(final);
		}
		free(input);
	}
	printf("exit\n");
	rl_clear_history();
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
