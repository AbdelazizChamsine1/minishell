/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:22:02 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 14:54:41 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_return_status(int last_ret)
{
	char	*expanded;

	expanded = malloc(12);
	if (!expanded)
		return (NULL);
	snprintf(expanded, 12, "%d", last_ret);
	return (expanded);
}

char	*expand_env_var(char *line, size_t *j, t_env *env)
{
	char	*key;
	char	*value;
	size_t	key_start;

	key_start = *j;
	while (ft_isalnum(line[*j]) || line[*j] == '_')
		(*j)++;
	key = ft_substr(line, key_start, *j - key_start);
	if (!key)
		return (NULL);
	value = get_env_value(key, env);
	free(key);
	return (value);
}

static void	handle_expansion(char *expanded, size_t *i, char *value)
{
	size_t	k;

	k = 0;
	while (value && value[k])
	{
		expanded[*i] = value[k];
		(*i)++;
		k++;
	}
}

char	*expand_heredoc_variable(char *line, size_t *j,
			t_env *env, int last_ret)
{
	char	*value;

	if (line[*j] == '?')
	{
		value = expand_return_status(last_ret);
		(*j)++;
	}
	else
		value = expand_env_var(line, j, env);
	return (value);
}

char	*process_heredoc_expansion(char *line, t_env *env, int last_ret,
			int expand)
{
	char	*expanded;
	size_t	i;
	size_t	j;
	char	*value;

	i = 0;
	j = 0;
	expanded = malloc(BUFF_SIZE);
	if (!expanded)
		return (NULL);
	while (line[j])
	{
		if (expand && line[j] == '$' && (line[j + 1] == '?'
				|| ft_isalpha(line[j + 1]) || line[j + 1] == '_'))
		{
			j++;
			value = expand_heredoc_variable(line, &j, env, last_ret);
			handle_expansion(expanded, &i, value);
			free(value);
		}
		else
			expanded[i++] = line[j++];
	}
	expanded[i] = '\0';
	return (expanded);
}
