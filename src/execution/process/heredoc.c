/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 03:35:48 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 14:03:18 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

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
