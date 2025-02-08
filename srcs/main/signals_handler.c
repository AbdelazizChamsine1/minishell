/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:41 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 12:16:57 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int signum)
{
	g_signum = signum;
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	heredoc_signals(int signum)
{
	g_signum = signum;
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	if (signum == SIGINT)
		close(STDIN_FILENO);
}

void	set_ctrl_d(t_mini *mini, char *prompt)
{
	if (!prompt)
	{
		free_env(mini->env);
		free_env(mini->secret_env);
		rl_clear_history();
		printf("exit\n");
		exit(0);
	}
}
