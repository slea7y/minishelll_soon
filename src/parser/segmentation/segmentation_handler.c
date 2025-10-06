/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segmentation_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 20:25:22 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 22:46:57 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

// ok
/// @brief  loops trought segments to assemble final segment
/// @param token
void	assemble_final_token(t_token *token)
{
	t_segment	*current_seg;
	char		*new_value;
	char		*temp;

	if (!token || !token->segment_list || !token->segment_list->head)
		return ;
	new_value = ft_strdup("");
	if (!new_value)
		return ;
	current_seg = token->segment_list->head;
	while (current_seg)
	{
		if (current_seg->value)
		{
			temp = ft_strjoin(new_value, current_seg->value);
			free(new_value);
			new_value = temp;
			if (!new_value)
				return ;
		}
		current_seg = current_seg->next;
	}
	free(token->value);
	token->value = new_value;
}

// ok
/// @brief creates temporary token wrapper for string variable expansion
/// converts a string into a token structure to reuse existing
// segmentation pipeline. the token will be deleted after processing,
// only the expanded string is returned
/// @param line the input string that needs variable expansion
/// @return temporary token containing the input string,
//	or NULL on malloc failure
static t_token	*create_temp_token(char *line)
{
	t_token	*temp_token;

	temp_token = ft_calloc(sizeof(t_token), 1);
	if (!temp_token)
		return (NULL);
	temp_token->value = ft_strdup(line);
	temp_token->token = TOKEN_WORD;
	temp_token->next = NULL;
	temp_token->segment_list = NULL;
	return (temp_token);
}

// ok
/// @brief expands var in a single string using token segmentation pipeline
/// creates temp token wrapper to process string through existing segmentation,
/// variable expansion,
//	and escape sequence handling. used for heredoc processing
/// @param line the input string that needs variable expansion
/// @param ctx shell context containing environment variables and state
/// @return newly allocated string with expanded variables,
//	or empty string on error
char	*expand_string_variables(char *line, t_shell_ctx *ctx)
{
	t_token_list	*token_list;
	t_token			*temp_token;
	char			*result;

	if (!line || !ctx)
		return (ft_strdup(""));
	token_list = initialise_token_list(line);
	if (!token_list)
		return (ft_strdup(""));
	temp_token = create_temp_token(line);
	if (!temp_token)
	{
		free(token_list);
		return (ft_strdup(""));
	}
	token_list->head = temp_token;
	token_list->tail = temp_token;
	token_list->size = 1;
	start_segmentation(token_list, ctx);
	result = ft_strdup(temp_token->value);
	free_token_list(token_list);
	return (result);
}

static void	create_final_segment(t_token *token, int start, int len)
{
	char	*temp;

	if (len <= 0)
		return ;
	temp = ft_substr(token->value, start, len);
	add_segment_to_token(token, temp, SEG_NORMAL_QUOTE);
	free(temp);
}

// ok
/// @brief control lfction loops through token to call handler
/// @param token
void	dissect_token(t_token *token)
{
	int	i;
	int	start;

	if (!token || !token->value)
		return ;
	i = 0;
	start = 0;
	while (token->value[i])
	{
		if (token->value[i] == '$' && (token->value[i + 1] == '"'
				|| token->value[i + 1] == '\''))
			handle_dollar_quotes_seg(token, &i, &start);
		else if (token->value[i] == '\'' || token->value[i] == '"')
			handle_quotes(token, &i, &start);
		else if (token->value[i] == '$')
			handle_dollar_vars(token, &i, &start);
		else
			i++;
	}
	create_final_segment(token, start, i - start);
}
