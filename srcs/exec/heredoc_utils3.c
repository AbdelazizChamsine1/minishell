/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:49:25 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 15:06:43 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_heredoc_line(char *line, t_env *env, int last_ret,
			int expand)
{
	return (process_heredoc_expansion(line, env, last_ret, expand));
}

int	count_heredocs(t_token *token)
{
	t_token	*current;
	int		count;

	current = token;
	count = 0;
	while (current && current->type == HEREDOC)
	{
		count++;
		if (current->next)
			current = current->next->next;
		else
			current = NULL;
	}
	return (count);
}

void	process_heredoc_pipes(t_mini *mini, t_token *token,
			int **pipe_fds, int heredoc_count)
{
	t_token	*current;
	int		i;

	i = 0;
	current = token;
	while (i < heredoc_count && current)
	{
		if (!process_single_heredoc(mini, current->next, pipe_fds[i][1]))
		{
			if (i < heredoc_count - 1)
			{
				if (current->next)
					current = current->next->next;
				else
					current = NULL;
				i++;
				continue ;
			}
		}
		if (current->next)
			current = current->next->next;
		else
			current = NULL;
		i++;
	}
}
