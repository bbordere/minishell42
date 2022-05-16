/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 13:41:13 by bbordere          #+#    #+#             */
/*   Updated: 2022/05/16 17:37:03 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

pid_t	*ft_update_childs(t_data *data, size_t n)
{
	pid_t	*res;

	if (data->childs)
		free(data->childs);
	res = ft_calloc(n + 1, sizeof(pid_t));
	if (!res)
		return (NULL); //PROTECT
	return (res);	
}

int	**ft_update_pipes(t_data *data, size_t n)
{
	int		**pipes;
	ssize_t	i;

	if (data->pipes)
		free(ft_free_tab((void **)data->pipes));
	pipes = ft_calloc(n + 1, sizeof(int *));
	if (!pipes)
		return (NULL); //PROTECT
	i = -1;
	while (++i < n)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (ft_free_tab((void **)pipes), NULL); // PROTECT
		pipe(pipes[i]);
	}
	return (pipes);
}

void	ft_close(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
}

int	ft_check_isapipe(t_token **args, int *i);
void	ft_glhf(t_data *data, t_token **args, t_list **env);
void	ft_find_heredoc(t_data *data, t_token **args);

void	ft_exec_first(t_data *data, t_token **args)
{
	data->childs[0] = fork();
	if (data->childs[0] < 0)
	{
		//ft_free_data(data);
		return ;
	}
	if (!data->childs[0])
	{
		close(data->pipes[0][0]);
		dup2(data->fd_in, STDIN_FILENO);
		dup2(data->pipes[0][1], STDOUT_FILENO);
		ft_close(data->fd_in, data->pipes[0][1]);
		ft_glhf(data, args, data->env);
		//ft_free_data(data);
	}
	else
		ft_close(data->fd_in, data->pipes[0][1]);
}

void	ft_exec_mid(t_data *data, t_token **args, int i)
{
	data->childs[i] = fork();
	if (data->childs[i] < 0)
	{
		//ft_free_data(data);
		return ;
	}
	if (!data->childs[i])
	{
		close(data->pipes[i][0]);
		dup2(data->pipes[i - 1][0], STDIN_FILENO);
		dup2(data->pipes[i][1], STDOUT_FILENO);
		ft_close(data->pipes[i - 1][0], data->pipes[i][1]);
		ft_glhf(data, args, data->env);
		//ft_free_data(data);
	}
	else
		ft_close(data->fd_in, data->pipes[0][1]);
}

void	ft_exec_last(t_data *data, t_token **args, int last)
{
	data->childs[last] = fork();
	if (data->childs[last] < 0)
	{
		//ft_free_data(data);
		return ;
	}
	if (!data->childs[last])
	{
		close(data->pipes[last][1]);
		dup2(data->fd_out, STDOUT_FILENO);
		dup2(data->pipes[last][0], STDIN_FILENO);
		ft_close(data->pipes[last][0], data->fd_out);
		ft_glhf(data, args, data->env);
		//ft_free_data(data);
	}
	else
		ft_close(data->fd_in, data->pipes[0][1]);
}

void	ft_fork(t_data *data, t_token **args, t_list **env);

void	ft_exec_pipeline(t_data *data, t_token **args)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	ft_find_heredoc(data, args);
	while (args[i])
	{
		while (args[i] && args[i]->type != PIPE
			&& args[i]->type != D_PIPE && args[i]->type != D_AND)
			i++;
		if (!args[i])
			ft_fork(data, &args[j], data->env);
		if (args[i] && args[i]->type == PIPE)
		{
			ft_exec_first(data, &args[j++]);
			while (ft_check_isapipe(args, &i))
				// ft_pipe(data, &args[i++], data->env, data->fd_out);
				ft_exec_mid(data, &args[i++], j++);
			// ft_fork(data, &args[j], data->env);
			ft_exec_last(data, &args[i], j);
		}
		if (args[i])
			i++;
		j = i;
	}
}

void	ft_pipeline(t_data *data, t_token **tokens)
{
	// t_token	**f = tokens;
	// size_t	offset = 0;
	// while (*(f + 1))
	// {
		// size_t	pipes = ft_count_pipes(f, &offset);
		// printf("%ld\n", pipes);
		// f += offset;
		// if (*(f + 1))
			// f++;
	// }
	t_token	**temp;
	size_t	offset;
	size_t	pipes;

	temp = tokens;
	offset = 0;
	if (!*(temp + 1))
		ft_glhf(data, tokens, data->env);
	else
	{
	while (*(temp + 1))
	{
		pipes = ft_count_pipes(temp, &offset);
		data->childs = ft_update_childs(data, pipes + 1);
		data->pipes = ft_update_pipes(data, pipes);
		ft_exec_pipeline(data, temp);
		temp += offset;
		if (*(temp + 1))
			temp++;
	}
	}
}

size_t	ft_count_pipes(t_token	**tokens, size_t *offset)
{
	ssize_t	i;
	size_t	count;

	i = -1;
	count = 0;
	while (tokens[++i] && tokens[i]->type != D_PIPE && tokens[i]->type != D_AND)
		if (tokens[i]->type == PIPE)
			count++;
	*offset = i;
	return (count);
}