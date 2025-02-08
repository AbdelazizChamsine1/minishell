/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:28:52 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 16:22:24 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	varlcpy(char *new_arg, const char *env_value, int pos)
{
	int		i;

	i = 0;
	while (env_value[i])
		new_arg[pos++] = env_value[i++];
	return (i);
}

static void	insert_var(t_expansions *ex, char *arg, t_env *env, int ret)
{
	char	*env_value;

	if (arg[ex->j] == EXPANSION)
	{
		ex->new_arg[ex->i++] = '$';
		ex->j++;
		return ;
	}
	env_value = get_var_value(arg, ex->j, env, ret);
	ex->i += env_value ? varlcpy(ex->new_arg, env_value, ex->i) : 0;
	ft_memdel(env_value);
	if (arg[ex->j] == '?')
		ex->j++;
	else if (ft_isdigit(arg[ex->j]) == 0)
	{
		while (is_env_char(arg[ex->j]) == 1)
			ex->j++;
	}
	else
		ex->j++;
}

void	expand_argument(t_expansions *ex, char *arg, t_env *env, int ret)
{
	while (arg[ex->j])
	{
		if (arg[ex->j] == EXPANSION)
		{
			ex->j++;
			if (!arg[ex->j] || arg[ex->j] == EXPANSION)
			{
				ex->new_arg[ex->i++] = '$';
				if (arg[ex->j] == EXPANSION)
					continue ;
				break ;
			}
			if ((arg[ex->j] == '\0' || ft_isalnum(arg[ex->j]) == 0)
				&& arg[ex->j] != '?')
				ex->new_arg[ex->i++] = '$';
			else
				insert_var(ex, arg, env, ret);
		}
		else
			ex->new_arg[ex->i++] = arg[ex->j++];
	}
	ex->new_arg[ex->i] = '\0';
}

char	*expansions(char *arg, t_env *env, int ret)
{
	t_expansions	ex;
	int				new_arg_len;

	new_arg_len = arg_alloc_len(arg, env, ret);
	ex.new_arg = malloc(sizeof(char) * (new_arg_len + 2));
	if (!ex.new_arg)
		return (NULL);
	ex.i = 0;
	ex.j = 0;
	expand_argument(&ex, arg, env, ret);
	return (ex.new_arg);
}
