/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:21:56 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 23:32:12 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

// #  define BUFFER_SIZE 1000

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>

/// testing
# include "../libft/libft.h"
# include "minishell.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_token			t_file_list_t_token;
typedef struct s_segment		t_segment;
typedef struct s_segment_list	t_segment_list;
typedef struct s_token_list		t_token_list;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_BUILTIN,
	TOKEN_OUTFILE,
	TOKEN_APPEND,
	TOKEN_INFILE,
	TOKEN_HEREDOC,
	TOKEN_PIPE,
	TOKEN_AMPERSAND,
	TOKEN_SINGLE_QUOTES,
	TOKEN_DOUBLE_QUOTES,
}								t_token_type;
// @brief enum struct for token types
// 0 regular command
// 1 Builtin function
// 2 >
// 3 >>
// 4 <
// 5 <<
// 6 |
// 7 &
// 8 '' only empty ones
// 9 "" only empty ones

/// @brief enum struct for quote type
typedef enum e_seg_quote_type
{
	SEG_SINGLE_QUOTE,
	SEG_DOUBLE_QUOTE,
	SEG_NORMAL_QUOTE,
}								t_seg_quote_type;
// Segment enbedded single quotes ''. no expansion
// Segment enbedded double quotes "". expansion
// Segment enbedded in no  quotes   . expansion

/// @brief
typedef struct s_token
{
	t_token_type				token;
	char						*value;
	struct s_token				*next;
	t_segment_list				*segment_list;

}								t_token;
// 0-9 Word, Builtin, etc...
// "echo", ""hello world""
// pointer to next token or NULL
// segs list of tokens NULL if not seg or no segs
typedef struct s_segment
{
	char						*value;
	t_seg_quote_type			type;
	struct s_segment			*next;
}								t_segment;

typedef struct s_segment_list
{
	t_segment					*head;
	t_segment					*tail;
	ssize_t						size;
}								t_segment_list;

typedef struct s_heredoc_ctx
{
	char						*delimiter;
	int							delimiter_quoted;
	int							fd;
	t_shell_ctx					*ctx;
}								t_heredoc_ctx;

// for expansion
typedef struct s_expansion_data
{
	char						**final_str;
	int							*i;
	int							*start;
	t_shell_ctx					*ctx;
}								t_expansion_data;
typedef struct s_parser_ctx
{
	t_cmd_list					*cmd_list;
	t_cmd_node					**current_cmd;
	t_token						**token;
	char						*original_input;
}								t_parser_ctx;
/// meta struct for tokens
typedef struct s_token_list
{
	t_token						*head;
	t_token						*tail;
	ssize_t						size;
	char						*prompt;
	int							found_error;
}								t_token_list;
// points to first t_token
// points to last t_token
// amount of tokens
// whole prompt

bool							check_builtin(t_token_list *token_list,
									int start, int i);
bool							check_space(char c);
bool							check_empty_quote(t_token_list *token_list,
									int i);
bool							check_quote(t_token_list *token_list, int i);
bool							check_redirection(t_token_list *token_list,
									int i);
bool							check_token_split(t_token_list *token_list,
									int i);

int								handle_empty_quotes(t_token_list *token_list,
									int i);
int								handle_redirection(t_token_list *token_list,
									int i);
int								handle_pipe(t_token_list *token_list, int i);
int								handle_word_in_quotes(t_token_list *token_list,
									int i);
int								handle_word_master(t_token_list *token_list,
									int i);

t_token_list					*initialise_token_list(char *prompt);

t_token_list					*start_lexer(char *prompt);

void							add_token(t_token_list *token_list, char *value,
									t_token_type token_type);
void							add_segment_to_token(t_token *token,
									char *value, t_seg_quote_type type);
void							found_token(t_token_list *token_list, int start,
									int end, t_token_type token_type);
void							start_segmentation(t_token_list *tokens,
									t_shell_ctx *ctx);
void							dissect_token(t_token *token);
void							assemble_final_token(t_token *token);
bool							handle_quoted_content(t_token *token,
									int *index, int *start);
int								handle_ampersand(t_token_list *token_list,
									int i);

t_segment_list					*init_segment_list(void);

char							*get_env_value(t_env_list *env_list,
									char *var_name);
char							*get_exit_code(t_shell_ctx *ctx);
// int								ft_strcmp(const char *s1, const char *s2);

/// cleanup functions
void							free_file_list(t_file_list *file_list);
void							free_token_list(t_token_list *tokens);
void							free_cmd_list(t_cmd_list *cmds_list);

void							init_list_env(t_env_list *env_list,
									char **envp);
void							free_env_list(t_env_list *env_list);

char							*process_escape_sequences(char *str);

char							*expand_string_variables(char *line,
									t_shell_ctx *ctx);
char							*process_escape_sequences(char *str);
bool							validate_quotes(t_token_list *token_list);
void							init_list_env(t_env_list *env_list,
									char **envp);
void							free_env_list(t_env_list *env_list);
t_cmd_list						*start_parser(t_token_list *tokens,
									t_shell_ctx *ctx, char *original_input);
bool							val_qu_closed_quotes(t_token_list *token_list);
void							free_tokens(t_token_list *tokens);
void							free_cmd_list(t_cmd_list *cmds_list);
void							free_env_list(t_env_list *env_list);
bool							validate_syntax(t_token_list *tokens);
void							safe_strjoin(char **dest, char *src);
void							handle_tilde_expansion(t_segment *segment,
									t_expansion_data *data);
void							handle_dollar_expansion(t_segment *segment,
									t_expansion_data *data);
void							add_to_cmd_list(t_cmd_list *list,
									t_cmd_node *node);
int								add_argument(t_cmd_node *cmd_node, char *arg);
void							handle_token_pipe(t_cmd_node **current_cmd,
									t_token **token);
int								handle_token_word_builtin(t_token *token,
									t_cmd_node **cmd, t_cmd_list *cmd_list);
t_cmd_node						*create_cmd_node(t_token *token);
void							expand_variables_in_segment(t_segment *segment,
									t_shell_ctx *ctx);
bool							validate_syntax(t_token_list *tokens);
char							*get_exit_code(t_shell_ctx *ctx);

char							*read_line_interactive(void);
char							*read_line_non_interactive(void);
char							*read_heredoc_line(int is_interactive);
int								write_line_to_file(int fd, char *line,
									int delimiter_quoted, t_shell_ctx *ctx);
void							add_redirection(t_cmd_node *cmd_node,
									t_token *token, t_token *next,
									char *original_input);
int								is_del_qu_in_org_input(char *original_input,
									char *delimiter);
void							handle_dollar_quotes_seg(t_token *token, int *i,
									int *start);
void							handle_quotes(t_token *token, int *i,
									int *start);
void							handle_dollar_vars(t_token *token, int *i,
									int *start);
void							handle_tilde_expansion(t_segment *segment,
									t_expansion_data *data);
void							process_escape_seq_in_seg(t_segment *segment);
t_file_list						*init_file_list_if_needed(t_cmd_node *cmd_node);

void							free_token_list(t_token_list *tokens);
void							free_segment_list(t_segment_list *seg_list);

#endif