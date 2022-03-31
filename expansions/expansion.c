/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:45:38 by bbordere          #+#    #+#             */
/*   Updated: 2022/03/31 16:20:20 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lexer/lexer.h"

typedef struct s_data{
	t_list		**env;
}	t_data;

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

char	*ft_getenv(t_list **env, char *str)
{
	t_list	*temp;
	size_t	i;
	int		lenght;

	i = 0;
	temp = *env;
	lenght = ft_strlen(str);
	while (temp && ft_strncmp(str, temp->content, lenght))
		temp = temp->next;
	if (!temp)
		return (NULL);
	return (&(temp->content)[lenght + 1]);
}

char	*ft_expand_str(char *str, t_list **env)
{
	size_t	i;
	t_token	**tokens;
	char	**join;
	char	**lexer;

	i = 0;
	lexer = ft_lexer(str);
	tokens = ft_tokenize(lexer);
	free(str);
	while (tokens[i])
	{
		if (tokens[i]->type == VAR)
			tokens[i]->val = ft_getenv(env, tokens[i]->val + 1);
		i++;
	}
	join = ft_join(tokens);
	ft_free((void **)tokens);
	ft_free((void **)lexer);
	return (join[0]);
	// while (str[i])
	// {
	// 	if (str[i] == '$' && str[i + 1])
	// 	{
	// 		len = 0;
	// 		start = i;
	// 		while (str[i] && !ft_isspace(str[i]) && !ft_issep(str[i]) && !ft_ispar(str[i]) && !ft_isspecchar(str[i]))
	// 		{
	// 			len++;
	// 			i++;
	// 		}
	// 		var = ft_substr(str, start, len);
	// 		printf("%s\n", var);
	// 	}
	// 	i++;
	// }
	
}

void	ft_expand(t_token **tokens, t_list **env)
{
	size_t	i;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == VAR)
			tokens[i]->val = ft_getenv(env, tokens[i]->val + 1);
		else if (tokens[i]->type == ARGS)
			tokens[i]->val = ft_expand_str(ft_substr(tokens[i]->val, 1, ft_strlen(tokens[i]->val) - 2), env);
		i++;
	}
	
}

int main(int ac, char **av, char **env)
{
	t_data	*data;

	data = ft_init_data(env);

	char *str = av[1];
	char **tab;
	t_token	**tabo;

	tab = ft_lexer(str);
	tabo = ft_tokenize(tab);

	ft_expand(tabo, data->env);
	char **joined;
	joined = ft_join(tabo);
	int i = 0;
	// printf("\t\t%lu\n", ft_block_count(str));
	t_token	**te = ft_tokenize(joined);
	ft_check_grammar(te) ? printf("OK\n") : printf("KO\n");
	ft_check_builtin(te);
	while (te[i])
	{
		printf("%s\n", te[i]->val);
		i++;
	}
	ft_free((void **)joined);
	ft_free((void **)te);
	ft_free((void **)tabo);
	ft_free((void **)tab);
	
	return 0;
}
