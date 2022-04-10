/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 10:28:52 by bbordere          #+#    #+#             */
/*   Updated: 2022/04/10 22:58:04 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/lexer.h"
#include <readline/readline.h>
#include <readline/history.h>


t_list	**ft_init_env(t_list **env, char **envp);
t_data	*ft_init_data(char **envp);
void	ft_expand(t_token **tokens, t_list **env);
int	ft_check_grammar(t_token **tokens);
void	ft_check_builtin(t_token **tokens);
void	ft_lstdel_all(t_list **lst);
size_t	ft_count_pipelines(t_token **tokens);
t_list	**ft_get_pipelines(t_token **tokens);


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

char	**ft_pipelines(t_list *pipes)
{
	size_t	j;
	char	**res;
	t_list	*start;
	char	**temp;

	start = pipes;
	temp = NULL;
	res = malloc(sizeof(char *) * (ft_lstsize(pipes) + 3));
	if (!res)
		return (NULL);
	j = 0;
	if (((t_token *)pipes->content)->type != T_FILE)
		res[j++] = ft_strdup("0");
	else
	{
		temp = ft_split(((t_token *)pipes->content)->val, ' ');
		if (!temp)
			return (NULL); // FREE RES
		res[j++] = ft_strdup(temp[0]);
		res[j++] = ft_strdup(((t_token *)pipes->content)->val + ft_strlen(temp[0]) + 1);
		(((t_token *)pipes->content)->type) = WORD;
		if (pipes->next)
			pipes = pipes->next;
	}
	while (pipes->next)
	{
		res[j++] = ft_strdup(((t_token *)pipes->content)->val); // SECU
		pipes = pipes->next;
	}
	if (!pipes->next && ((t_token *)pipes->content)->type != T_FILE)
	{
		if (pipes != start)
			res[j++] = ft_strdup(((t_token *)pipes->content)->val);
		res[j++] = ft_strdup("1");
	}
	else if (!pipes->next && ((t_token *)pipes->content)->type == T_FILE)
	{
		res[j++] = ft_strdup(((t_token *)pipes->content)->val);
	}
	res[j] = NULL;
	if (temp)
		ft_free((void **)temp);
	return (res);
}

void	ft_free_pipeline(t_list *pipeline)
{
	t_list	*temp;

	temp = pipeline;
	while (pipeline)
	{
		pipeline = pipeline->next;
		free(((t_token *)temp->content)->val);
		free(temp->content);
		free(temp);
		temp = pipeline;
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

		// input = readline("minishell > ");
		// input = ft_strdup("cat|cat");
		ft_putstr_fd("Prompt > ", 0);
		input = get_next_line(0);
		if (!input)
			break ;
		if (ft_strncmp(input, "\n", ft_strlen(input)))
		{
			// add_history(input);
			tab = ft_lexer(input);
			tokens = ft_tokenize(tab);
			ft_expand(tokens, data->env);
			joined = ft_join(tokens);
			final = ft_tokenize(joined);
			ft_expand(final, data->env);
			pipes = ft_get_pipelines(final);
			ft_free((void **)tab);
			ft_free((void **)joined);
			ft_free_tokens(tokens);
			ft_free_tokens(final);
			int z = 0;
			char **piped;

			while(pipes[z])
			{
				piped = ft_pipelines(pipes[z]);
				ft_free_pipeline(pipes[z]);
				int y = 0;
				printf("****************************\n");
				while (piped[y])
					printf("~%s~\n", piped[y++]);
				z++;
				printf("****************************\n");
				ft_free((void **)piped);
			}
			free(pipes);
		}
		free(input);
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
