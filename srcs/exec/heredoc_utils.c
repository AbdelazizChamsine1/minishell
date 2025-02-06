/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 13:56:43 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/06 14:04:54 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_exit_status(char *expanded, size_t *i, int last_ret)
{
	char	ret_str[12];
	size_t	k;

	snprintf(ret_str, 12, "%d", last_ret);
	k = 0;
	while (ret_str[k])
	{
		expanded[*i] = ret_str[k];
		(*i)++;
		k++;
	}
}

static void	expand_variable(char *expanded, size_t *i, char *line,
				size_t *j, t_env *env)
{
	char	*key;
	char	*value;
	size_t	key_start;
	size_t	k;

	key_start = *j;
	while (ft_isalnum(line[*j]) || line[*j] == '_')
		(*j)++;
	key = strndup(line + key_start, *j - key_start);
	value = get_env_value(key, env);
	free(key);
	if (value)
	{
		k = 0;
		while (value[k])
		{
			expanded[*i] = value[k];
			(*i)++;
			k++;
		}
		free(value);
	}
}

char	*expand_heredoc_line(char *line, t_env *env, int last_ret,
				int expand_variables)
{
	char	*expanded;
	size_t	i;
	size_t	j;

	expanded = malloc(BUFF_SIZE);
	if (!expanded)
		return (NULL);
	i = 0;
	j = 0;
	while (line[j])
	{
		if (expand_variables && line[j] == '$'
			&& (line[j + 1] == '?' || ft_isalpha(line[j + 1])
				|| line[j + 1] == '_'))
		{
			j++;
			if (line[j] == '?')
				expand_exit_status(expanded, &i, last_ret);
			else
				expand_variable(expanded, &i, line, &j, env);
		}
		else
			expanded[i++] = line[j++];
	}
	expanded[i] = '\0';
	return (expanded);
}

void	free_pipe_fds(int **pipe_fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
		free(pipe_fds[i]);
		i++;
	}
	free(pipe_fds);
}
