/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:00:00 by your_login        #+#    #+#             */
/*   Updated: 2024/02/08 10:00:00 by your_login       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_pipe(int **pipe_fds, int i)
{
	pipe_fds[i] = malloc(sizeof(int) * 2);
	if (!pipe_fds[i])
		return (0);
	if (pipe(pipe_fds[i]) == -1)
	{
		cleanup_pipes(pipe_fds, i);
		return (0);
	}
	return (1);
}

int	**init_heredoc_pipes(int count, t_mini *mini)
{
	int	**pipe_fds;
	int	i;

	pipe_fds = malloc(sizeof(int *) * count);
	if (!pipe_fds)
	{
		mini->ret = 1;
		mini->no_exec = 1;
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		if (!init_pipe(pipe_fds, i))
		{
			mini->ret = 1;
			mini->no_exec = 1;
			return (NULL);
		}
		i++;
	}
	return (pipe_fds);
}

static int	handle_heredoc_input(char *line, t_token *delim_token, t_mini *mini,
	int write_fd)
{
	if (g_signum == SIGINT)
	{
		free(line);
		mini->ret = 130;
		return (0);
	}
	if (!line)
	{
		ft_putstr_fd("minishell: warning: here-document at EOF\n", 2);
		return (0);
	}
	if (ft_strcmp(line, delim_token->str) == 0)
	{
		free(line);
		setup_signals();
		return (1);
	}
	process_heredoc_line(line, write_fd, mini,
		(delim_token->quote_type == 0));
	return (2);
}

int	heredoc_to_pipe(t_mini *mini, t_token *delim_token, int write_fd)
{
	char	*line;
	int		result;

	setup_heredoc_signals();
	if (!delim_token || !delim_token->str)
		return (0);
	while (1)
	{
		line = readline("");
		result = handle_heredoc_input(line, delim_token, mini, write_fd);
		if (result == 0 || result == 1)
			return (result);
	}
}

int	process_single_heredoc(t_mini *mini, t_token *token, int write_fd)
{
	if (!heredoc_to_pipe(mini, token, write_fd))
	{
		close(write_fd);
		return (1);
	}
	close(write_fd);
	return (0);
}
