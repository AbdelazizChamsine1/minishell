/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:29:11 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 15:52:53 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ret_size(int ret)
{
	char	*tmp;
	int		ret_len;

	tmp = ft_itoa(ret);
	ret_len = ft_strlen(tmp);
	ft_memdel(tmp);
	return (ret_len);
}

int	get_var_len(const char *arg, int pos, t_env *env, int ret)
{
	char	var_name[BUFF_SIZE];
	char	*var_value;
	int		i;

	i = 0;
	if (arg[pos] == '?')
		return (ret_size(ret));
	if (ft_isdigit(arg[pos]))
		return (0);
	while (arg[pos] && is_env_char(arg[pos]) == 1 && i < BUFF_SIZE)
	{
		var_name[i] = arg[pos];
		pos++;
		i++;
	}
	var_name[i] = '\0';
	var_value = get_env_value(var_name, env);
	i = ft_strlen(var_value);
	ft_memdel(var_value);
	return (i);
}

int	handle_expansion(const char *arg, int *i, t_env *env, int ret)
{
	int	size;

	size = 0;
	(*i)++;
	if (!arg[*i])
	{
		size++;
		return (size);
	}
	if (arg[*i] == EXPANSION)
	{
		size++;
		return (size);
	}
	if ((arg[*i] == '\0' || ft_isalnum(arg[*i]) == 0) && arg[*i] != '?')
		size++;
	else
		size += get_var_len(arg, *i, env, ret);
	if (ft_isdigit(arg[*i]) == 0 && arg[*i] != '?')
	{
		while (arg[*i] && is_env_char(arg[*i]))
			(*i)++;
		(*i)--;
	}
	return (size);
}

int	arg_alloc_len(const char *arg, t_env *env, int ret)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (arg[i])
	{
		if (arg[i] == EXPANSION)
			size += handle_expansion(arg, &i, env, ret);
		else
			size++;
		i++;
	}
	return (size);
}

char	*get_var_value(const char *arg, int pos, t_env *env, int ret)
{
	char	var_name[BUFF_SIZE];
	char	*var_value;
	int		i;

	i = 0;
	if (arg[pos] == '?')
	{
		var_value = ft_itoa(ret);
		return (var_value);
	}
	if (ft_isdigit(arg[pos]))
		return (NULL);
	while (arg[pos] && is_env_char(arg[pos]) == 1 && i < BUFF_SIZE)
	{
		var_name[i] = arg[pos];
		pos++;
		i++;
	}
	var_name[i] = '\0';
	var_value = get_env_value(var_name, env);
	return (var_value);
}
