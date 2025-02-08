/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:27:36 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 11:36:26 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	env_size(char *env)
{
	size_t		i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (i);
}

static void	free_node(t_mini *mini, t_env *env)
{
	if (mini->env == env && env->next == NULL)
	{
		ft_memdel(mini->env->value);
		mini->env->value = NULL;
		mini->env->next = NULL;
		return ;
	}
	ft_memdel(env->value);
	ft_memdel(env);
}

static void	remove_from_env_list(t_env **env_list, char *var, t_mini *mini)
{
	t_env	*current;
	t_env	*tmp;

	if (!env_list || !(*env_list))
		return ;
	current = *env_list;
	if (ft_strncmp(var, current->value, env_size(current->value)) == 0)
	{
		*env_list = current->next;
		free_node(mini, current);
		return ;
	}
	while (current && current->next)
	{
		if (ft_strncmp(var, current->next->value,
				env_size(current->next->value)) == 0)
		{
			tmp = current->next->next;
			free_node(mini, current->next);
			current->next = tmp;
			break ;
		}
		current = current->next;
	}
}

int	ft_unset(char **a, t_mini *mini)
{
	int	i;

	if (!a[1])
		return (SUCCESS);
	i = 1;
	while (a[i])
	{
		remove_from_env_list(&(mini->env), a[i], mini);
		remove_from_env_list(&(mini->secret_env), a[i], mini);
		i++;
	}
	return (SUCCESS);
}
