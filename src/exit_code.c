/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_code.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 23:09:48 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 07:34:34 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"
#include "../libft/libft.h"
//ok
/// @brief converts exit code of last cmd in string.
// passes 0 as char not of number
/// @param ctx
/// @return
char	*get_exit_code(t_shell_ctx *ctx)
{
	char	*result;

	if (!ctx)
		return (ft_strdup("0"));
	result = ft_itoa(ctx->last_exit_code);
	if (!result)
		return (ft_strdup("0"));
	return (result);
}
