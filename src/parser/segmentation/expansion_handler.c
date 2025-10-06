/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 02:21:35 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 22:55:22 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/parser.h"

// ok
/// @brief cpys string after $ and between quotes in final_str.
// skips $, loops util closing quote, cpys value, skips closing quote
/// @param segment
/// @param data
/// @param quote
static void	handle_dollar_quotes(t_segment *segment, t_expansion_data *data,
		char quote)
{
	char	*literal_str;
	int		quote_start;
	char	*temp;

	(*data->i)++;
	quote_start = *data->i;
	while (segment->value[*data->i] && segment->value[*data->i] != quote)
		(*data->i)++;
	if (segment->value[*data->i] == quote)
	{
		literal_str = ft_substr(segment->value, quote_start, *data->i
				- quote_start);
		temp = ft_strjoin(*data->final_str, literal_str);
		free(*data->final_str);
		*data->final_str = temp;
		free(literal_str);
		(*data->i)++;
	}
}

static void	safe_strjoin_inplace(char **dest, char *src)
{
	char	*temp;

	if (!src)
		return ;
	temp = ft_strjoin(*dest, src);
	free(*dest);
	*dest = temp;
}

// ok
/// @brief process $var in a segment. checks if previous char is invalid
// loops til end of var, extracts it, get val value, cpys it
/// @param segment
/// @param data
static void	handle_dollar_variable(t_segment *segment, t_expansion_data *data)
{
	char	*var_name;
	char	*env_value;

	if (segment->value[*data->i] == '\0' || ft_strchr(" \t\n'\"",
			segment->value[*data->i]))
	{
		safe_strjoin_inplace(data->final_str, "$");
	}
	else
	{
		while (ft_isalnum(segment->value[*data->i])
			|| segment->value[*data->i] == '_')
			(*data->i)++;
		var_name = ft_substr(segment->value, *data->start, *data->i
				- *data->start);
		env_value = get_env_value(data->ctx->env, var_name);
		safe_strjoin_inplace(data->final_str, env_value);
		free(var_name);
		free(env_value);
	}
}

// ok
/// @brief main controllfction for $ expansion
// adds text before, skips $, identifies ?, '' or ""
/// @param segment
/// @param data
static void	append_and_free(char **dest, char *temp_src)
{
	char	*new_str;

	if (!temp_src)
		return ;
	new_str = ft_strjoin(*dest, temp_src);
	free(*dest);
	*dest = new_str;
	free(temp_src);
}

void	handle_dollar_expansion(t_segment *segment, t_expansion_data *data)
{
	append_and_free(data->final_str, ft_substr(segment->value, *data->start,
			*data->i - *data->start));
	(*data->i)++;
	*data->start = *data->i;
	if (segment->value[*data->i] == '?')
	{
		append_and_free(data->final_str, get_exit_code(data->ctx));
		(*data->i)++;
	}
	else if (segment->value[*data->i] == '"')
		handle_dollar_quotes(segment, data, '"');
	else if (segment->value[*data->i] == '\'')
		handle_dollar_quotes(segment, data, '\'');
	else
		handle_dollar_variable(segment, data);
	*data->start = *data->i;
}
