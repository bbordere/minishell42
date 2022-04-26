/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 10:28:52 by bbordere          #+#    #+#             */
/*   Updated: 2022/04/26 23:15:12 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/lexer.h"
#include "expansions/expansions.h"
#include "parser/parser.h"
#include <readline/readline.h>
#include <readline/history.h>

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

t_list	**ft_init_wd(t_list **wd)
{
	wd = malloc(sizeof(t_list));
	if (!wd)
		return (NULL);
	*wd = NULL;
	return (wd);
}

t_data	*ft_init_data(char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);//modifier secu err dans main
	data->env = ft_init_env(data->env, envp);
	if (!data->env)
		return (NULL);
	data->wd = ft_init_wd(data->wd);
	if (!data->wd)
		return (NULL);
	data->fd_in = STDIN_FILENO;
	data->fd_out = STDOUT_FILENO;
	data->rtn_val = 0;
	data->nb_heredoc = 0;
	data->act_heredoc = -1;
	return (data);
}

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
void	ft_check_separator(t_data *data, t_token **args, t_list **env);

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
			// ft_check_builtin(tokens);
			if (!ft_check_grammar(tokens))
			{
				ft_free((void **)tab);
				ft_free_tokens(tokens);
				free(input);
				continue ;
			}
			ft_update_type(tokens);
			ft_expand(tokens, data->env, data->wd);
			joined = ft_join(tokens);
			final = ft_tokenize(joined);
			ft_update_type(final);
			// ft_check_builtin(final);
			int o = 0;
			while (final[o])
			{
				printf("----------%s %d----------\n", final[o]->val, final[o]->type);
				o++;
			}
			ft_check_separator(data, final, data->env);
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
	ft_lstdel_all(data->wd);
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

// int	main(int argc, char **argv, char **envp)
// {
// 	t_data	*data;
// 	char	**tab;
// 	char	**tab2;
// 	char	**tab3;
// 	t_token **tab4;

// //	tab = ft_split("a HOME", ' ');
// //	tab2 = ft_split("a /tmp", ' ');
// //	tab3 = ft_split("cd /usr/bin/", ' ');
// 	data = ft_init_data(envp);
// //	ft_echo(tab);
// //	data->env = ft_export(data->env, tab);
// //	ft_env(data->env);
// //	data->env = ft_unset(data->env, tab);
// //	ft_cd(data, tab2);
// //	ft_pwd();
// //	ft_env(data->env);
// //	ft_wildcard(data->wd, "*in*");

// 	int	i;

// 	i = 0;
// 	tab4 = malloc(sizeof(t_token *) * 6);
// 	while (i < 6)
// 	{
// 		tab4[i] = malloc(sizeof(t_token));
// 		i++;
// 	}
// 	tab4[0]->val = "<";
// 	tab4[0]->type = R_IN;
// 	tab4[1]->val = "in";
// 	tab4[1]->type = IN_FILE;
// 	tab4[2]->val = "cat";
// 	tab4[2]->type = WORD;
// 	tab4[3]->val = "|";
// 	tab4[3]->type = PIPE;
// 	tab4[4]->val = "cat";
// 	tab4[4]->type = WORD;
// 	tab4[5] = NULL;
// 	ft_check_separator(data, tab4, data->env);
// /*
// 	char *input;
// 	ft_init_sig();
// 	while(1)
// 	{
// 		input = readline("Prompt > ");
// 		if (!input)
// 			break ;
// 		add_history(input);
// 	printf("%s", input);
// 	}
// 	printf("exit");
// */
// 	ft_lstdel_all(data->env);
// 	ft_lstdel_all(data->wd);
// 	free(data);
// }
