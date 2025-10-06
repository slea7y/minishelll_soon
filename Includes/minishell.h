/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 14:00:26 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 22:36:29 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Includes - Start
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
// # include "libft.h"
// # include "parser.h"
// # include "executor.h"
// Includes - End
// Linked list - Start

typedef struct s_file_node
{
	char				*filename;
	int					redir_type;
	int					delimiter_quoted;
	struct s_file_node	*next;
}						t_file_node;

typedef struct s_file_list
{
	t_file_node			*head;
	t_file_node			*tail;
	ssize_t				size;
}						t_file_list;

typedef struct s_cmd_node
{
	struct s_cmd_node	*next;
	int					cmd_type;
	char				**cmd;
	t_file_list			*files;
}						t_cmd_node;

typedef struct s_cmd_list
{
	int					syntax_error;
	t_cmd_node			*head;
	t_cmd_node			*tail;
	ssize_t				size;
}						t_cmd_list;

typedef struct s_env_node
{
	char				*key;
	char				*value;
	struct s_env_node	*next;
}						t_env_node;

typedef struct s_env_list
{
	t_env_node			*head;
	t_env_node			*tail;
	ssize_t				size;
}						t_env_list;

typedef struct s_shell_ctx
{
	t_env_list			*env;
	int					last_exit_code;
}						t_shell_ctx;

// Linked list - End
// Structs - Start
typedef enum CMD_TYPE
{
	BUILTIN,
	CMD,
	PIPE
}						t_CMD_TYPE;

typedef enum REDIR_TYPE
{
	INFILE,
	OUTFILE,
	HEREDOC,
	OUTFILE_APPEND
}						t_REDIR_TYPE;
// Structs - End

// Files in Folder src/

// exit_code.c - Start

// exit_code.c - End

// main.c - Start

// main.c - End

// signals.c - Start
void					start_signals(void);
void					setup_heredoc_and_child_sig(int is_heredoc);
extern volatile sig_atomic_t	g_heredoc_abort;
// signals.c - End

/// cleanup functions

#endif
