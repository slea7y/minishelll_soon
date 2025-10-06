/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 22:26:59 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/06 02:01:50 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"
#include "../../../Includes/executor.h"
#include "../../../Includes/parser.h"
#include <sys/stat.h>

int	search_in_paths(char **cmd, char **envp, char **paths)
{
	char	*path_to_cmd;
	int		i;

	i = 0;
	while (paths[i])
	{
		path_to_cmd = build_cmd_paths(paths[i], cmd[0]);
		if (!path_to_cmd)
			return (free_env(paths), 1);
		if (access(path_to_cmd, X_OK) == 0)
		{
			free_env(paths);
			execve(path_to_cmd, &cmd[0], envp);
			perror("execve");
			free(path_to_cmd);
			return (1);
		}
		free(path_to_cmd);
		i++;
	}
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_env(paths);
	return (127);
}

// "./" for the relative paths
int	execute_cmd(t_cmd_node *cmd, char **envp, t_env_list *env_list)
{
	char	**paths;

	if (cmd->cmd[0][0] == '/' || ft_strncmp(cmd->cmd[0], "./", 2) == 0)
	{
		if (access(cmd->cmd[0], X_OK) == 0)
		{
			execve(cmd->cmd[0], cmd->cmd, envp);
			perror("execve");
			return (1);
		}
		ft_putstr_fd(cmd->cmd[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (127);
	}
	paths = find_full_path(env_list);
	if (!paths)
	{
		ft_putstr_fd(cmd->cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	return (search_in_paths(cmd->cmd, envp, paths));
}

//when u call frok childproces gets it s own memory placce and 
//apparentlly memory just gets cleaned by the os (wild)
//in child process memory is completlly separet form the parent 
//so i should be free to just exit here :-DDD
void	handle_external_child(t_cmd_node *cmd, char **envp, t_shell_ctx *ctx)
{
	int	exit_code;

	exit_code = handle_redirections(cmd);
	if (exit_code != 0)
	{
		free_env(envp);
		exit (1);
	}
	if (cmd->cmd && cmd->cmd[0])
		exit_code = execute_cmd(cmd, envp, ctx->env);
	else
		exit_code = 0;
	free_env(envp);
	exit(exit_code);
}

//after child process has been created and exectued parents needs to wait
//for child to finish, get childs exit status, clean up resorces, return the 
//desired exit B-)
//the &status thingi is where the child termination info is getting stored
//and the 0 after that means "wait for any child state to change" (normal 
//terimantion, siganls etc.) 
//WIFEXITED(status) =did the child exit normally? if YES WIFEXITED(status)
// extracts the actual exit code 
//WIFSIGNALED(status== was the child killed by the signal? (rip baby)
//if yes WTERMSIG(status) is the signal number
//esle some other error so exit should be 1
int	handle_parent_proc(pid_t pid_try)
{
	int	exit_code;
	int	status;

	waitpid(pid_try, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	else
		exit_code = 1;
	return (exit_code);
}

int	execute_single_external(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	pid_t	pid_try;
	char	**envp;
	int		exit_code;

	if (!cmd || !ctx)
		return (1);
	envp = env_to_array(ctx->env);
	if (!envp)
		return (1);
	pid_try = fork();
	if (pid_try < 0)
	{
		perror("fork");
		free_env(envp);
		return (1);
	}
	if (pid_try == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		handle_external_child(cmd, envp, ctx);
	}
	exit_code = handle_parent_proc(pid_try);
	free_env(envp);
	return (exit_code);
}
