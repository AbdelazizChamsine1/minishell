/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:27:53 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 11:40:04 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	invalid_lvl(const char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (1);
		i++;
	}
	return (0);
}

static int	get_lvl(const char *str)
{
	int	i;
	int	sign;
	int	num;

	i = 0;
	sign = 1;
	num = 0;
	ft_skip_spacenl(str, &i);
	if (invalid_lvl(str))
		return (0);
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
		num = num * 10 + (str[i++] - '0');
	return (num * sign);
}

// void	increment_shell_level(t_env *env)
// {
// 	int		shell_level;
// 	char	env_name[BUFF_SIZE];
// 	char	*shlvl;
// 	char	*shell_level_value;

// 	shell_level_value = get_env_value("SHLVL", env);
// 	if (!shell_level_value || ft_strcmp(shell_level_value, "") == 0)
// 	{
// 		ft_memdel(shell_level_value);
// 		return ;
// 	}
// 	shell_level = get_lvl(shell_level_value) + 1;
// 	ft_memdel(shell_level_value);
// 	while (env)
// 	{
// 		get_env_name(env_name, env->value);
// 		if (ft_strcmp("SHLVL", env_name) == 0)
// 		{
// 			ft_memdel(env->value);
// 			shlvl = ft_itoa(shell_level);
// 			env->value = ft_strjoin("SHLVL=", shlvl);
// 			ft_memdel(shlvl);
// 			return ;
// 		}
// 		env = env->next;
// 	}
// }

void	update_shell_level_in_env(t_env *env, int shell_level)
{
	char	env_name[BUFF_SIZE];
	char	*shlvl;

	while (env)
	{
		get_env_name(env_name, env->value);
		if (ft_strcmp("SHLVL", env_name) == 0)
		{
			ft_memdel(env->value);
			shlvl = ft_itoa(shell_level);
			env->value = ft_strjoin("SHLVL=", shlvl);
			ft_memdel(shlvl);
			return ;
		}
		env = env->next;
	}
}

void	increment_shell_level(t_env *env)
{
	int		shell_level;
	char	*shell_level_value;

	shell_level_value = get_env_value("SHLVL", env);
	if (!shell_level_value || ft_strcmp(shell_level_value, "") == 0)
	{
		ft_memdel(shell_level_value);
		return ;
	}
	shell_level = get_lvl(shell_level_value) + 1;
	ft_memdel(shell_level_value);
	update_shell_level_in_env(env, shell_level);
}
