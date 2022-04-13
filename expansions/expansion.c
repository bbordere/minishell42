/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:45:38 by bbordere          #+#    #+#             */
/*   Updated: 2022/04/14 00:23:51 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lexer/lexer.h"

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


void	*ft_return_dup(char *str, char *dup)
{
	free(str - 1);
	if (!dup)
		return (NULL);
	return  (ft_strdup(dup));
}
char	*ft_strjoin2(char *s1, char *s2);

void	*ft_expand_return_code(char *str)
{
	char	*res;

	res = ft_strjoin2(ft_strdup("{RETURN CODE}"), ft_strdup(&str[1]));
	free(str - 1);
	return (res);
}

char	*ft_get_var(t_list **env, char *str)
{
	t_list	*temp;
	size_t	len;
	char	*content;

	if (!env || !(*env))
		return (ft_return_dup(str, NULL));
	if (!ft_strncmp(str, "?", 1))
		return (ft_expand_return_code(str));
	temp = *env;
	len = ft_strlen(str);
	while (temp)
	{
		content = temp->content;
		if (!ft_strncmp(str, content, len) && content[len] == '=')
			return (ft_return_dup(str, &content[len + 1]));
		temp = temp->next;
	}
	return (ft_return_dup(str, ""));
}

static size_t	ft_count_var(char *str)
{
	size_t	nb;
	size_t	i;

	nb = 0;
	i = 0;
	while(str[i])
	{
		if (str[i + 1] && str[i] == '$' && str[i + 1] != '$')
		{
			while (str[i] && !ft_isspace(str[i]) && !ft_isspecchar(str[i])
			&& !ft_issep(str[i]) && str[i] != '$' && !ft_ispar(str[i]))
				i++;
			nb++;
		}
		i++;
	}
	return (nb);
}

static size_t	ft_len_var(char *str, size_t i)
{
	size_t	size;

	i++;
	size = 0;
	while (str[i] && !ft_isspace(str[i]) && !ft_isspecchar(str[i])
		&& !ft_issep(str[i]) && str[i] != '$' && !ft_ispar(str[i]))
	{
		i++;
		size++;
	}
	return (size + 1);
}

char	**ft_extract_var(char *str)
{
	char	**res;
	size_t	i;
	size_t	j;
	size_t	nb;

	i = 0;
	j = 0;
	nb = ft_count_var(str);
	res = malloc(sizeof(char *) * (nb + 1));
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i + 1] && str[i] == '$' && str[i + 1] != '$')
		{
			res[j] = ft_substr(str, i, ft_len_var(str, i));
			j++;
		}
		i++;
	}
	res[j] = NULL;
	return (res);
}

char	*ft_strjoin2(char *s1, char *s2)
{
	size_t	lens1;
	size_t	lens2;
	char	*s3;

	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	s3 = malloc((lens1 + lens2 + 1) * sizeof(char));
	if (!s3)
		return (NULL);
	ft_memcpy(s3, s1, lens1);
	ft_memcpy(s3 + lens1, s2, lens2);
	s3[lens1 + lens2] = '\0';
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (s3);
}

char	*ft_charjoin(char *str, char c)
{
	size_t	len;
	char	*res;

	len = ft_strlen(str);
	res = malloc(sizeof(char) * (len + 2));
	if (!res)
		return (NULL);
	ft_memcpy(res, str, len);
	res[len] = c;
	res[len + 1] = '\0';
	if (str)
		free(str);
	return (res);
}

char **ft_get_vars(t_list **env, char *str, size_t *i, size_t *j, char **res)
{
	char	**vars;

	vars = ft_extract_var(str);
	if (!vars)
		return (NULL);
	*res = NULL;
	*i = 0;
	*j = 0;
	return (vars);
}

char	*ft_get_str(char *str)
{
	size_t	i;
	char	*res;

	res = NULL;
	i = 1;
	while (str[i + 1])
	{
		if (str[i] != str[0])
			res = ft_charjoin(res, str[i]);
		i++;
	}
	return (res);
}

char	*ft_expand_str(t_list **env, char *str)
{
	char	**vars;
	char	*res;
	char	*temp;
	size_t	i;
	size_t	j;

	vars = ft_get_vars(env, str, &i, &j, &res);
	if (!vars)
		return (NULL);
	// temp = ft_strtrim(str, "\"");
	temp = ft_strdup(str);
	free(str);
	while (temp[i])
	{
		if (temp[i + 1] && temp[i] == '$' && temp[i + 1] != '$')
		{
			res = ft_strjoin2(res, ft_get_var(env, vars[j++] + 1));
			while (temp[i + 1] && !ft_isspace(temp[i + 1])
				&& !ft_isspecchar(temp[i + 1]) && !ft_issep(temp[i + 1])
				&& temp[i + 1] != '$' && !ft_ispar(temp[i + 1]))
					i++;
			i++;
		}
		else if (temp[i] == '\"')
		{
			i++;
			while (temp[i] && temp[i] != '\"')
			{
				if (temp[i + 1] && temp[i] == '$' && temp[i + 1] != '$')
				{
					res = ft_strjoin2(res, ft_get_var(env, vars[j++] + 1));
					while (temp[i + 1] && !ft_isspace(temp[i + 1])
						&& !ft_isspecchar(temp[i + 1]) && !ft_issep(temp[i + 1])
						&& temp[i + 1] != '$' && !ft_ispar(temp[i + 1]))
							i++;
					i++;					
				}
				else
					res = ft_charjoin(res, temp[i++]);
			}
			i++;
		}
		else if (temp[i] == '\'')
		{
			i++;
			while (temp[i] && temp[i] != '\'')
				res = ft_charjoin(res, temp[i++]);
			i++;
		}
		else
			res = ft_charjoin(res, temp[i++]);
	}	
	free(vars);
	free(temp);
	return (res);
}
void	ft_expand(t_token **tokens, t_list **env)
{
	size_t	i;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == VAR || tokens[i]->type == D_QUOTE || tokens[i]->type == S_QUOTE)
		{
			tokens[i]->val = ft_expand_str(env, tokens[i]->val);
			tokens[i]->type = WORD;
		}
		if (i != 0 && tokens[i - 1] && (tokens[i - 1]->type == R_OUT || tokens[i - 1]->type == R_IN || tokens[i - 1]->type == R_APPEND))
			tokens[i]->type = T_FILE;
		i++;
	}	
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

// int main(int ac, char **av, char **env)
// {
// 	t_data	*data;

// 	data = ft_init_data(env);

// 	char *str = av[1];
// 	char **tab;
// 	t_token	**tabo;

// 	tab = ft_lexer(str);
// 	tabo = ft_tokenize(tab);


	

// 	ft_expand(tabo, data->env);
// 	char **joined;
// 	joined = ft_join(tabo);
// 	int i = 0;

	
// 	// // printf("\t\t%lu\n", ft_block_count(str));
// 	t_token	**te = ft_tokenize(joined);
// 	ft_check_grammar(te) ? printf("OK\n") : printf("KO\n");
// 	ft_check_builtin(te);


// 	i = -1;
// 	while (te[++i])
// 		printf("%s\n", te[i]->val);

// 	i = 0;
// 	while (tabo[i])
// 		free(tabo[i++]->val);
// 	i = 0;
// 	while (te[i])
// 		free(te[i++]->val);

// 	ft_lstdel_all(data->env);
// 	free(data);
// 	ft_free((void **)joined);
// 	ft_free((void **)te);
// 	ft_free((void **)tabo);
// 	ft_free((void **)tab);
	
// 	return 0;
// }
