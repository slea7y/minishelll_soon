/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segmentation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 08:34:01 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 12:32:44 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

// ok
/// @brief creates and fills segment
/// @param value
/// @param type
/// @return
static t_segment	*create_and_fill_segment(char *value, t_seg_quote_type type)
{
	t_segment	*segment;

	if (!value)
		return (NULL);
	segment = (t_segment *)malloc(sizeof(t_segment));
	if (!segment)
		return (NULL);
	segment->value = ft_strdup(value);
	segment->type = type;
	segment->next = NULL;
	if (!segment->value)
	{
		free(segment);
		return (NULL);
	}
	return (segment);
}

// ok
/// @brief adds new segment to token.
/// @param token
/// @param value
/// @param type
void	add_segment_to_token(t_token *token, char *value, t_seg_quote_type type)
{
	t_segment	*new_seg;

	if (!token)
		return ;
	if (!token->segment_list)
	{
		token->segment_list = init_segment_list();
		if (!token->segment_list)
			return ;
	}
	if (!value || ft_strlen(value) == 0)
		return ;
	new_seg = create_and_fill_segment(value, type);
	if (!new_seg)
		return ;
	if (!token->segment_list->head)
		token->segment_list->head = new_seg;
	else
		token->segment_list->tail->next = new_seg;
	token->segment_list->tail = new_seg;
	token->segment_list->size++;
}

// ok
/// @brief proccesses input around $. adds text before,
// extracts quote, looks for end of quote, adds $ segment as normal segment
/// @param token
/// @param i
/// @param start
void	handle_dollar_quotes_seg(t_token *token, int *i, int *start)
{
	char	quote_char;
	char	*temp;

	if (*i > *start)
	{
		temp = ft_substr(token->value, *start, *i - *start);
		add_segment_to_token(token, temp, SEG_NORMAL_QUOTE);
		free(temp);
	}
	quote_char = token->value[*i + 1];
	*start = *i;
	*i += 2;
	while (token->value[*i] && token->value[*i] != quote_char)
		(*i)++;
	if (token->value[*i] == quote_char)
		(*i)++;
	temp = ft_substr(token->value, *start, *i - *start);
	add_segment_to_token(token, temp, SEG_NORMAL_QUOTE);
	free(temp);
	*start = *i;
}

// ok
/// @brief sperates text infront of quote, in quote and after quote
/// @param token
/// @param i
/// @param start
void	handle_quotes(t_token *token, int *i, int *start)
{
	char	quote_char;
	char	*temp;

	if (*i > *start)
	{
		temp = ft_substr(token->value, *start, *i - *start);
		add_segment_to_token(token, temp, SEG_NORMAL_QUOTE);
		free(temp);
	}
	quote_char = token->value[*i];
	*start = *i + 1;
	(*i)++;
	while (token->value[*i] && token->value[*i] != quote_char)
		(*i)++;
	if (*i > *start)
	{
		temp = ft_substr(token->value, *start, *i - *start);
		if (quote_char == '\'')
			add_segment_to_token(token, temp, SEG_SINGLE_QUOTE);
		else
			add_segment_to_token(token, temp, SEG_DOUBLE_QUOTE);
		free(temp);
	}
	if (token->value[*i])
		(*i)++;
	*start = *i;
}

// ok
/// @brief processes $var, adds text before, extracts $name, adds segment
/// @param token
/// @param i
/// @param start
void	handle_dollar_vars(t_token *token, int *i, int *start)
{
	char	*temp;

	if (*i > *start)
	{
		temp = ft_substr(token->value, *start, *i - *start);
		add_segment_to_token(token, temp, SEG_NORMAL_QUOTE);
		free(temp);
	}
	*start = *i;
	(*i)++;
	if (token->value[*i] == '?')
		(*i)++;
	else
	{
		while (token->value[*i] && (ft_isalnum(token->value[*i])
				|| token->value[*i] == '_'))
			(*i)++;
	}
	temp = ft_substr(token->value, *start, *i - *start);
	add_segment_to_token(token, temp, SEG_NORMAL_QUOTE);
	free(temp);
	*start = *i;
}
