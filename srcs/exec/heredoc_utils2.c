/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:26:34 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 14:57:39 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_final_input(int fd, t_mini *mini)
{
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 error");
		mini->ret = 1;
		mini->no_exec = 1;
		return (0);
	}
	return (1);
}

void	cleanup_pipes(int **pipe_fds, int count)
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

void	process_heredoc_line(char *line, int write_fd, t_mini *mini,
	int expand)
{
	char	*expanded_line;

	if (expand)
	{
		expanded_line = expand_heredoc_line(line, mini->env, mini->ret, 1);
		ft_putendl_fd(expanded_line, write_fd);
		free(expanded_line);
	}
	else
		ft_putendl_fd(line, write_fd);
	free(line);
}

void	process_final_input_and_cleanup(int **pipe_fds,
		int heredoc_count, t_mini *mini)
{
	if (heredoc_count > 0)
		setup_final_input(pipe_fds[heredoc_count - 1][0], mini);
	cleanup_pipes(pipe_fds, heredoc_count);
}

void	process_heredocs(t_mini *mini, t_token *token)
{
	int	heredoc_count;
	int	**pipe_fds;

	heredoc_count = count_heredocs(token);
	if (heredoc_count == 0)
		return ;
	pipe_fds = init_heredoc_pipes(heredoc_count, mini);
	if (!pipe_fds)
		return ;
	process_heredoc_pipes(mini, token, pipe_fds, heredoc_count);
	process_final_input_and_cleanup(pipe_fds, heredoc_count, mini);
}
