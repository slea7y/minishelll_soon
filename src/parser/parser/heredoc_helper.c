/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 05:25:32 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/06 02:05:23 by majkijew         ###   ########.fr       */
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
	char	*line;

	write (1, "> ", 2);
	line = get_next_line(STDIN_FILENO);
	return (line);
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
	(void)is_interactive;
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
	free(expanded_line);
	return (0);
}
