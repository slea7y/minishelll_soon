/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 03:35:48 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 23:15:28 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

// static int	append_file_into_file(char *src_path, char *dst_path)
// {
// 	int		in_fd;
// 	int		out_fd;
// 	char	buf[4096];
// 	ssize_t	bytes_read;
// 	int		ret;

// 	in_fd = open(src_path, O_RDONLY);
// 	if (in_fd == -1)
// 		return (1);
// 	out_fd = open(dst_path, O_WRONLY | O_APPEND | O_CREAT, 0644);
// 	if (out_fd == -1)
// 		return (close(in_fd), 1);
// 	ret = 0;
// 	bytes_read = read(in_fd, buf, sizeof(buf));
// 	while (bytes_read > 0)
// 	{
// 		if (write(out_fd, buf, bytes_read) != bytes_read)
// 		{
// 			ret = 1;
// 			break ;
// 		}
// 		bytes_read = read(in_fd, buf, sizeof(buf));
// 	}
// 	close(in_fd);
// 	close(out_fd);
// 	if (bytes_read < 0)
// 		ret = 1;
// 	return (ret);
// }

static int	transfer_file_content(int in_fd, int out_fd)
{
	char	buf[4096];
	ssize_t	bytes_read;
	int		ret;

	ret = 0;
	while (1)
	{
		bytes_read = read(in_fd, buf, sizeof(buf));
		if (bytes_read <= 0)
			break ;
		if (write(out_fd, buf, bytes_read) != bytes_read)
		{
			ret = 1;
			break ;
		}
	}
	if (bytes_read < 0)
		ret = 1;
	return (ret);
}

static int	append_file_into_file(char *src_path, char *dst_path)
{
	int	in_fd;
	int	out_fd;
	int	ret;

	in_fd = open(src_path, O_RDONLY);
	if (in_fd == -1)
		return (1);
	out_fd = open(dst_path, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (out_fd == -1)
	{
		close(in_fd);
		return (1);
	}
	ret = transfer_file_content(in_fd, out_fd);
	close(in_fd);
	close(out_fd);
	return (ret);
}

static void	cleanup_heredoc_files(char *temp1, char *temp2)
{
	if (temp1)
	{
		unlink(temp1);
		free(temp1);
	}
	if (temp2)
	{
		unlink(temp2);
		free(temp2);
	}
}

int	handle_one_heredoc(t_file_node *file, t_shell_ctx *ctx,
		char **temp_filename, t_file_node **last_hd)
{
	char	*temp;

	if (!file || !ctx)
		return (0);
	temp = handle_heredoc(file->filename, file->delimiter_quoted, ctx);
	if (!temp)
		return (1);
	free(file->filename);
	file->filename = NULL;
	if (*temp_filename == NULL)
		*temp_filename = temp;
	else
	{
		if (append_file_into_file(temp, *temp_filename) != 0)
			return (cleanup_heredoc_files(temp, *temp_filename), 1);
		unlink(temp);
		free(temp);
	}
	*last_hd = file;
	return (0);
}

int	process_heredoc(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	t_file_node	*file;
	t_file_node	*last_hered;
	char		*temp_filename;

	g_heredoc_abort = 0;
	setup_heredoc_and_child_sig(1);
	if (!cmd || !cmd->files)
		return (0);
	last_hered = NULL;
	temp_filename = NULL;
	file = cmd->files->head;
	while (file)
	{
		if (file->redir_type == HEREDOC)
			if (handle_one_heredoc(file, ctx, &temp_filename, &last_hered) != 0)
				return (1);
		file = file->next;
	}
	if (temp_filename && last_hered)
		rewrite_heredoc_nodes(cmd, last_hered, temp_filename);
	start_signals();
	return (0);
}
