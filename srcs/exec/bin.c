/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:28:04 by achamsin          #+#    #+#             */
/*   Updated: 2025/02/08 11:44:20 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_message(char *path)
{
	DIR	*folder;
	int	fd;
	int	ret;

	fd = open(path, O_WRONLY);
	folder = opendir(path);
	ft_putstr_fd("minishell: ", STDERR);
	ft_putstr_fd(path, STDERR);
	if (ft_strchr(path, '/') == NULL)
		ft_putendl_fd(": command not found", STDERR);
	else if (fd == -1 && folder == NULL)
		ft_putendl_fd(": No such file or directory", STDERR);
	else if (fd == -1 && folder != NULL)
		ft_putendl_fd(": is a directory", STDERR);
	else if (fd != -1 && folder == NULL)
		ft_putendl_fd(": Permission denied", STDERR);
	if (ft_strchr(path, '/') == NULL || (fd == -1 && folder == NULL))
		ret = UNKNOWN_COMMAND;
	else
		ret = IS_DIRECTORY;
	if (folder)
		closedir(folder);
	ft_close(fd);
	return (ret);
}

static void	execute_child_process(char *path, char **args, t_mini *mini)
{
	char	**environ;
	char	*ptr;
	int		ret;

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

int	magic_box(char *path, char **args, t_mini *mini)
{
	int		ret;
	pid_t	pid;

	ret = SUCCESS;
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		execute_child_process(path, args, mini);
	else
		waitpid(pid, &ret, 0);
	if (WIFEXITED(ret))
		ret = WEXITSTATUS(ret);
	else
		ret = 1;
	check_signal_if_recieved(&ret);
	setup_signals();
	return (ret);
}

char	*path_join(const char *s1, const char *s2)
{
	char	*tmp;
	char	*path;

	tmp = ft_strjoin(s1, "/");
	path = ft_strjoin(tmp, s2);
	ft_memdel(tmp);
	return (path);
}

char	*check_dir(char *bin, char *command)
{
	DIR				*folder;
	struct dirent	*item;
	char			*path;

	path = NULL;
	folder = opendir(bin);
	if (!folder)
		return (NULL);
	while ((item = readdir(folder)))
	{
		if (ft_strcmp(item->d_name, command) == 0)
			path = path_join(bin, item->d_name);
	}
	closedir(folder);
	return (path);
}

int	exec_bin(char **args, t_env *env, t_mini *mini)
{
	int		i;
	char	**bin;
	char	*path;
	int		ret;

	i = 0;
	ret = UNKNOWN_COMMAND;
	while (env && env->value && ft_strncmp(env->value, "PATH=", 5) != 0)
		env = env->next;
	if (env == NULL || env->next == NULL)
		return (magic_box(args[0], args, mini));
	bin = ft_split(env->value + 5, ':');
	if (!args[0] && !bin[0])
		return (ERROR);
	path = check_dir(bin[0], args[0]);
	while (args[0] && bin[i] && path == NULL)
		path = check_dir(bin[i++], args[0]);
	if (path != NULL)
		ret = magic_box(path, args, mini);
	else
		ret = magic_box(args[0], args, mini);
	free_tab(bin);
	ft_memdel(path);
	return (ret);
}
