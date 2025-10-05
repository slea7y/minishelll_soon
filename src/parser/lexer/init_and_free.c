/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 18:09:10 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 18:18:08 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/parser.h"

/// @brief initialise the token list like the name said lol
/// @param prompt combined string of arguments
/// @return
t_token_list	*initialise_token_list(char *prompt)
{
	t_token_list	*token_list;

	token_list = ft_calloc(1, sizeof(t_token_list));
	if (!token_list)
		return (NULL);
	token_list->prompt = ft_strdup(prompt);
	if (!token_list->prompt)
		return (NULL);
	return (token_list);
}

void	free_segment_list(t_segment_list *seg_list)
{
	t_segment	*current;
	t_segment	*next;

	if (!seg_list)
		return ;
	current = seg_list->head;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	free(seg_list);
}

/// @brief free the people and the token_list
/// @param tokens
void	free_token_list(t_token_list *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return ;
	current = tokens->head;
	while (current)
	{
		next = current->next;
		free(current->value);
		if (current->segment_list)
			free_segment_list(current->segment_list);
		free(current);
		current = next;
	}

	free(tokens->prompt);
	free(tokens);
}
