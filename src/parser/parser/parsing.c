/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 18:33:43 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 22:20:21 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

//ok
/// @brief if not init, init cmd file list node
/// @param cmd_node 
t_file_list	*init_file_list_if_needed(t_cmd_node *cmd_node)
{
	if (cmd_node->files)
		return (cmd_node->files);
	cmd_node->files = malloc(sizeof(t_file_list));
	if (!cmd_node->files)
		return (NULL);
	cmd_node->files->head = NULL;
	cmd_node->files->tail = NULL;
	cmd_node->files->size = 0;
	return (cmd_node->files);
}

// ok
/// @brief skips whitespace
/// @param pos
/// @return
static char	*skip_whitespace(char *pos)
{
	while (*pos && (*pos == ' ' || *pos == '\t'))
		pos++;
	return (pos);
}

// ok
/// @brief checks if text at current position matches quoted delimiter
/// extracts content between quotes and compares with given delimiter
/// @param pos pointer to current position in text (will be advanced)
/// @param delimiter 1 if quoted text matches delimiter, 0 if not
/// @return
static int	check_quoted_match(char **pos, char *delimiter)
{
	char	quote_char;
	char	*delimiter_start;
	int		delimiter_len;

	if (**pos != '\'' && **pos != '"')
		return (0);
	quote_char = **pos;
	(*pos)++;
	delimiter_start = *pos;
	while (**pos && **pos != quote_char)
		(*pos)++;
	if (**pos != quote_char)
		return (0);
	delimiter_len = *pos - delimiter_start;
	if (delimiter_len > 0 && (size_t)delimiter_len == ft_strlen(delimiter)
		&& ft_strncmp(delimiter_start, delimiter, delimiter_len) == 0)
	{
		return (1);
	}
	return (0);
}

// ok
/// @brief checks if heredoc delimiter is quoted 
/// searches first '<<' operators and determines if  delimiter is in quotes
/// @param original_input the original command line input to search in
/// @param delimiter the heredoc delimiter to look for
/// @return 1 if delimiter is found quoted after '<<', 0 if not
int	is_del_qu_in_org_input(char *original_input, char *delimiter)
{
	char	*pos;

	if (!original_input || !delimiter)
		return (0);
	pos = original_input;
	while (true)
	{
		pos = ft_strnstr(pos, "<<", ft_strlen(pos));
		if (!pos)
			break ;
		pos += 2;
		pos = skip_whitespace(pos);
		if (check_quoted_match(&pos, delimiter))
			return (1);
		pos++;
	}
	return (0);
}

// ok
/// @brief adds arg to cmd_list of node
// cehcks arr size, allocates, cpys old arg,
/// @param cmd_node
/// @param arg
/// @return
int	add_argument(t_cmd_node *cmd_node, char *arg)
{
	int		size;
	char	**new_cmd;

	size = 0;
	if (cmd_node->cmd)
		while (cmd_node->cmd[size])
			size++;
	new_cmd = ft_calloc(size + 2, sizeof(char *));
	if (!new_cmd)
		return (0);
	if (cmd_node->cmd)
		ft_memcpy(new_cmd, cmd_node->cmd, size * sizeof(char *));
	new_cmd[size] = ft_strdup(arg);
	if (!new_cmd[size])
	{
		free(new_cmd);
		return (0);
	}
	if (cmd_node->cmd)
		free(cmd_node->cmd);
	cmd_node->cmd = new_cmd;
	return (1);
}
