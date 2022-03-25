/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:05:53 by bbordere          #+#    #+#             */
/*   Updated: 2022/03/25 11:05:53 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include <stdio.h>

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
	T_NULL = 15
};

typedef struct s_token
{
	char	*val;
	int		type;
}	t_token;

int	ft_isspecchar(int c);
int	ft_issep(int c);
int	ft_ispar(int c);
void	ft_skip_spaces(char *str, size_t *i);
void	ft_count_word_sep(char *str, size_t *i, size_t *nb);
void	ft_count_word_spec(char *str, size_t *i, size_t *nb);
void	ft_count_word(char *str, size_t *i, size_t *nb);
size_t	ft_block_count(char *str);
int	ft_init_op(t_token *token, char *val);
t_token	*ft_init_token(char *val);
t_token	**ft_tokenize(char **tab);