/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 04:25:36 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 18:48:59 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# define MAX_INPUT 1024
# define MAX_ARGS 64

char	*get_next_line(int fd);
// start
int		start_execution(t_cmd_list *cmds, t_shell_ctx *ctx);
int		detect_pipes(t_cmd_list	*cmds);
int		execute_bulidins(t_cmd_node *cmd, t_shell_ctx *ctx);
int		is_bulidin(char *cmd);
// utils
int		ft_strcmp(char *str, char *check);
int		is_numeric(char	*str);
//cd_utils
int		go_dest_with_path(char *pa, char *prev_pwd, t_env_list *env);
// Environment functions
int		check_if_var_exist(char *key, t_env_list *env);
void	change_var_value(char *key, char *new_value, t_env_list *env);
void	sort_env(t_env_list *env_list);
void	add_change_export(char *args, t_env_list *env);
int		is_valid_identifier(char *args);
void	init_list_env(t_env_list *env_list, char **envp);
void	sort_env(t_env_list *env_list);
// Builtin functions
int		ft_echo(t_cmd_node *cmd, t_shell_ctx *ctx);
int		ft_pwd(t_cmd_node *cmd, t_shell_ctx *ctx);
int		ft_env(t_cmd_node *cmd, t_shell_ctx *ctx);
int		ft_cd(t_cmd_node *cmd, t_shell_ctx *ctx);
int		ft_export(t_cmd_node *cmd, t_shell_ctx *ctx);
int		ft_exit(t_cmd_node *cmd, t_shell_ctx *ctx);
int		ft_unset(t_cmd_node *cmd, t_shell_ctx *ctx);
//paths
void	failed_paths(char **paths, char *cmd);
char	**find_full_path(t_env_list *env_list);
char	*build_cmd_paths(char *dir, char *cmd);
//process
int		execute_single_external(t_cmd_node *cmd, t_shell_ctx *ctx);
//external_p
char	**env_to_array(t_env_list *env_list);
char	**free_env(char **env);
int		execute_cmd(t_cmd_node *cmd, char **envp, t_env_list *env_list);
//redir
int		handle_redirections(t_cmd_node *cmd);
//pipes
int		execute_pipes(t_cmd_list *cmd, t_shell_ctx *ctx);
int		update_pipe(t_cmd_node *current, int pipe_fd[2], int prev_pipr);
void	prep_child_for_exec(t_cmd_node *current,
			t_shell_ctx *ctx, int prev_pipe, int pipe_fd[2]);
char	*handle_heredoc(char *delimiter, int delimiter_quoted,
			t_shell_ctx *ctx);
int		process_heredoc(t_cmd_node *cmd, t_shell_ctx *ctx);

#endif