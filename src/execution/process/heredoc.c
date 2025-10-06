/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 03:35:48 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/06 00:20:47 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

int	execute_pipes(t_cmd_list *cmd, t_shell_ctx *ctx)
{
	pid_t	last_pid;
	int		error_status;

	last_pid = -1;
	error_status = 0;
	if (process_all_heredocs(cmd, ctx) != 0)
		return (1);
	error_status = execute_pipeline_loop(cmd, ctx, &last_pid);
	if (error_status != 0)
		return (1);
	return (wait_for_multiple(last_pid));
}

int	process_heredoc(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	t_file_node	*file;
	char		*temp_filename;

	if (!cmd || !cmd->files)
		return (0);
	file = cmd->files->head;
	while (file)
	{
		if (file->redir_type == HEREDOC)
		{
			temp_filename = handle_heredoc(file->filename,
					file->delimiter_quoted, ctx);
			if (!temp_filename)
				return (1);
			free(file->filename);
			file->filename = temp_filename;
			file->redir_type = INFILE;
		}
		file = file->next;
	}
	return (0);
}
