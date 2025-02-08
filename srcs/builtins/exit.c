/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:27:22 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 11:16:11 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_exit_errors(char **cmd)
{
	if (cmd[1] && cmd[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR);
		return (1);
	}
	if (cmd[1] && ft_strisnum(cmd[1]) == 0)
	{
		ft_putstr_fd("minishell: exit: ", STDERR);
		ft_putstr_fd(cmd[1], STDERR);
		ft_putendl_fd(": numeric argument required", STDERR);
		return (2);
	}
	return (0);
}

static int	parse_exit_argument(char *arg)
{
	long long	num;

	num = atoll(arg);
	if ((num > 9223372036854775807LL || num < -9223372036854775807LL - 1)
		|| (strlen(arg) > 19 && ft_strisnum(arg)))
	{
		ft_putstr_fd("minishell: exit: ", STDERR);
		ft_putstr_fd(arg, STDERR);
		ft_putendl_fd(": numeric argument required", STDERR);
		return (2);
	}
	return ((int)num);
}

void	mini_exit(t_mini *mini, char **cmd)
{
	int	error_code;

	mini->exit = 1;
	ft_putstr_fd("exit\n", STDERR);
	error_code = handle_exit_errors(cmd);
	if (error_code)
	{
		mini->ret = error_code;
		return ;
	}
	if (cmd[1])
		mini->ret = parse_exit_argument(cmd[1]);
	else
		mini->ret = 0;
}
