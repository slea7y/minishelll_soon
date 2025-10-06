/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 18:13:57 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/06 00:19:31 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

// int		correct_pipe(t_cmd_node *current, int pipe_fd[2]);

/// @brief
/// @param current
/// @param ctx
/// @param pipe_fd
/// @param prev_pipe
/// @return -2 to distinguish from -1 (not last pid)
pid_t	fork_execute(t_cmd_node *current, t_shell_ctx *ctx, int pipe_fd[2],
		int prev_pipe)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-2);
	}
	if (pid == 0)
		prep_child_for_exec(current, ctx, prev_pipe, pipe_fd);
	if (prev_pipe != -1)
		close(prev_pipe);
	if (current->next)
		close(pipe_fd[1]);
	if (!current->next)
		return (pid);
	return (-1);
}

/// @param r_val = return value
int	safe_pipe_return(int prev_pipe, int pipe_fd[2], t_cmd_node *curr, int r_val)
{
	if (prev_pipe != -1)
		close(prev_pipe);
	if (curr && curr->next)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	return (r_val);
}

// after child process has been created and exectued parents needs to wait
// for child to finish, get childs exit status, clean up resorces, return the
// desired exit B-)
// the &status thingi is where the child termination info is getting stored
// and the 0 after that means "wait for any child state to change" (normal
// terimantion, siganls etc.)
// WIFEXITED(status) =did the child exit normally? if YES WIFEXITED(status)
// extracts the actual exit code
// WIFSIGNALED(status== was the child killed by the signal? (rip baby)
// if yes WTERMSIG(status) is the signal number
// esle some other error so exit should be 1
// SIGPIPE is a signal sent to a process when it tries to write to a pipe
// that has no readers. This happens in pipelines when a command tries to
// write to the next command,
// but the next command has already exited or closed its reading end.
int	wait_for_multiple(pid_t last_pid)
{
	int	status;
	int	exit_code;

	exit_code = 0;
	if (last_pid != -1)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGPIPE)
				exit_code = 0;
			else
				exit_code = 128 + WTERMSIG(status);
		}
	}
	while (wait(&status) > 0)
		;
	return (exit_code);
}

int	process_all_heredocs(t_cmd_list *cmd, t_shell_ctx *ctx)
{
	t_cmd_node	*current;

	current = cmd->head;
	while (current)
	{
		if (process_heredoc(current, ctx) != 0)
			return (1);
		current = current->next;
	}
	return (0);
}

int	execute_pipeline_loop(t_cmd_list *cmd, t_shell_ctx *ctx, pid_t *last_pid)
{
	t_cmd_node	*current;
	int			pipe_fd[2];
	int			prev_pipe;

	current = cmd->head;
	prev_pipe = -1;
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	while (current)
	{
		if (!correct_pipe(current, pipe_fd))
			return (safe_pipe_return(prev_pipe, pipe_fd, current, 1));
		*last_pid = fork_execute(current, ctx, pipe_fd, prev_pipe);
		if (*last_pid == -2)
			return (safe_pipe_return(prev_pipe, pipe_fd, current, 1));
		prev_pipe = update_pipe(current, pipe_fd, prev_pipe);
		current = current->next;
	}
	return (0);
}

