/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:27:25 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 11:32:23 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_error(int error, const char *arg)
{
	int		i;

	if (error == -1)
		ft_putstr_fd("export: not valid in this context: ", STDERR);
	else if (error == 0 || error == -3)
	{
		ft_putstr_fd("export: not a valid identifier: ", STDERR);
	}
	i = 0;
	while (arg[i] && (arg[i] != '=' || error == -3))
	{
		write(STDERR, &arg[i], 1);
		i++;
	}
	write(STDERR, "\n", 1);
	return (ERROR);
}

int	env_add(const char *value, t_env *env)
{
	t_env	*new;
	t_env	*tmp;

	if (env && env->value == NULL)
	{
		env->value = ft_strdup(value);
		return (SUCCESS);
	}
	new = malloc(sizeof(t_env));
	if (!(new))
		return (-1);
	new->value = ft_strdup(value);
	while (env && env->next && env->next->next)
		env = env->next;
	tmp = env->next;
	env->next = new;
	new->next = tmp;
	return (SUCCESS);
}

char	*get_env_name(char *dest, const char *src)
{
	int		i;

	i = 0;
	while (src[i] && src[i] != '=' && ft_strlen(src) < BUFF_SIZE)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static int	handle_invalid_export(t_mini *mini, char *arg)
{
	int	error_ret;

	error_ret = is_valid_env(arg);
	if (arg[0] == '=')
		error_ret = -3;
	if (error_ret <= 0)
	{
		print_error(error_ret, arg);
		mini->ret = 1;
		return (1);
	}
	return (0);
}

int	ft_export(char **args, t_env *env, t_env *secret, t_mini *mini)
{
	int	i;

	mini->ret = 0;
	if (!args[1])
	{
		print_sorted_env(secret);
		return (SUCCESS);
	}
	i = 1;
	while (args[i])
	{
		if (handle_invalid_export(mini, args[i]) == 0)
			add_to_env(args[i], env, secret);
		i++;
	}
	return (SUCCESS);
}
