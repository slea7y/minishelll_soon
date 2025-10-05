/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 02:21:35 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 18:56:10 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/parser.h"

// ok
/// @brief replaces ~ with homepath. cpy text before ~, find home env path,
//	adds it
/// @param segment
/// @param data
void	handle_tilde_expansion(t_segment *segment, t_expansion_data *data)
{
	char	*before_tilde;
	char	*home;
	char	*temp;
	char	*with_home;

	before_tilde = ft_substr(segment->value, *data->start, *data->i
			- *data->start);
	temp = ft_strjoin(*data->final_str, before_tilde);
	free(*data->final_str);
	home = get_env_value(data->ctx->env, "HOME");
	free(before_tilde);
	with_home = ft_strjoin(temp, home);
	free(temp);
	*data->final_str = with_home;
	free(home);
	(*data->i)++;
	*data->start = *data->i;
}

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

// ok
/// @brief process $var in a segment. checks if previous char is invalid
// loops til end of var, extracts it, get val value, cpys it
/// @param segment
/// @param data
static void	handle_dollar_variable(t_segment *segment, t_expansion_data *data)
{
	char	*var_name;
	char	*env_value;
	char	*temp;

	if (segment->value[*data->i] == '\0' || segment->value[*data->i] == ' '
		|| segment->value[*data->i] == '\t' || segment->value[*data->i] == '\n'
		|| segment->value[*data->i] == '"' || segment->value[*data->i] == '\'')
	{
		temp = ft_strjoin(*data->final_str, "$");
		free(*data->final_str);
		*data->final_str = temp;
	}
	else
	{
		while (ft_isalnum(segment->value[*data->i])
			|| segment->value[*data->i] == '_')
			(*data->i)++;
		var_name = ft_substr(segment->value, *data->start, *data->i
				- *data->start);
		env_value = get_env_value(data->ctx->env, var_name);

		if (env_value)
		{
			temp = ft_strjoin(*data->final_str, env_value);
			free(*data->final_str);
			*data->final_str = temp;
			free(var_name);
		}
		free(env_value);
	}
}

// ok
/// @brief main controllfction for $ expansion
// adds text before, skips $, identifies ?, '' or ""
/// @param segment
/// @param data
void	handle_dollar_expansion(t_segment *segment, t_expansion_data *data)
{
	char	*remaining;
	char	*temp;

	remaining = ft_substr(segment->value, *data->start, *data->i
			- *data->start);
	temp = ft_strjoin(*data->final_str, remaining);
	free(*data->final_str);
	*data->final_str = temp;
	free(remaining);
	(*data->i)++;
	*data->start = *data->i;
	if (segment->value[*data->i] == '?')
	{
		remaining = get_exit_code(data->ctx);
		temp = ft_strjoin(*data->final_str, remaining);
		free(*data->final_str);
		*data->final_str = temp;
		free(remaining);
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
