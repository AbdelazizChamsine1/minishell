/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:27:36 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/06 15:30:08 by achamsin         ###   ########.fr       */
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

int ft_unset(char **a, t_mini *mini)
{
    t_env *env;
    t_env *tmp;
    t_env *secret;
    t_env *tmp_secret;
    int i;

    if (!(a[1]))
        return (SUCCESS);
    
    i = 1; // Start from the first argument after "unset"
    while (a[i])
    {
        env = mini->env;
        if (ft_strncmp(a[i], env->value, env_size(env->value)) == 0)
        {
            mini->env = (env->next) ? env->next : mini->env;
            free_node(mini, env);
        }
        else
        {
            while (env && env->next)
            {
                if (ft_strncmp(a[i], env->next->value, env_size(env->next->value)) == 0)
                {
                    tmp = env->next->next;
                    free_node(mini, env->next);
                    env->next = tmp;
                    break;
                }
                env = env->next;
            }
        }
        secret = mini->secret_env;
        if (ft_strncmp(a[i], secret->value, env_size(secret->value)) == 0)
        {
            mini->secret_env = (secret->next) ? secret->next : mini->secret_env;
            free_node(mini, secret);
        }
        else
        {
            while (secret && secret->next)
            {
                if (ft_strncmp(a[i], secret->next->value, env_size(secret->next->value)) == 0)
                {
                    tmp_secret = secret->next->next;
                    free_node(mini, secret->next);
                    secret->next = tmp_secret;
                    break;
                }
                secret = secret->next;
            }
        }
        i++; // Move to the next argument
    }
    return (SUCCESS);
}