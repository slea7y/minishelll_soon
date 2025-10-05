/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segmentation2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 08:16:19 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 08:16:44 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

// ok
/// @brief loops through segments to process them
/// @param current_token
/// @param ctx
static void	process_segments(t_token *current_token, t_shell_ctx *ctx)
{
	t_segment	*current_seg;

	if (!current_token->segment_list)
		return ;
	current_seg = current_token->segment_list->head;
	while (current_seg)
	{
		process_escape_sequences_in_segment(current_seg);
		expand_variables_in_segment(current_seg, ctx);
		current_seg = current_seg->next;
	}
}

// ok
/// @brief controll fction for segmentation
/// @param tokens
/// @param ctx
void	start_segmentation(t_token_list *tokens, t_shell_ctx *ctx)
{
	t_token	*current_token;

	if (!tokens || !tokens->head || !ctx)
		return ;
	current_token = tokens->head;
	while (current_token)
	{
		dissect_token(current_token);
		process_segments(current_token, ctx);
		assemble_final_token(current_token);
		current_token = current_token->next;
	}
}
