/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 11:39:48 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/06 15:25:31 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_n_option(char **args, int *i)
{
	int	j;

	while (args[*i] && args[*i][0] == '-' && args[*i][1] == 'n')
	{
		j = 2;
		while (args[*i][j] == 'n')
			j++;
		if (args[*i][j] == '\0')
		{
			(*i)++;
			return (1);
		}
		else
			break ;
	}
	return (0);
}

void	print_args(char **args, int i, int n_option)
{
	int	len;

	while (args[i])
	{
		len = 0;
		while (args[i][len])
			len++;
		write(1, args[i], len);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_option)
		write(1, "\n", 1);
}

int	ft_echo(char **args)
{
	int	i;
	int	n_option;

	i = 1;
	n_option = check_n_option(args, &i);
	print_args(args, i, n_option);
	return (SUCCESS);
}

