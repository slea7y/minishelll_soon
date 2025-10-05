/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 05:25:32 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 04:42:46 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/heredoc.h"
#include "../../../Includes/parser.h"

// ok
/// @brief interactive mode. readline: reads line unitl pressed enter,
// displays "> " and waits for input, returns line value,  allows signals
/// @param
/// @return
char	*read_line_interactive(void)
{
	return (readline("> "));
}

// ok
/// @brief non interactive mode: eg minishell < script.lol
// reads unitl \n, no edits or signal, cause read from file. no history
/// @param
/// @return
char	*read_line_non_interactive(void)
{
	return (get_next_line(STDIN_FILENO));
}

// ok
/// @brief simple condition fction bases ond if input is interactive
// by isatty
/// @param is_interactive
/// @return
char	*read_heredoc_line(int is_interactive)
{
	if (is_interactive)
		return (read_line_interactive());
	return (read_line_non_interactive());
}

// ok
/// @brief checks if delimiter quoted -> no expansion, writes line in fd file
/// @param fd
/// @param line
/// @param delimiter_quoted
/// @param ctx
/// @return
int	write_line_to_file(int fd, char *line, int delimiter_quoted,
		t_shell_ctx *ctx)
{
	char	*expanded_line;

	if (delimiter_quoted)
		expanded_line = ft_strdup(line);
	else
		expanded_line = expand_string_variables(line, ctx);
	if (!expanded_line)
		return (-1);
	write(fd, expanded_line, ft_strlen(expanded_line));
	write(fd, "\n", 1);
	free(expanded_line);
	return (0);
}
