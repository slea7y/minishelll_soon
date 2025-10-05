/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 21:40:17 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 19:10:43 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/parser.h"

// ok
/// @brief Processes escape sequences in a string,
/// removing backslashes and handling escaped characters
/// @param str
/// @return
char	*process_escape_sequences(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] != '\0')
		{
			i++;
			result[j++] = str[i++];
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

// ok
/// @brief loops through the env var list until it finds a match,
// returns its value
/// @param env_list
/// @param var_name
/// @return
char	*get_env_value(t_env_list *env_list, char *var_name)
{
	t_env_node	*current;

	if (!env_list || !env_list->head || !var_name)
		return (ft_strdup(""));
	current = env_list->head;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (ft_strdup(""));
}

// ok
/// @brief adds given text to final_str
/// @param segment
/// @param final_str
/// @param start
/// @param i
static void	process_remaining_text(t_segment *segment, char **final_str,
		int start, int i)
{
	char	*remaining;
	char	*temp;

	remaining = ft_substr(segment->value, start, i - start);
	temp = ft_strjoin(*final_str, remaining);
	free(*final_str);
	*final_str = temp;
	free(remaining);
}

// ok
/// @brief loops trough segment for ~ or $, not in ', calls handler
/// @param segment
/// @param data
static void	process_expansions(t_segment *segment, t_expansion_data *data)
{
    while (segment->value[*data->i])
    {
        if (segment->value[*data->i] == '~' && segment->type == SEG_NORMAL_QUOTE
            && (*data->i == 0 || ft_strchr(" \t\n", segment->value[*data->i
                        - 1])))
            handle_tilde_expansion(segment, data);
		else if (segment->value[*data->i] == '$'
			&& segment->type != SEG_SINGLE_QUOTE && segment->value[*data->i
				+ 1] != '\0')
			handle_dollar_expansion(segment, data);
		else
			(*data->i)++;
	}
}

// ok============PASSES MORE TEST BUT 
/// @brief controll fction expansionm ii
/// @param segment
/// @param ctx
void	expand_variables_in_segment(t_segment *segment, t_shell_ctx *ctx)
{
	t_expansion_data	data;
	char				*final_str;
	int					i;
	int					start;

	if (!segment || !segment->value || segment->value[0] == '\0')
		return ;
	final_str = ft_strdup("");
	if (!final_str)
		return ;
	i = 0;
	start = 0;
	data.final_str = &final_str;
	data.i = &i;
	data.start = &start;
	data.ctx = ctx;
	process_expansions(segment, &data);
	process_remaining_text(segment, &final_str, start, i);
	if (final_str != NULL && final_str[0] != '\0')
	{
		free(segment->value);
		segment->value = final_str;
	}
	else
		free(final_str);
}

