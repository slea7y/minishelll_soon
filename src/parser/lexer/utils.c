/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:09:59 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/04 07:38:20 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/parser.h"

/// @brief checks if the given char is space
/// @param c
/// @return true for whitespace false for non whitespace char
bool	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\r'
		|| c == '\f')
		return (true);
	return (false);
}

/// @brief not enough char per line. Need to make a wrap fction
/// @param str
/// @param fd
void	ft_p(char *str, int fd)
{
	ft_putstr_fd(str, fd);
}
