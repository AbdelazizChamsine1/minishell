/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:27:25 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/06 15:28:54 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_error(int error, const char *arg, t_mini *mini)
{
	int		i;

	if (error == -1)
		ft_putstr_fd("export: not valid in this context: ", STDERR);
	else if (error == 0 || error == -3)
	{
		ft_putstr_fd("export: not a valid identifier: ", STDERR);
		mini->ret = 1;
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

int	is_in_env(t_env *env, char *args)
{
	char	var_name[BUFF_SIZE];
	char	env_name[BUFF_SIZE];

	get_env_name(var_name, args);
	while (env)
	{
		get_env_name(env_name, env->value);
		if (ft_strcmp(var_name, env_name) == 0)
		{
			ft_memdel(env->value);
			env->value = ft_strdup(args);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

int ft_export(char **args, t_env *env, t_env *secret, t_mini *mini)
{
    int error_ret;
    char *equal_sign;
    int i = 1;

    if (!args[1])
    {
        print_sorted_env(secret);
        return (SUCCESS);
    }
    while (args[i])
    {
        error_ret = is_valid_env(args[i]);
        if (args[i][0] == '=')
            error_ret = -3;
        if (error_ret <= 0)
        {
            print_error(error_ret, args[i], mini);
            i++;
            continue;
        }
        equal_sign = ft_strchr(args[i], '=');
        if (equal_sign)
        {
            if (*(equal_sign + 1) == '\0')
            {
                if (is_in_env(env, args[i]) == 0)
                {
                    // Do nothing if not in env
                }
                if (!is_in_env(secret, args[i]))
                    env_add(args[i], secret);
            }
            else
            {
                if (!is_in_env(env, args[i]))
                    env_add(args[i], env);
                if (!is_in_env(secret, args[i]))
                    env_add(args[i], secret);
            }
        }
        else
        {
            if (!is_in_env(secret, args[i]))
                env_add(args[i], secret);
        }
        i++;
    }
    return (SUCCESS);
}
