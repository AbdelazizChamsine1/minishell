/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:05:18 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 16:16:45 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_heredocs(t_mini *mini, t_token *prev, t_token **next)
{
	t_token	*first_heredoc;

	first_heredoc = prev;
	while (first_heredoc->prev && first_heredoc->prev->type == HEREDOC)
		first_heredoc = first_heredoc->prev;
	process_heredocs(mini, first_heredoc);
	while (*next && (*next)->type == HEREDOC)
	{
		if ((*next)->next)
			*next = (*next)->next->next;
		else
			*next = NULL;
	}
}

void	handle_redirections(t_mini *mini, t_token *token, int *pipe,
		t_token **next)
{
	t_token	*prev;

	prev = prev_sep(token, NOSKIP);
	*pipe = 0;
	if (is_type(prev, TRUNC))
		redir(mini, token, TRUNC);
	else if (is_type(prev, APPEND))
		redir(mini, token, APPEND);
	else if (is_type(prev, INPUT))
		input(mini, token);
	else if (is_type(prev, HEREDOC))
		handle_heredocs(mini, prev, next);
	else if (is_type(prev, PIPE))
		*pipe = minipipe(mini);
}

void	redir_and_exec(t_mini *mini, t_token *token)
{
	t_token	*prev;
	t_token	*next;
	int		pipe;

	next = next_sep(token, NOSKIP);
	handle_redirections(mini, token, &pipe, &next);
	prev = prev_sep(token, NOSKIP);
	if (next && is_type(next, END) == 0 && pipe != 1)
		redir_and_exec(mini, next->next);
	if ((is_type(prev, END) || is_type(prev, PIPE) || !prev)
		&& pipe != 1 && mini->no_exec == 0)
		exec_cmd(mini, token);
}
