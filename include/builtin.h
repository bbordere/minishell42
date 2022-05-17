/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdesmet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 10:38:00 by tdesmet           #+#    #+#             */
/*   Updated: 2022/05/13 15:32:53 by tdesmet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

int     ft_cd(t_data *data, char **str);
int     ft_cd_error(char **str);
int     ft_cd_home(t_data *data, char **str);
char    *ft_getenv(t_list **env, char *str);

int     ft_echo(char **args);
int     ft_check_option(char **args);

int     ft_env(t_list **env);

t_list  **ft_export(t_list **env, char **arg);
void    ft_add_env(t_list **env, char *str, char *name);
char    *ft_name_env(char *str);
int     ft_check_export_arg(char *str);
void    ft_sort_export(t_list **env);
void    ft_print_export(char *name);
int     ft_env_size(t_list **env);

int     ft_pwd(t_data *data);
void    ft_update_env(t_data *data, t_list **env, char *name, char *value);
char	*ft_pwd_str(void);

t_list  **ft_unset(t_list **env, char **arg);
void    ft_del_env(t_list **env, char *str);
char    *ft_check_unset_arg(char *str);

#endif