/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:29:39 by achamsin          #+#    #+#             */
/*   Updated: 2025/01/25 16:21:28 by oismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*next_sep(t_token *token, int skip)
{
	if (token && skip)
		token = token->next;
	while (token && token->type < TRUNC)
		token = token->next;
	return (token);
}

t_token	*prev_sep(t_token *token, int skip)
{
	if (token && skip)
		token = token->prev;
	while (token && token->type < TRUNC)
		token = token->prev;
	return (token);
}

t_token	*next_run(t_token *token, int skip)
{
	if (token && skip)
		token = token->next;
	while (token && token->type != CMD)
	{
		if (token && token->type == HEREDOC)
		{
			while (token && token->type == HEREDOC)
			{
				token = token->next;
				if (token)
					token = token->next;
			}
		}
		if (token)
		{
			token = token->next;
			if (token && token->type == CMD && token->prev == NULL)
				;
			else if (token && token->type == CMD
				&& token->prev && token->prev->type < END)
				token = token->next;
		}
	}
	return (token);
}
