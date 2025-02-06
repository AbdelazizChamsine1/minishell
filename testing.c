
int g_signum;

void	set_ctrl_d(t_mini *mini, char *prompt)
{
	if (!prompt)
	{
		free_env((mini->env));
		rl_clear_history();
		printf("exit\n");
		exit(0);
	}
}

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

void setup_heredoc_signals(void) {
    struct sigaction sa_heredoc;
    
    sa_heredoc.sa_handler = sigint_handler;
    sigemptyset(&sa_heredoc.sa_mask);
    sa_heredoc.sa_flags = 0;
    sigaction(SIGINT, &sa_heredoc, NULL);
}

void sigint_handler(int signum) {
    g_signum = signum;
    write(STDERR_FILENO, "\n", 1);
    if (waitpid(-1, NULL, WNOHANG) == -1) {
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void setup_signals(void) {
    struct sigaction sa;
    
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}


char *expand_heredoc_line(char *line, t_env *env, int last_ret, int expand_variables) {
    char *expanded = malloc(BUFF_SIZE);
    if (!expanded)
        return NULL;

    size_t i = 0, j = 0;
    while (line[j]) {
        if (expand_variables && line[j] == '$' &&
            (line[j + 1] == '?' || ft_isalpha(line[j + 1]) || line[j + 1] == '_')) {
            j++;
            if (line[j] == '?') { // Handle $?
                char ret_str[12];
                snprintf(ret_str, 12, "%d", last_ret);
                size_t k = 0;
                while (ret_str[k]) 
                    expanded[i++] = ret_str[k++];
                j++;
            } else { // Handle environment variables
                size_t key_start = j;
                while (ft_isalnum(line[j]) || line[j] == '_')
                    j++;
                char *key = strndup(line + key_start, j - key_start);
                char *value = get_env_value(key, env);
                free(key);

                if (value) {
                    size_t k = 0;
                    while (value[k]) 
                        expanded[i++] = value[k++];
                    free(value);
                }
            }
        } else { // Copy regular characters
            expanded[i++] = line[j++];
        }
    }
    expanded[i] = '\0';
    return expanded;
}

void process_multiple_heredocs(t_mini *mini, t_token *token)
{
    t_token *current = token;
    int heredoc_count = 0;
    int **pipe_fds = NULL;
    
    while (current && current->type == HEREDOC) {
        heredoc_count++;
        current = current->next ? current->next->next : NULL;
    }
    
    pipe_fds = malloc(sizeof(int *) * heredoc_count);
    int i = 0;
    while (i < heredoc_count) {
        pipe_fds[i] = malloc(sizeof(int) * 2);
        if (pipe(pipe_fds[i]) == -1) {
            perror("minishell: pipe error");
            int j = 0;
            while (j < i) {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
                free(pipe_fds[j]);
                j++;
            }
            free(pipe_fds);
            mini->ret = 1;
            mini->no_exec = 1;
            return;
        }
        i++;
    }
    
    current = token;
    i = 0;
    while (i < heredoc_count && current) {
        if (!heredoc_to_pipe(mini, current->next, pipe_fds[i][1])) {
            close(pipe_fds[i][1]);
            if (i < heredoc_count - 1) {
                current = current->next ? current->next->next : NULL;
                i++;
                continue;
            }
        }
        close(pipe_fds[i][1]);
        current = current->next ? current->next->next : NULL;
        i++;
    }
    if (heredoc_count > 0) {
        if (dup2(pipe_fds[heredoc_count - 1][0], STDIN_FILENO) == -1) {
            perror("minishell: dup2 error");
            mini->ret = 1;
            mini->no_exec = 1;
        }
    }
    i = 0;
    while (i < heredoc_count) {
        close(pipe_fds[i][0]);
        free(pipe_fds[i]);
        i++;
    }
    free(pipe_fds);
}


int heredoc_to_pipe(t_mini *mini, t_token *delimiter_token, int write_fd)
{
    char *line;
    char *expanded_line;
    int expand_variables;
    int delimiter_found = 0;

    setup_heredoc_signals();
    expand_variables = (delimiter_token->quote_type == 0);
    char *delimiter = ft_strdup(delimiter_token->str);
    while (1) {
        line = readline("> ");
        if (g_signum == SIGINT)
        {
            free(line);
            free(delimiter);
            mini->ret = 130;
            return 0;
        }
        if (!line)
        {
            ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n", 2);
            free(delimiter);
            return 0;
        }
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            delimiter_found = 1;
            break;
        }
        if (expand_variables)
        {
            expanded_line = expand_heredoc_line(line, mini->env, mini->ret, 1);
            ft_putendl_fd(expanded_line, write_fd);
            free(expanded_line);
        }
        else
            ft_putendl_fd(line, write_fd);
        free(line);
    }
    setup_signals();
    free(delimiter);
    return delimiter_found;
}


int magic_box(char *path, char **args, t_mini *mini)
{
    char **environ;
    char *ptr;
    int ret;

    ret = SUCCESS;
	signal(SIGINT, SIG_IGN);
    pid_t pid = fork();
    if (pid == 0)
    {
        // signal(SIGINT, SIG_DFL);
		setup_signals();
        ptr = env_to_str(mini->env);
        environ = ft_split(ptr, '\n');
        ft_memdel(ptr);
        if (ft_strchr(path, '/') != NULL)
            execve(path, args, environ);
        ret = error_message(path);
        free_tab(environ);
        free_token(mini->start);
        exit(ret);
    }
    else
        waitpid(pid, &ret, 0);
    ret = WIFEXITED(ret) ? WEXITSTATUS(ret) : 1;
	check_signal_if_recieved(&ret);
    return (ret);
}

void	redir(t_mini *mini, t_token *token, int type)
{
	ft_close(mini->fdout);
	if (type == TRUNC)
		mini->fdout = open(token->str, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
	else
		mini->fdout = open(token->str, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
	if (mini->fdout == -1)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(token->str, STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		mini->ret = 1;
		mini->no_exec = 1;
		return ;
	}
	if (dup2(mini->fdout, STDOUT) == -1)
	{
		perror("dup2 failed");
		mini->ret = 1;
		mini->no_exec = 1;
		close(mini->fdout);
		return ;
	}
}

void input(t_mini *mini, t_token *token)
{
	ft_close(mini->fdin);
	mini->fdin = open(token->str, O_RDONLY, S_IRWXU);
	if (mini->fdin == -1)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(token->str, STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		mini->ret = 1;
		mini->no_exec = 1;
		return ;
	}
	if (dup2(mini->fdin, STDIN) == -1)
	{
		perror("dup2 failed");
		mini->ret = 1;
		mini->no_exec = 1;
		close(mini->fdin);
		return ;
	}
}

int	minipipe(t_mini *mini)
{
	pid_t	pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("minishell");
		return (1);
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		ft_close(pipefd[1]);
		dup2(pipefd[0], STDIN);
		mini->pipin = pipefd[0];
		mini->pid = -1;
		mini->parent = 0;
		mini->no_exec = 0;
		return (2);
	}
	else
	{
		setup_signals();
		ft_close(pipefd[0]);
		dup2(pipefd[1], STDOUT);
		mini->pipout = pipefd[1];
		mini->pid = pid;
		mini->last = 0;
		return (1);
	}
}

void redir_and_exec(t_mini *mini, t_token *token)
{
    t_token *prev;
    t_token *next;
    int pipe;

    prev = prev_sep(token, NOSKIP);
    next = next_sep(token, NOSKIP);
    pipe = 0;
    
    if (is_type(prev, TRUNC))
        redir(mini, token, TRUNC);
    else if (is_type(prev, APPEND))
        redir(mini, token, APPEND);
    else if (is_type(prev, INPUT))
        input(mini, token);
    else if (is_type(prev, HEREDOC))
    {
        t_token *first_heredoc = prev;
        while (first_heredoc->prev && first_heredoc->prev->type == HEREDOC)
            first_heredoc = first_heredoc->prev;
        process_multiple_heredocs(mini, first_heredoc);
        while (next && next->type == HEREDOC)
            next = next->next ? next->next->next : NULL;
    }
    else if (is_type(prev, PIPE))
    {
        pipe = minipipe(mini);
    }
        
    if (next && is_type(next, END) == 0 && pipe != 1)
        redir_and_exec(mini, next->next);
    if ((is_type(prev, END) || is_type(prev, PIPE) || !prev)
        && pipe != 1 && mini->no_exec == 0)
        exec_cmd(mini, token);
}


void minishell(t_mini *mini)
{
    t_token *token;
    int status;

    token = next_run(mini->start, NOSKIP);
    token = (is_types(mini->start, "TAIH")) ? mini->start->next : token;
    while (mini->exit == 0 && token)
    {
        mini->charge = 1;
        mini->parent = 1;
        mini->last = 1;
        redir_and_exec(mini, token);
        reset_std(mini);
        close_fds(mini);
        reset_fds(mini);
        waitpid(-1, &status, 0);
        check_signal_if_recieved(&status);
        status = WEXITSTATUS(status);
        mini->ret = (mini->last == 0) ? status : mini->ret;
        if (mini->parent == 0)
        {
            free_token(mini->start);
            exit(mini->ret);
        }
        mini->no_exec = 0;
        token = next_run(token, SKIP);
    }
}

int main(int ac, char **av, char **env)
{
    t_mini mini;

    (void)ac;
    (void)av;
    mini.in = dup(STDIN);
    mini.out = dup(STDOUT);
    mini.exit = 0;
    mini.ret = 0;
    mini.no_exec = 0;
    mini.start = NULL;
    if (!env || !env[0])
        env = create_minimal_env();
    reset_fds(&mini);
    env_init(&mini, env);
    secret_env_init(&mini, env);
    increment_shell_level(mini.env);
    setup_signals();
    while (mini.exit == 0)
    {
        g_signum = 0;
        parse(&mini);
        if (mini.start != NULL && check_line(&mini, mini.start))
            minishell(&mini);
        free_token(mini.start);
    }
    free_env(mini.env);
    free_env(mini.secret_env);
    return (mini.ret);
}