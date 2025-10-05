/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 03:35:48 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 20:24:49 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"
#include "../../../Includes/executor.h"
#include "../../../Includes/parser.h"

static int	append_file_into_file(char *src_path, char *dst_path)
{
	int		in_fd;
	int		out_fd;
	char	buf[4096];
	ssize_t	read_bytes;

	in_fd = open(src_path, O_RDONLY);
	if (in_fd == -1)
		return (1);
	out_fd = open(dst_path, O_WRONLY | O_APPEND);
	if (out_fd == -1)
		return (close(in_fd), 1);
	while ((read_bytes = read(in_fd, buf, sizeof(buf))) > 0)
		if (write(out_fd, buf, read_bytes) != read_bytes)
			return (close(in_fd), close(out_fd), 1);
	close(in_fd);
	close(out_fd);
	return (read_bytes < 0);
}

static int	handle_one_heredoc(t_file_node *file, t_shell_ctx *ctx,
				char **combined, t_file_node **last_hd)
{
	char	*tmp;

	if (!file || !ctx)
		return (0);
	tmp = handle_heredoc(file->filename, file->delimiter_quoted, ctx);
	if (!tmp)
		return (1);
	free(file->filename);
	if (*combined == NULL)
		*combined = tmp;
	else
	{
		if (append_file_into_file(tmp, *combined) != 0)
		{
			unlink(tmp);
			free(tmp);
			free(*combined);
			*combined = NULL;
			return (1);
		}
		unlink(tmp);
		free(tmp);
	}
	*last_hd = file;
	return (0);
}

static void	rewrite_heredoc_nodes(t_cmd_node *cmd, t_file_node *last_hd,
				char *combined)
{
	t_file_node	*iter;

	if (!cmd || !last_hd || !combined)
		return ;
	iter = cmd->files->head;
	while (iter)
	{
		if (iter->redir_type == HEREDOC)
		{
			iter->redir_type = INFILE;
			if (iter != last_hd)
				iter->filename = ft_strdup("/dev/null");
			else
				iter->filename = combined;
		}
		iter = iter->next;
	}
}

int	process_heredoc(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	t_file_node	*file;
	t_file_node	*last_hd;
	char		*combined;

	setup_heredoc_and_child_sig(1);
	if (!cmd || !cmd->files)
		return (0);
	combined = NULL;
	last_hd = NULL;
	file = cmd->files->head;
	while (file)
	{
		if (file->redir_type == HEREDOC)
			if (handle_one_heredoc(file, ctx, &combined, &last_hd) != 0)
				return (1);
		file = file->next;
	}
	if (combined && last_hd)
		rewrite_heredoc_nodes(cmd, last_hd, combined);
	return (0);
}
