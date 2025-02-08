/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:28:46 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 16:17:06 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signum;

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

void	setup_heredoc_signals(void)
{
	struct sigaction	sa_heredoc;

	sa_heredoc.sa_handler = heredoc_signals;
	sigemptyset(&sa_heredoc.sa_mask);
	sa_heredoc.sa_flags = 0;
	sigaction(SIGINT, &sa_heredoc, NULL);
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
