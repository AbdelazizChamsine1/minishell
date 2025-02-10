static int	init_pipe(int **pipe_fds, int i)
{
	pipe_fds[i] = malloc(sizeof(int) * 2);
	if (!pipe_fds[i])
		return (0);
	if (pipe(pipe_fds[i]) == -1)
	{
		cleanup_pipes(pipe_fds, i);
		return (0);
	}
	return (1);
}

int	**init_heredoc_pipes(int count, t_mini *mini)
{
	int	**pipe_fds;
	int	i;

	pipe_fds = malloc(sizeof(int *) * count);
	if (!pipe_fds)
	{
		mini->ret = 1;
		mini->no_exec = 1;
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		if (!init_pipe(pipe_fds, i))
		{
			mini->ret = 1;
			mini->no_exec = 1;
			return (NULL);
		}
		i++;
	}
	return (pipe_fds);
}

static int	handle_heredoc_input(char *line, t_token *delim_token, t_mini *mini,
	int write_fd)
{
	if (g_signum == SIGINT)
	{
		free(line);
		mini->ret = 130;
		return (0);
	}
	if (!line)
	{
		ft_putstr_fd("minishell: warning: here-document at EOF\n", 2);
		return (0);
	}
	if (ft_strcmp(line, delim_token->str) == 0)
	{
		free(line);
		setup_signals();
		return (1);
	}
	process_heredoc_line(line, write_fd, mini,
		(delim_token->quote_type == 0));
	return (2);
}

int	heredoc_to_pipe(t_mini *mini, t_token *delim_token, int write_fd)
{
	char	*line;
	int		result;

	setup_heredoc_signals();
	if (!delim_token || !delim_token->str)
		return (0);
	while (1)
	{
		line = readline("");
		result = handle_heredoc_input(line, delim_token, mini, write_fd);
		if (result == 0 || result == 1)
			return (result);
	}
}

int	process_single_heredoc(t_mini *mini, t_token *token, int write_fd)
{
	if (!heredoc_to_pipe(mini, token, write_fd))
	{
		close(write_fd);
		return (1);
	}
	close(write_fd);
	return (0);
}


char	*expand_return_status(int last_ret)
{
	char	*expanded;

	expanded = malloc(12);
	if (!expanded)
		return (NULL);
	snprintf(expanded, 12, "%d", last_ret);
	return (expanded);
}

char	*expand_env_var(char *line, size_t *j, t_env *env)
{
	char	*key;
	char	*value;
	size_t	key_start;

	key_start = *j;
	while (ft_isalnum(line[*j]) || line[*j] == '_')
		(*j)++;
	key = ft_substr(line, key_start, *j - key_start);
	if (!key)
		return (NULL);
	value = get_env_value(key, env);
	free(key);
	return (value);
}

static void	handle_expansion(char *expanded, size_t *i, char *value)
{
	size_t	k;

	k = 0;
	while (value && value[k])
	{
		expanded[*i] = value[k];
		(*i)++;
		k++;
	}
}

char	*expand_heredoc_variable(char *line, size_t *j,
			t_env *env, int last_ret)
{
	char	*value;

	if (line[*j] == '?')
	{
		value = expand_return_status(last_ret);
		(*j)++;
	}
	else
		value = expand_env_var(line, j, env);
	return (value);
}

char	*process_heredoc_expansion(char *line, t_env *env, int last_ret,
			int expand)
{
	char	*expanded;
	size_t	i;
	size_t	j;
	char	*value;

	i = 0;
	j = 0;
	expanded = malloc(BUFF_SIZE);
	if (!expanded)
		return (NULL);
	while (line[j])
	{
		if (expand && line[j] == '$' && (line[j + 1] == '?'
				|| ft_isalpha(line[j + 1]) || line[j + 1] == '_'))
		{
			j++;
			value = expand_heredoc_variable(line, &j, env, last_ret);
			handle_expansion(expanded, &i, value);
			free(value);
		}
		else
			expanded[i++] = line[j++];
	}
	expanded[i] = '\0';
	return (expanded);
}


int	setup_final_input(int fd, t_mini *mini)
{
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 error");
		mini->ret = 1;
		mini->no_exec = 1;
		return (0);
	}
	return (1);
}

void	cleanup_pipes(int **pipe_fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
		free(pipe_fds[i]);
		i++;
	}
	free(pipe_fds);
}

void	process_heredoc_line(char *line, int write_fd, t_mini *mini,
	int expand)
{
	char	*expanded_line;

	if (expand)
	{
		expanded_line = expand_heredoc_line(line, mini->env, mini->ret, 1);
		ft_putendl_fd(expanded_line, write_fd);
		free(expanded_line);
	}
	else
		ft_putendl_fd(line, write_fd);
	free(line);
}

void	process_final_input_and_cleanup(int **pipe_fds,
		int heredoc_count, t_mini *mini)
{
	if (heredoc_count > 0)
		setup_final_input(pipe_fds[heredoc_count - 1][0], mini);
	cleanup_pipes(pipe_fds, heredoc_count);
}

void	process_heredocs(t_mini *mini, t_token *token)
{
	int	heredoc_count;
	int	**pipe_fds;

	heredoc_count = count_heredocs(token);
	if (heredoc_count == 0)
		return ;
	pipe_fds = init_heredoc_pipes(heredoc_count, mini);
	if (!pipe_fds)
		return ;
	process_heredoc_pipes(mini, token, pipe_fds, heredoc_count);
	process_final_input_and_cleanup(pipe_fds, heredoc_count, mini);
}


char	*expand_heredoc_line(char *line, t_env *env, int last_ret,
    int expand)
{
return (process_heredoc_expansion(line, env, last_ret, expand));
}

int	count_heredocs(t_token *token)
{
t_token	*current;
int		count;

current = token;
count = 0;
while (current && current->type == HEREDOC)
{
count++;
if (current->next)
    current = current->next->next;
else
    current = NULL;
}
return (count);
}

void	process_heredoc_pipes(t_mini *mini, t_token *token,
    int **pipe_fds, int heredoc_count)
{
t_token	*current;
int		i;

i = 0;
current = token;
while (i < heredoc_count && current)
{
if (!process_single_heredoc(mini, current->next, pipe_fds[i][1]))
{
    if (i < heredoc_count - 1)
    {
        if (current->next)
            current = current->next->next;
        else
            current = NULL;
        i++;
        continue ;
    }
}
if (current->next)
    current = current->next->next;
else
    current = NULL;
i++;
}
}
