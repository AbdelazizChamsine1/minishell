/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:47:19 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/06 14:49:24 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	secret_env_init(t_mini *mini, char **env_array)
{
	t_env	*env;
	int		i;

	if (!env_array || !env_array[0])
		return (1);
	if (init_env_helper(&env, env_array[0]))
		return (1);
	mini->secret_env = env;
	i = 1;
	while (env_array[i])
	{
		if (init_env_helper(&(env->next), env_array[i]))
		{
			free_env(mini->secret_env);
			mini->secret_env = NULL;
			return (1);
		}
		env = env->next;
		i++;
	}
	return (0);
}

char	**create_minimal_env(void)
{
	char	**env;
	char	*str;

	str = "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	env = malloc(sizeof(char *) * 5);
	if (!env)
		return (NULL);
	env[0] = ft_strdup("PWD=/User/");
	env[1] = ft_strdup("SHELL=/bin/bash");
	env[2] = ft_strdup("SHLVL=1");
	env[3] = ft_strdup(str);
	env[4] = NULL;
	return (env);
}
