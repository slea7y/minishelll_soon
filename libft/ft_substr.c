/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:52:51 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/04 07:25:37 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dst;
	size_t	len_s;

	if (!s)
		return (NULL);
	len_s = ft_strlen(s);
	if (start >= len_s)
		return (ft_strdup(""));
	if ((start + len) >= len_s)
		len = len_s - start;
	dst = (char *)malloc(len + 1);
	if (!dst)
		return (NULL);
	ft_memmove(dst, s + start, len);
	dst[len] = '\0';
	return (dst);
}
