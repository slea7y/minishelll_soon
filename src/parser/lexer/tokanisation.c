/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokanisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 17:25:10 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 03:32:13 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/parser.h"
#include <stdio.h> // Füge das hinzu, wenn es noch nicht da ist

// ok name has a typo
/// @brief goes through the prompt and checks for different chars for the
/// different token types and passes it to the handler function
/// @param token_list
static void	split_tokenisation(t_token_list *token_list)
{
	int	i;

	i = 0;
	while (token_list->prompt[i])
	{
		while (token_list->prompt[i] && check_space(token_list->prompt[i]))
			i++;
		if (!token_list->prompt[i])
			break ;
		if (check_empty_quote(token_list, i))
			i = handle_empty_quotes(token_list, i);
		else if (check_redirection(token_list, i))
			i = handle_redirection(token_list, i);
		else if (token_list->prompt[i] == '|')
			i = handle_pipe(token_list, i);
		else if (token_list->prompt[i] == '&')
			i = handle_ampersand(token_list, i);
		else
			i = handle_word_master(token_list, i);
		if (i == -1)
		{
			token_list->found_error = 1;
			break ;
		}
	}
}

// ok
/// @brief general main fct of lexer. validation and tokeniser start.
/// calls all the necessary functions for the lexer
/// @param prompt string of combined arguments
/// @return
t_token_list	*start_lexer(char *prompt)
{
	t_token_list	*token_list;

	if (!prompt)
		return (NULL);
	token_list = initialise_token_list(prompt);
	if (!token_list)
		return (NULL);
	if (!validate_quotes(token_list))
	{
		token_list->found_error = 1;
		free(token_list);
		return (NULL);
	}
	split_tokenisation(token_list);
	if (token_list->found_error)
		return (free_token_list(token_list), NULL);
	if (!validate_syntax(token_list))
	{
		token_list->found_error = 1;
		free_token_list(token_list);
		return (NULL);
	}
	return (token_list);
}
