/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:27:18 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 11:38:20 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_env_variable(t_env *env, const char *var, size_t len)
{
	while (env && env->next != NULL)
	{
		if (ft_strncmp(env->value, var, len) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	add_to_env(char *arg, t_env *env, t_env *secret)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		if (*(equal_sign + 1) == '\0')
		{
			if (!is_in_env(secret, arg))
				env_add(arg, secret);
			if (!is_in_env(env, arg))
				env_add(arg, env);
		}
		else
		{
			if (!is_in_env(env, arg))
				env_add(arg, env);
			if (!is_in_env(secret, arg))
				env_add(arg, secret);
		}
	}
	else
	{
		if (!is_in_env(secret, arg))
			env_add(arg, secret);
	}
}

char	*extract_env_path(const char *env_value, size_t len)
{
	char	*oldpwd;
	int		i;
	int		j;
	int		s_alloc;

	s_alloc = ft_strlen(env_value) - len;
	oldpwd = malloc(sizeof(char) * (s_alloc + 1));
	if (!oldpwd)
		return (NULL);
	i = 0;
	j = 0;
	while (env_value[i++])
	{
		if (i > (int)len)
			oldpwd[j++] = env_value[i];
	}
	oldpwd[j] = '\0';
	return (oldpwd);
}

int	ft_env(t_env *env)
{
	while (env && env->next != NULL)
	{
		ft_putendl(env->value);
		env = env->next;
	}
	if (env)
		ft_putendl(env->value);
	return (SUCCESS);
}
