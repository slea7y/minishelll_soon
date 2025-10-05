/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:25:21 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 18:48:59 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/executor.h"
#include "Includes/minishell.h"
#include "Includes/parser.h"

/// maybe a leanpu file will me necessary

/// @brief unlink - call the unlink function to remove the specified
/// @param file_list
void	free_file_list(t_file_list *file_list)
{
	t_file_node	*current;
	t_file_node	*next;

	if (!file_list)
		return ;
	current = file_list->head;
	while (current)
	{
		next = current->next;
		if (current->filename)
		{
			if (current->redir_type == HEREDOC)
			{
				unlink(current->filename);
				free(current->filename);
			}
			else
				free(current->filename);
		}
		free(current);
		current = next;
	}
	free(file_list);
}

void	free_cmd_list(t_cmd_list *cmds_list)
{
	t_cmd_node	*current;
	t_cmd_node	*next;
	int			i;

	if (!cmds_list)
		return ;
	current = cmds_list->head;
	while (current)
	{
		next = current->next;
		if (current->cmd)
		{
			i = 0;
			while (current->cmd[i])
				free(current->cmd[i++]);
			free(current->cmd);
		}
		if (current->files)
			free_file_list(current->files);
		free(current);
		current = next;
	}
	free(cmds_list);
}

void	free_env_list(t_env_list *env_list)
{
	t_env_node	*current;
	t_env_node	*next;

	if (!env_list || !env_list->head)
		return ;
	current = env_list->head;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	env_list->head = NULL;
	env_list->tail = NULL;
	env_list->size = 0;
}
