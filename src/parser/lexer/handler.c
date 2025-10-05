/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 22:42:56 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 03:09:14 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/parser.h"

// ok
/// @brief skips empty quotes
/// @param token_list
/// @param i position of first char after quotes incremented before passing
int	handle_empty_quotes(t_token_list *token_list, int i)
{
	(void)token_list;
	return (i + 2);
}

// ok
/// @brief defines token_type of redirection if Append >>,
///			outfile >,Heredoc << or Infile <. calls found_token
/// @param token_list
/// @param i
/// @return one or two increments from the given i
///	depending if its a one or two char redirection
int	handle_redirection(t_token_list *token_list, int i)
{
	int	start;

	start = i;
	if (token_list->prompt[i] == '>')
	{
		if (token_list->prompt[i + 1] == '>')
			return (found_token(token_list, start, i + 2, TOKEN_APPEND), i + 2);
		else
			return (found_token(token_list, start, i + 1, TOKEN_OUTFILE), i
				+ 1);
	}
	else if (token_list->prompt[i] == '<')
	{
		if (token_list->prompt[i + 1] == '<')
			return (found_token(token_list, start, i + 2, TOKEN_HEREDOC), i
				+ 2);
		else
			return (found_token(token_list, start, i + 1, TOKEN_INFILE), i + 1);
	}
	return (i);
}

// ok
/// @brief find end of quotes and returns it
/// @param token_list
/// @param i
/// @return
int	handle_word_in_quotes(t_token_list *token_list, int i)
{
	char	quote_type;

	quote_type = token_list->prompt[i];
	i++;
	while (token_list->prompt[i] && token_list->prompt[i] != quote_type)
		i++;
	if (token_list->prompt[i] == quote_type)
		return (i + 1);
	if (!token_list->prompt[i])
	{
		token_list->found_error = 1;
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		return (i);
	}
	return (i);
}

//ok but new
/// @brief checks if it a char that ends a token
/// @param token_list 
/// @param i 
/// @return 
static bool	is_token_end(t_token_list *token_list, int i)
{
	if (check_space(token_list->prompt[i]))
		return (true);
	if (token_list->prompt[i] == '|')
		return (true);
	if (check_redirection(token_list, i))
		return (true);
	return (false);
}

//ok but new
/// @brief Finds a word token using a state-based loop.
/// The loop explicitly breaks when a token separator is found.
/// @param token_list
/// @param i
/// @return Position of the next char after the current token.
int	handle_word_master(t_token_list *token_list, int i)
{
	int	start;

	start = i;
	while (token_list->prompt[i])
	{
		if (check_quote(token_list, i))
			i = handle_word_in_quotes(token_list, i);
		else if (is_token_end(token_list, i))
			break ;
		else
			i++;
	}
	found_token(token_list, start, i, TOKEN_WORD);
	return (i);
}
