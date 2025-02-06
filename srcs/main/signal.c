/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:28:46 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/06 11:20:18 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_signum;

void	set_ctrl_d(t_mini *mini, char *prompt)
{
	if (!prompt)
	{
		free_env((mini->env));
		rl_clear_history();
		printf("exit\n");
		exit(0);
	}
}

void	check_signal_if_recieved(int *status)
{
	if (g_signum == SIGINT)
	{
		*status = 130;
		g_signum = 0;
	}
	else if (g_signum == SIGQUIT)
	{
		*status = 131;
		g_signum = 0;
	}
}

void setup_heredoc_signals(void) {
    struct sigaction sa_heredoc;
    
    sa_heredoc.sa_handler = sigint_handler2;
    sigemptyset(&sa_heredoc.sa_mask);
    sa_heredoc.sa_flags = 0;
    sigaction(SIGINT, &sa_heredoc, NULL);
}

void sigint_handler(int signum) {
    g_signum = signum;
    write(STDERR_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
}

void sigint_handler2(int signum) {
    g_signum = signum;
    write(STDERR_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    if (signum == SIGINT) {
        close(STDIN_FILENO);
    }
}

void setup_signals(void) {
    struct sigaction sa;
    
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}