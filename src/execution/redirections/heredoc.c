/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 03:35:48 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 07:44:01 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"
#include "../../../Includes/executor.h"
#include "../../../Includes/parser.h"

int	process_heredoc(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	t_file_node	*file;
	char		*temp_filename;

	setup_heredoc_and_child_sig(1);
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
