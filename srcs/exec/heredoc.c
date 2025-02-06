/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 13:49:30 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/06 13:49:30 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	allocate_pipes(int ***pipe_fds, int count, t_mini *mini)
{
	int	i;

	*pipe_fds = malloc(sizeof(int *) * count);
	i = 0;
	while (i < count)
	{
		(*pipe_fds)[i] = malloc(sizeof(int) * 2);
		if (pipe((*pipe_fds)[i]) == -1)
		{
			perror("minishell: pipe error");
			free_pipe_fds(*pipe_fds, i);
			mini->ret = 1;
			mini->no_exec = 1;
			return ;
		}
		i++;
	}
}

void	process_multiple_heredocs(t_mini *mini, t_token *token)
{
	t_token	*current;
	int		heredoc_count;
	int		**pipe_fds;
	int		i;

	current = token;
	heredoc_count = 0;
	while (current && current->type == HEREDOC)
	{
		heredoc_count++;
		if (current->next)
			current = current->next->next;
		else
			current = NULL;
	}
	allocate_pipes(&pipe_fds, heredoc_count, mini);
	i = 0;
	current = token;
	while (i < heredoc_count && current)
	{
		if (!heredoc_to_pipe(mini, current->next, pipe_fds[i][1]))
			close(pipe_fds[i][1]);
		if (current->next)
			current = current->next->next;
		else
			current = NULL;
		i++;
	}
	if (heredoc_count > 0 && dup2(pipe_fds[heredoc_count - 1][0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 error");
		mini->ret = 1;
		mini->no_exec = 1;
	}
	free_pipe_fds(pipe_fds, heredoc_count);
}

int	heredoc_to_pipe(t_mini *mini, t_token *delimiter_token, int write_fd)
{
	char	*line;
	char	*expanded_line;
	char	*delimiter;
	int		expand_variables;

	setup_heredoc_signals();
	expand_variables = (delimiter_token->quote_type == 0);
	delimiter = ft_strdup(delimiter_token->str);
	while (1)
	{
		line = readline("");
		if (g_signum == SIGINT)
			return (free(line), free(delimiter), mini->ret = 130, 0);
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by EOF\n", 2);
			return (free(delimiter), 0);
		}
		if (ft_strcmp(line, delimiter) == 0)
			return (free(line), free(delimiter), 1);
		if (expand_variables)
		{
			expanded_line = expand_heredoc_line(line, mini->env, mini->ret, 1);
			ft_putendl_fd(expanded_line, write_fd);
			free(expanded_line);
		}
		else
			ft_putendl_fd(line, write_fd);
		free(line);
	}
}
