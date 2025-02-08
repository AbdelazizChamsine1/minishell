/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:28:15 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 14:53:17 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_tab_size(t_token *start)
{
	t_token	*token;
	int		i;

	token = start->next;
	i = 2;
	while (token && token->type < TRUNC)
	{
		token = token->next;
		i++;
	}
	return (i);
}

static void	populate_tab(t_token *start, char **tab)
{
	t_token	*token;
	int		i;

	token = start->next;
	i = 1;
	tab[0] = start->str;
	while (token && token->type < TRUNC)
	{
		tab[i++] = token->str;
		token = token->next;
	}
	tab[i] = NULL;
}

char	**cmd_tab(t_token *start)
{
	int		tab_size;
	char	**tab;

	if (!start)
		return (NULL);
	tab_size = get_tab_size(start);
	tab = malloc(sizeof(char *) * tab_size);
	if (!tab)
		return (NULL);
	populate_tab(start, tab);
	return (tab);
}

void	exec_cmd(t_mini *mini, t_token *token)
{
	char	**cmd;
	int		i;

	if (mini->charge == 0)
		return ;
	cmd = cmd_tab(token);
	i = 0;
	while (cmd && cmd[i])
	{
		cmd[i] = expansions(cmd[i], mini->env, mini->ret);
		i++;
	}
	if (cmd && ft_strcmp(cmd[0], "exit") == 0 && has_pipe(token) == 0)
		mini_exit(mini, cmd);
	else if (cmd && is_builtin(cmd[0]))
		mini->ret = exec_builtin(cmd, mini);
	else if (cmd)
		mini->ret = exec_bin(cmd, mini->env, mini);
	free_tab(cmd);
	ft_close(mini->pipin);
	ft_close(mini->pipout);
	mini->pipin = -1;
	mini->pipout = -1;
	mini->charge = 0;
}
