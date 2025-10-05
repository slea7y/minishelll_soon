/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_start.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 05:23:00 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 14:03:29 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

// ok
/// @brief adds new node to heredoc struct
/// @param list
/// @param new_node
static void	append_file_node(t_file_list *list, t_file_node *new_node)
{
	new_node->next = NULL;
	if (!list->head)
	{
		list->head = new_node;
		list->tail = new_node;
	}
	else
	{
		list->tail->next = new_node;
		list->tail = new_node;
	}
	list->size++;
}

// ok
/// @brief fills redirection struct for heredocs
/// @param file
/// @param next
/// @param original_input
static void	populate_heredoc_node(t_file_node *file, t_token *next,
		char *original_input)
{
	file->redir_type = HEREDOC;
	file->filename = ft_strdup(next->value);
	file->delimiter_quoted = is_delimiter_quoted_in_original_input(original_input,
			next->value);
}

/// ok
/// @brief fills redireciton struct for everything exept heredoc
/// @param file
/// @param token
/// @param next
static void	populate_other_redir_node(t_file_node *file, t_token *token,
		t_token *next)
{
	file->filename = ft_strdup(next->value);
	file->delimiter_quoted = 0;
	if (token->token == TOKEN_INFILE)
		file->redir_type = INFILE;
	else if (token->token == TOKEN_OUTFILE)
		file->redir_type = OUTFILE;
	else if (token->token == TOKEN_APPEND)
		file->redir_type = OUTFILE_APPEND;
}

// ok
/// @brief control struct to add redirection to command struct
/// @param cmd_node
/// @param token
/// @param next
/// @param original_input
void	add_redirection(t_cmd_node *cmd_node, t_token *token, t_token *next,
		char *original_input)
{
	t_file_node	*file;
	t_file_list	*file_list;

	if (!next || !next->value)
		return ;
	file_list = init_file_list_if_needed(cmd_node);
	if (!file_list)
		return ;
	file = malloc(sizeof(t_file_node));
	if (!file)
		return ;
	if (token->token == TOKEN_HEREDOC)
		populate_heredoc_node(file, next, original_input);
	else
		populate_other_redir_node(file, token, next);
	append_file_node(file_list, file);
}
