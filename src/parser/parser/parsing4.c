/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 06:28:39 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 06:44:19 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/parser.h"

// ok
/// @brief creates new cmd node, fillsit with value
/// @param token
/// @return
t_cmd_node	*create_cmd_node(t_token *token)
{
	t_cmd_node	*node;

	node = malloc(sizeof(t_cmd_node));
	if (!node)
		return (NULL);
	node->next = NULL;
	if (token->token == TOKEN_BUILTIN)
		node->cmd_type = BUILTIN;
	else
		node->cmd_type = CMD;
	node->cmd = malloc(sizeof(char *) * 2);
	if (!node->cmd)
	{
		free(node);
		return (NULL);
	}
	node->cmd[0] = ft_strdup(token->value);
	node->cmd[1] = NULL;
	node->files = NULL;
	return (node);
}

//ok
/// @brief adds new node to list. 
// if list unint,head poits to same node, if not adds tail
/// @param list
/// @param node
void	add_to_cmd_list(t_cmd_list *list, t_cmd_node *node)
{
	if (!list->head)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		list->tail->next = node;
		list->tail = node;
	}
	list->size++;
}
