/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbordere <bbordere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 14:32:25 by bbordere          #+#    #+#             */
/*   Updated: 2022/03/28 14:36:10 by bbordere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lexer/lexer.h"

typedef struct s_node_info
{
	t_token			*token;
	struct s_node	*root;
}	t_node_info;


typedef struct	s_node
{
	struct s_node	*r_node;
	struct s_node	*l_node;
	t_node_info		*info;

}	t_node;