/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 21:42:25 by tdietztp          #+#    #+#             */
/*   Updated: 2025/10/05 03:09:17 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/parser.h"

//ok
/// @brief handles pipe token/ calls found_token with token_type pipe
/// @param token_list
/// @param i
/// @return current i position in prompt plus one
int	handle_pipe(t_token_list *token_list, int i)
{
	found_token(token_list, i, i + 1, TOKEN_PIPE);
	return (i + 1);
}

//ok
/// @brief handles ampersand token & calls found_token with token_type ampersand
/// @param token_list
/// @param i
/// @return current i position in prompt plus two (for &&)
int	handle_ampersand(t_token_list *token_list, int i)
{
	found_token(token_list, i, i + 1, TOKEN_AMPERSAND);
	return (i + 1);
}
