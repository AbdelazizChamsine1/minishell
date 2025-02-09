/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:29:05 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 15:47:27 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	type_arg(t_token *token, int separator)
{
	if (ft_strcmp(token->str, "") == 0)
		token->type = EMPTY;
	else if (ft_strcmp(token->str, ">") == 0 && separator == 0)
		token->type = TRUNC;
	else if (ft_strcmp(token->str, ">>") == 0 && separator == 0)
		token->type = APPEND;
	else if (ft_strcmp(token->str, "<") == 0 && separator == 0)
		token->type = INPUT;
	else if (ft_strcmp(token->str, "<<") == 0 && separator == 0)
		token->type = HEREDOC;
	else if (ft_strcmp(token->str, "|") == 0 && separator == 0)
		token->type = PIPE;
	else if (ft_strcmp(token->str, ";") == 0 && separator == 0)
		token->type = END;
	else if (token->prev == NULL || token->prev->type >= TRUNC)
		token->type = CMD;
	else
		token->type = ARG;
}

void	update_token_links(t_token *token, t_token *prev, t_mini *mini)
{
	token->prev->next = token->next;
	if (token->next)
		token->next->prev = token->prev;
	token->prev = prev;
	if (prev)
		token->next = prev->next;
	else
		token->next = mini->start;
	if (!prev)
		prev = token;
	if (prev->next)
		prev->next->prev = token;
	prev->next = token;
	if (mini->start->prev)
		mini->start = mini->start->prev;
}

void	squish_args(t_mini *mini)
{
	t_token	*token;
	t_token	*prev;

	token = mini->start;
	while (token)
	{
		prev = prev_sep(token, NOSKIP);
		if (is_type(token, ARG) && is_types(prev, "TAIH"))
		{
			while (is_last_valid_arg(prev) == 0)
				prev = prev->prev;
			update_token_links(token, prev, mini);
		}
		token = token->next;
	}
}

int	next_alloc(char *line, int *i)
{
	int		count;
	int		j;
	char	c;

	count = 0;
	j = 0;
	c = ' ';
	while (line[*i + j] && (line[*i + j] != ' ' || c != ' '))
	{
		if (c == ' ' && (line[*i + j] == '\'' || line[*i + j] == '\"'))
			c = line[*i + j++];
		else if (c != ' ' && line[*i + j] == c)
		{
			count += 2;
			c = ' ';
			j++;
		}
		else
			j++;
	}
	return (j - count + 2);
}

t_token	*get_tokens(char *line)
{
	t_token	*prev;
	t_token	*next;
	int		i;
	int		sep;

	prev = NULL;
	next = NULL;
	i = 0;
	ft_skip_space(line, &i);
	while (line[i])
	{
		sep = ignore_sep(line, i);
		next = next_token(line, &i);
		next->prev = prev;
		if (prev)
			prev->next = next;
		prev = next;
		type_arg(next, sep);
		ft_skip_space(line, &i);
	}
	if (next)
		next->next = NULL;
	while (next && next->prev)
		next = next->prev;
	return (next);
}
