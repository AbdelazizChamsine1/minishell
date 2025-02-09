/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 15:45:35 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 16:18:04 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*allocate_token(char *line, int *i)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!(token))
		return (NULL);
	token->str = malloc(sizeof(char) * next_alloc(line, i));
	if (!(token->str))
	{
		free(token);
		return (NULL);
	}
	token->quote_type = 0;
	return (token);
}

t_token	*next_token(char *line, int *i)
{
	t_token	*token;

	token = allocate_token(line, i);
	if (!token)
		return (NULL);
	process_token_content(line, i, token);
	return (token);
}

int	print_syntax_error(t_mini *mini, char *msg)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", STDERR);
	ft_putstr_fd(msg, STDERR);
	ft_putendl_fd("'", STDERR);
	mini->ret = 2;
	return (0);
}

void	process_token_content(char *line, int *i, t_token *token)
{
	int		j;
	char	c;

	j = 0;
	c = ' ';
	while (line[*i] && (line[*i] != ' ' || c != ' '))
	{
		if (c == ' ' && (line[*i] == '\'' || line[*i] == '\"'))
		{
			c = line[(*i)++];
			token->quote_type = 1;
		}
		else if (c != ' ' && line[*i] == c)
		{
			c = ' ';
			(*i)++;
		}
		else if (line[*i] == '\\' && (*i)++)
			token->str[j++] = line[(*i)++];
		else
			token->str[j++] = line[(*i)++];
	}
	token->str[j] = '\0';
}
