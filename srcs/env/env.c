/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:27:42 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/06 14:48:50 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	size_env(t_env *lst)
{
	size_t	lst_len;

	lst_len = 0;
	while (lst != NULL)
	{
		if (lst->value != NULL)
		{
			lst_len += strlen(lst->value);
			lst_len++;
		}
		lst = lst->next;
	}
	return (lst_len);
}

static void	append_env_value(char *env, t_env *lst, int *i)
{
	int	j;

	if (lst->value != NULL)
	{
		j = 0;
		while (lst->value[j])
		{
			env[*i] = lst->value[j];
			(*i)++;
			j++;
		}
	}
}

char	*env_to_str(t_env *lst)
{
	char	*env;
	int		i;

	env = malloc(sizeof(char) * size_env(lst) + 1);
	if (!env)
		return (NULL);
	i = 0;
	while (lst != NULL)
	{
		append_env_value(env, lst, &i);
		if (lst->next != NULL)
			env[i++] = '\n';
		lst = lst->next;
	}
	env[i] = '\0';
	return (env);
}

int	init_env_helper(t_env **env, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (1);
	new->value = ft_strdup(value);
	new->next = NULL;
	*env = new;
	return (0);
}

int	env_init(t_mini *mini, char **env_array)
{
	t_env	*env;
	int		i;

	if (!env_array || !env_array[0])
		return (1);
	if (init_env_helper(&env, env_array[0]))
		return (1);
	mini->env = env;
	i = 1;
	while (env_array[i])
	{
		if (init_env_helper(&(env->next), env_array[i]))
		{
			free_env(mini->env);
			mini->env = NULL;
			return (1);
		}
		env = env->next;
		i++;
	}
	return (0);
}
