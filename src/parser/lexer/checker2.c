/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:27:53 by tdietztp          #+#    #+#             */
/*   Updated: 2025/10/05 08:57:12 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/parser.h"

/// @brief checks for unclosed quotes
/// @param token_list
/// @return true if valid, false if no bueneo
bool	validate_quotes(t_token_list *token_list)
{
	int		i;
	char	quote_char;
	bool	in_quotes;

	if (!token_list || !token_list->prompt)
		return (false);
	i = -1;
	in_quotes = false;
	quote_char = 0;
	while (token_list->prompt[++i])
	{
		if (!in_quotes && (token_list->prompt[i] == '"'
				|| token_list->prompt[i] == '\''))
		{
			in_quotes = true;
			quote_char = token_list->prompt[i];
		}
		else if (in_quotes && token_list->prompt[i] == quote_char)
			in_quotes = false;
	}
	if (in_quotes)
		return (ft_putstr_fd("minishell: syntax error: uncloded quotes\n", 2),
			false);
	return (true);
}

/// @brief checks if current char is a token separation char
/// @param token_list
/// @param i
/// @return false if it is not a separation char
/// true if it is a separation char
bool	check_token_split(t_token_list *token_list, int i)
{
	if (token_list->prompt[i] && !check_space(token_list->prompt[i])
		&& token_list->prompt[i] != '|' && token_list->prompt[i] != '>'
		&& token_list->prompt[i] != '<')
		return (false);
	else
		return (true);
}
