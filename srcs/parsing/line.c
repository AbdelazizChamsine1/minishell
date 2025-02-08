/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 16:39:15 by oismail           #+#    #+#             */
/*   Updated: 2025/02/08 16:17:23 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*space_alloc(char *line)
{
	char	*new;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (line[i])
	{
		if (is_sep(line, i))
			count++;
		i++;
	}
	new = malloc(sizeof(char) * (i + 2 * count + 1));
	if (!new)
		return (NULL);
	return (new);
}

int	handle_special_chars(char *new, char *line, int *i, int *j)
{
	if (quotes(line, *i) != 2 && line[*i] == '$' && *i && line[*i - 1] != '\\')
	{
		new[(*j)++] = (char)(-line[(*i)++]);
		return (1);
	}
	if (quotes(line, *i) == 0 && is_sep(line, *i))
	{
		new[(*j)++] = ' ';
		new[(*j)++] = line[(*i)++];
		if (quotes(line, *i) == 0 && line[*i] == '<')
			new[(*j)++] = line[(*i)++];
		else if (quotes(line, *i) == 0 && line[*i] == '>')
			new[(*j)++] = line[(*i)++];
		new[(*j)++] = ' ';
		return (1);
	}
	return (0);
}

char	*space_line(char *line)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = space_alloc(line);
	while (new && line[i])
	{
		if (!handle_special_chars(new, line, &i, &j))
			new[j++] = line[i++];
	}
	new[j] = '\0';
	ft_memdel(line);
	return (new);
}

ssize_t	read_stat_file(char *buffer, size_t buffer_size)
{
	int		fd;
	ssize_t	bytes_read;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd == -1)
	{
		perror("Failed to open /proc/self/stat");
		return (-1);
	}
	bytes_read = read(fd, buffer, buffer_size - 1);
	if (bytes_read == -1)
	{
		perror("Failed to read /proc/self/stat");
		close(fd);
		return (-1);
	}
	buffer[bytes_read] = '\0';
	close(fd);
	return (bytes_read);
}

void	parse(t_mini *mini)
{
	char	*line;
	t_token	*token;

	line = readline("\033[0;36m\033[1mminishell ▸ \033[0m");
	set_ctrl_d(mini, line);
	if (*line)
		add_history(line);
	check_signal_if_recieved(&mini->ret);
	if (quote_check(mini, &line))
		return ;
	line = space_line(line);
	mini->start = get_tokens(line);
	ft_memdel(line);
	squish_args(mini);
	token = mini->start;
	while (token)
	{
		if (is_type(token, ARG))
			type_arg(token, 0);
		token = token->next;
	}
}
