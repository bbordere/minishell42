/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 21:31:05 by bbordere          #+#    #+#             */
/*   Updated: 2022/04/19 21:31:05 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSIONS_H
# define EXPANSIONS_H

# include "../libft/includes/libft.h"
# include <stdio.h>

typedef struct s_data{
	t_list	**env;
}	t_data;

t_list			**ft_init_env(t_list **env, char **envp);
t_data			*ft_init_data(char **envp);
void			*ft_return_dup(char *str, char *dup);
void			*ft_expand_return_code(char *str);
char			*ft_get_var(t_list **env, char *str);
size_t			ft_count_vars(char *str);
size_t			ft_len_vars(char *str, size_t i);
char			**ft_extract_var(char *str);
char			*ft_charjoin(char *str, char c);
char			*ft_get_str(char *str, int mode);

enum	e_type
{
	WORD = 1,
	PIPE = 2,
	CMD = 3,
	ARGS = 4,
	D_PIPE = 5,
	AND = 6,
	D_AND = 7,
	I_PAR = 8,
	O_PAR = 9,
	VAR = 10,
	R_IN = 11,
	R_HERE_DOC = 12,
	R_OUT = 13,
	R_APPEND = 14,
	T_NULL = 15,
	WILDCARD = 16,
	T_FILE = 17,
	D_QUOTE = 18,
	S_QUOTE = 19,
	IN_FILE = 20,
	OUT_FILE = 21,
	DELIMITER = 22,
	OUT_A_FILE = 23
};

typedef struct s_temp
{
	char	**vars;
	char	*str;
	t_list	**env;
	size_t	i;
	size_t	j;
}	t_temp;

typedef struct s_token
{
	char	*val;
	int		type;
}	t_token;

void			ft_init_temp(t_temp *temp, char **vars,
					char *str, t_list **env);
void			ft_free(void **tab);
char			*ft_var(char *res, t_temp *temp);
char			*ft_str_var(char *res, t_temp *temp);
char			**ft_init_expand(char **res, char *str,
					t_temp *temp, t_list **env);
char			*ft_copy_quotes(char *res, t_temp *temp);
char			*ft_expand_str(t_list **env, char *str);
void			ft_expand(t_token **tokens, t_list **env);
void			ft_lstdel_all(t_list **lst);
int				ft_isspecchar(int c);
int				ft_ispar(int c);
int				ft_issep(int c);

#endif