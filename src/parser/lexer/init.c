/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:52:42 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 03:24:33 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/parser.h"

//ok
/// @brief does what it looks like lol. initialises the segment_list
/// @param
/// @return
t_segment_list	*init_segment_list(void)
{
	t_segment_list	*list;

	list = (t_segment_list *)malloc(sizeof(t_segment_list));
	if (!list)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}
