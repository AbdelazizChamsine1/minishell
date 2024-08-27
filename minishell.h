/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 14:13:19 by achamsin          #+#    #+#             */
/*   Updated: 2024/08/27 15:04:49 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define SKIP 1
# define NOSKIP 0

# define BUFF_SIZE 4096
# define EXPANSION -36
# define ERROR 1
# define SUCCESS 0
# define IS_DIRECTORY 126
# define UNKNOWN_COMMAND 127

typedef struct s_token
{
	char			*str;
	int				quotype;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}				t_token;

// typedef struct s_env
// {
// 	char			*value;
// 	struct s_env	*next;
// }				t_env;

typedef enum e_tokens
{
	TOKEN_PIPE,
	TOKEN_TRUNC,
	TOKEN_WORD,
	TOKEN_IN,
	TOKEN_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_EOF
}	t_tokens;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				hidden;
	struct s_env	*next;
}	t_env;

void	display_prompt();
void	shell_loop();
int		ignore_sep(char *line, int i);
void	ft_skip_space(const char *str, int *i);
void	ft_skip_space(const char *str, int *i);
void	type_arg(t_token *token, int separator);
void	display_tokens(t_token *token);

//Tokenisation

//ENV
int		is_env_char(int c);
// void	squish_args(t_mini *mini);
t_token	*get_tokens(char *line);

#endif