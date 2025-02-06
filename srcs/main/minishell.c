/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:08:07 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/06 15:11:43 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void redir_and_exec(t_mini *mini, t_token *token)
{
	t_token	*prev;
	t_token	*next;
	int		pipe;

	prev = prev_sep(token, NOSKIP);
	next = next_sep(token, NOSKIP);
	pipe = 0;

	if (is_type(prev, TRUNC))
		redir(mini, token, TRUNC);
	else if (is_type(prev, APPEND))
		redir(mini, token, APPEND);
	else if (is_type(prev, INPUT))
		input(mini, token);
	else if (is_type(prev, HEREDOC))
	{
		t_token *first_heredoc = prev;
		while (first_heredoc->prev && first_heredoc->prev->type == HEREDOC)
			first_heredoc = first_heredoc->prev;
		process_multiple_heredocs(mini, first_heredoc);
		while (next && next->type == HEREDOC)
			next = next->next ? next->next->next : NULL;
	}
	else if (is_type(prev, PIPE))
	{
		pipe = minipipe(mini);
	}

	if (next && is_type(next, END) == 0 && pipe != 1)
		redir_and_exec(mini, next->next);
	if ((is_type(prev, END) || is_type(prev, PIPE) || !prev)
		&& pipe != 1 && mini->no_exec == 0)
		exec_cmd(mini, token);
}

void minishell(t_mini *mini)
{
	t_token	*token;
	int		status;

	token = next_run(mini->start, NOSKIP);
	token = (is_types(mini->start, "TAIH")) ? mini->start->next : token;
	while (mini->exit == 0 && token)
	{
		mini->charge = 1;
		mini->parent = 1;
		mini->last = 1;
		redir_and_exec(mini, token);
		reset_std(mini);
		close_fds(mini);
		reset_fds(mini);
		waitpid(-1, &status, 0);
		check_signal_if_recieved(&status);
		status = WEXITSTATUS(status);
		mini->ret = (mini->last == 0) ? status : mini->ret;
		if (mini->parent == 0)
		{
			free_token(mini->start);
			exit(mini->ret);
		}
		mini->no_exec = 0;
		token = next_run(token, SKIP);
	}
}

int	main(int ac, char **av, char **env)
{
	t_mini	mini;

	(void)ac;
	(void)av;
	mini.in = dup(STDIN);
	mini.out = dup(STDOUT);
	mini.exit = 0;
	mini.ret = 0;
	mini.no_exec = 0;
	mini.start = NULL;
	if (!env || !env[0])
		env = create_minimal_env();
	reset_fds(&mini);
	env_init(&mini, env);
	secret_env_init(&mini, env);
	increment_shell_level(mini.env);
	setup_signals();
	while (mini.exit == 0)
	{
		g_signum = 0;
		parse(&mini);
		if (mini.start != NULL && check_line(&mini, mini.start))
			minishell(&mini);
		free_token(mini.start);
	}
	free_env(mini.env);
	free_env(mini.secret_env);
	return (mini.ret);
}
