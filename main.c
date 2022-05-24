/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 10:28:52 by bbordere          #+#    #+#             */
/*   Updated: 2022/05/24 11:19:33 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	**ft_init_env(t_list **env, char **envp)
{
	size_t	i;
	t_list	*temp;
	char	*value;

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
	data->childs = NULL;
	data->pipes = NULL;
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

void	ft_update_type(t_token **tokens, int mode) // Mode sert juste a rechanger le type des wildcards qui ont deja ete expand 
{
	size_t	i;

	i = 0;
	while (tokens[i])
	{
		if (mode == 1)
			if (tokens[i]->type == WILDCARD || tokens[i]->type == S_QUOTE || tokens[i]->type == D_QUOTE || tokens[i]->type == VAR)
				tokens[i]->type = WORD;
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
		if (tokens[i]->type == OUT_A_FILE || tokens[i]->type == OUT_FILE
		||tokens[i]->type == IN_FILE || tokens[i]->type == DELIMITER)
		{
			if (tokens[i]->val[0] == '\'')
			{
				ft_memmove(tokens[i]->val, &tokens[i]->val[1], ft_strlen(tokens[i]->val));
				tokens[i]->val[ft_strlen(tokens[i]->val) - 1] = '\0';
			}
		}
		i++;
	}	
}

void	ft_free_loop(void **lexed, void **regrouped, t_token **tokens, t_token **final)
{
	if (lexed)
		ft_free((void **)lexed);
	if (regrouped)
		ft_free((void **)regrouped);
	if (tokens)
		ft_free_tokens(tokens);
	if (final)
		ft_free_tokens(final);
	lexed = NULL;
	regrouped = NULL;
	tokens = NULL;
	final = NULL;
}

/*
Pour skip suppress les leaks de readline avec valgrind : valgrind --suppressions=rl ./minishell
*/

char	*ft_prompt(t_list **env)
{
	char	*prompt;
	char	*pwd;
	char	*home;
	char	*temp;

	if (!*env)
		return (ft_strdup("minishell > "));
	pwd = ft_get_var(env, "PWD");
	temp = pwd;
	pwd = ft_strrchr(pwd, '/') + 1;
	home = ft_get_var(env, "HOME");
	prompt = ft_strjoin2("\1\033[0;32m\2", ft_strjoin1(ft_charjoin(ft_get_var(env, "LOGNAME"), '@'), "minishell\1\033[0;37m:\033[0;34m\2"));
	if (ft_strstr(pwd, home))
		prompt = ft_strjoin1(prompt, ft_strjoin2("~", pwd + ft_strlen(home)));
	else
		prompt = ft_strjoin1(prompt, pwd);
	prompt = ft_strjoin1(prompt, "\1\033[0;37m\2$ ");
	free(home);
	free(temp);
	return (prompt);
}

void    handler_int(int sig)
{
	if (g_global->in_exec == 0)
	{
		if (sig == SIGINT)
		{
			ft_putstr_fd("\b\b  \b\b\n", 1);
			printf("%s", g_global->prompt);
			g_global->rtn_val = 130;
		}
		else if (sig == SIGQUIT)
			ft_putstr_fd("\b\b  \b\b\n", 1);
	}
	else if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		kill(g_global->pid, SIGINT);
		g_global->rtn_val = 130;
	}
}

int ft_sig_init(void)
{
	struct sigaction sig_int;

	g_global = malloc(sizeof(t_global));
	if (!g_global)
		return (0);
	sig_int.sa_handler=&handler_int;
	sigaction(SIGINT,&sig_int,0);
	sigaction(SIGQUIT,&sig_int,0);
	g_global->in_exec = 0;
	g_global->pid = 0;
	g_global->rtn_val = 0;
	return (1);
}

t_global	*g_global;

int main(int ac, char **av, char **env)
{
	t_data *data;
	t_token **tokens; //Premier tableau de tokens
	t_token	**final; // Tableau de tokens une fois regroupe
	char	**lexed; // Tableau des strings une fois "lexee"
	char	*input;
	char	**regrouped; // Tableau des strings "regroupees"
	
	ft_sig_init();
	data = ft_init_data(env);
	while (1)
	{
		input = NULL;
		lexed = NULL;
		tokens = NULL;
		final = NULL;
		regrouped = NULL;
		g_global->prompt = ft_prompt(data->env);
		input = readline(g_global->prompt);
		// input = readline("$> ");
		if (!input)
			break ;
		if (*input)
		{
			g_global->in_exec = 1;
			add_history(input);
			lexed = ft_lexer(input);
			tokens = ft_tokenize(lexed);

			if (!ft_check_grammar(tokens)) // Renvoie le msg d'erreur dans la fonction ft_check_grammar
			{
				ft_free((void **)lexed);
				ft_free_tokens(tokens);
				free(input);
				continue ;
			}
			ft_update_type(tokens, 0);
			ft_expand(tokens, data->env, data->wd);
			regrouped = ft_join(tokens);
			final = ft_tokenize(regrouped);
			ft_update_type(final, 1);
			ft_pipeline(data, final);
			// int i = 0;
			// while (final[i])
			// {
			// 	printf("%s:%d\n", final[i]->val, final[i]->type);
			// 	i++;
			// }			
			// int i = 0;
			// while (tokens[i])
			// {
			// 	printf("%s : %d -%d-\n", tokens[i]->val, tokens[i]->type, ft_strcmp(tokens[i]->val, "\'\'"));
			// 	i++;
			// }
			// int i = 0;
			// while (lexed[i])
			// {
			// 	printf("%s\n", lexed[i]);
			// 	i++;
			// }
			// ft_check_builtin(final);  //Desactiver pour les tests de pipes
			// ft_check_separator(data, final, data->env); // Changer le nom de la fonction
			ft_free_loop((void **)lexed, (void **)regrouped, tokens, final);
			g_global->in_exec = 0;
		}
		free(g_global->prompt);
		free(input);
	}
	free(g_global->prompt);
	printf("exit\n");
	rl_clear_history();
	free(g_global);
	if (data->pipes)
		ft_free_tab((void **)data->pipes);
	if (data->childs)
		free(data->childs);
	ft_lstdel_all(data->env);
	ft_lstdel_all(data->wd);
	free(data);
	ft_free_loop((void **)lexed, (void **)regrouped, tokens, final);

	return (0);
}
