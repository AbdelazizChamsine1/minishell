/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:27:22 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/06 15:28:11 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void mini_exit(t_mini *mini, char **cmd) {
    long long num;

    mini->exit = 1;
    ft_putstr_fd("exit ", STDERR);
    cmd[1] ? ft_putendl_fd("", STDERR) : ft_putendl_fd("", STDERR);

    if (cmd[1] && cmd[2]) {
        mini->ret = 1;
        ft_putendl_fd("minishell: exit: too many arguments", STDERR);
    } else if (cmd[1] && ft_strisnum(cmd[1]) == 0) {
        mini->ret = 2;
        ft_putstr_fd("minishell: exit: ", STDERR);
        ft_putstr_fd(cmd[1], STDERR);
        ft_putendl_fd(": numeric argument required", STDERR);
    } else if (cmd[1]) {
        num = atoll(cmd[1]);
        if ((num > 9223372036854775807LL || num < -9223372036854775807LL - 1) || (strlen(cmd[1]) > 19 && ft_strisnum(cmd[1]))) {
            mini->ret = 2;
            ft_putstr_fd("minishell: exit: ", STDERR);
            ft_putstr_fd(cmd[1], STDERR);
            ft_putendl_fd(": numeric argument required", STDERR);
        } else {
            mini->ret = (int)num;
        }
    } else {
        mini->ret = 0;
    }
}
