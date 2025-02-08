/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:08:07 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 16:09:58 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_commands(t_mini *mini, t_token *token)
{
	int	status;

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
		if (mini->last == 0)
			mini->ret = status;
		if (mini->parent == 0)
		{
			free_token(mini->start);
			exit(mini->ret);
		}
		mini->no_exec = 0;
		token = next_run(token, SKIP);
	}
}

void	minishell(t_mini *mini)
{
	t_token	*token;

	token = next_run(mini->start, NOSKIP);
	if (is_types(mini->start, "TAIH"))
		token = mini->start->next;
	execute_commands(mini, token);
}

void	main_init(t_mini *mini)
{
	mini->in = dup(STDIN);
	mini->out = dup(STDOUT);
	mini->exit = 0;
	mini->ret = 0;
	mini->no_exec = 0;
	mini->start = NULL;
}

int	main(int ac, char **av, char **env)
{
	t_mini	mini;

	(void)ac;
	(void)av;
	main_init(&mini);
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
