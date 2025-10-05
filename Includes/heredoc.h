/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 06:23:09 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 14:00:08 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "parser.h"
# include <stddef.h>

char	*expand_string_variables(char *line, t_shell_ctx *ctx);
char	*get_next_line(int fd);
void	cleanup_line_reader(void);
char	*handle_heredoc(char *delimiter, int delimiter_quoted,
			t_shell_ctx *ctx);
char	*handle_heredoc_with_content(char *delimiter, int delimiter_quoted,
			t_shell_ctx *ctx, char *content);

#endif