/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segmentation2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 08:16:19 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 22:55:12 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

// ok
/// @brief replaces ~ with homepath. cpy text before ~, find home env path,
//	adds it
/// @param segment
/// @param data
void	handle_tilde_expansion(t_segment *segment, t_expansion_data *data)
{
	char	*before_tilde;
	char	*home;
	char	*temp;
	char	*with_home;

	before_tilde = ft_substr(segment->value, *data->start, *data->i
			- *data->start);
	temp = ft_strjoin(*data->final_str, before_tilde);
	free(*data->final_str);
	home = get_env_value(data->ctx->env, "HOME");
	free(before_tilde);
	with_home = ft_strjoin(temp, home);
	free(temp);
	*data->final_str = with_home;
	free(home);
	(*data->i)++;
	*data->start = *data->i;
}

// ok
/// @brief checks if value should be processesd depedning if its in "" or ""
/// @param segment
void	process_escape_seq_in_seg(t_segment *segment)
{
	char	*processed;
	char	*temp;

	if (!segment || !segment->value)
		return ;
	if (segment->type == SEG_NORMAL_QUOTE)
	{
		processed = process_escape_sequences(segment->value);
		if (processed)
		{
			temp = segment->value;
			segment->value = processed;
			free(temp);
		}
	}
}

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
		process_escape_seq_in_seg(current_seg);
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
