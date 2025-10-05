/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_processor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 04:04:12 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 19:33:03 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/heredoc.h"
#include "../../../Includes/parser.h"

// ok
/// @brief returns individual filename with counter for each heredoc
/// @param counter
/// @return
static char	*get_temp_filename(int counter)
{
	char	*num_str;
	char	*filename;

	num_str = ft_itoa(counter);
	if (!num_str)
		return (NULL);
	filename = ft_strjoin("./heredoc_", num_str);
	if (!filename)
	{
		free(num_str);
		return NULL;
	}
	return (filename);
}

// ok
/// @brief checks for availible filename, creates fd for test. creates file
// flags: create if not there, write only, deletes old content, permissions
/// @param delimiter
/// @return
static char	*create_temp_file_interactive(char *delimiter)
{
	char	*temp_filename;
	int		fd;
	int		counter;

	(void)delimiter;
	counter = 0;
	while (counter < 1000)
	{
		temp_filename = get_temp_filename(counter);
		if (!temp_filename)
			return (NULL);
		fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd != -1)
		{
			close(fd);
			return (temp_filename);
		}
		free(temp_filename);
		counter++;
	}
	return (NULL);
}

// ok
/// @brief cheks if line is delimiter. check if return empty = EOF
// gets line len, gets delimterlen, removes /n,
// compares content and len to check if its delimiter on a single line
/// @param line
/// @param hdoc
/// @return
static int	process_heredoc_line(char *line, t_heredoc_ctx *hdoc)
{
	size_t	line_len;
	size_t	delimiter_len;
	int		is_delimiter;

	if (!line)
		return (1);
	line_len = ft_strlen(line);
	delimiter_len = ft_strlen(hdoc->delimiter);
	is_delimiter = 0;
	if (line_len > 0 && line[line_len - 1] == '\n')
		line_len--;
	if (line_len == delimiter_len && ft_strncmp(line, hdoc->delimiter,
			delimiter_len) == 0)
		is_delimiter = 1;
	if (is_delimiter)
		return (1);
	return (write_line_to_file(hdoc->fd, line, hdoc->delimiter_quoted,
			hdoc->ctx));
}

// ok
/// @brief read input until end. check if non or interactive mode
// calls readline or get_next_line. reads line. checks for delimiter for exit
/// @param hdoc
static void	heredoc_read_loop(t_heredoc_ctx *hdoc)
{
	char	*line;
	int		is_interactive;

	is_interactive = isatty(STDIN_FILENO);
	while (1)
	{
		line = read_heredoc_line(is_interactive);
		if (process_heredoc_line(line, hdoc))
		{
			free(line);
			break ;
		}
		free(line);
	}
}

// ok
/// @brief controls heredocs process. checks delimiter, creates tmp filename
// opens file to write (write only and apped), reads until end
/// @param delimiter
/// @param delimiter_quoted
/// @param ctx
/// @return
char	*handle_heredoc(char *delimiter, int delimiter_quoted, t_shell_ctx *ctx)
{
	char			*temp_filename;
	t_heredoc_ctx	hdoc;

	if (!delimiter)
		return (NULL);
	temp_filename = create_temp_file_interactive(delimiter);
	if (!temp_filename)
		return (NULL);
	hdoc.fd = open(temp_filename, O_WRONLY | O_APPEND);
	if (hdoc.fd == -1)
	{
		free(temp_filename);
		return (NULL);
	}
	hdoc.delimiter = delimiter;
	hdoc.delimiter_quoted = delimiter_quoted;
	hdoc.ctx = ctx;
	heredoc_read_loop(&hdoc);
    close(hdoc.fd);
    return (temp_filename);
}
