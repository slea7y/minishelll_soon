/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 18:28:25 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 04:54:40 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../libft/libft.h"

static t_env_node	*create_new_node(char*key, char *value)
{
	t_env_node	*new;

	new = malloc(sizeof(t_env_node));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	if (!new->key || !new->value)
	{
		free(new->key);
		free(new->value);
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

void	add_new_var(char *key, char *value, t_env_list *env)
{
	t_env_node	*new;

	if (!key || !env || !value)
		return ;
	new = create_new_node(key, value);
	if (!new)
		return ;
	if (!env->head)
	{
		env->head = new;
		env->tail = new;
	}
	else
	{
		env->tail->next = new;
		env->tail = new;
	}
	env->size++;
}

void	change_var_value(char *key, char *new_value, t_env_list *env)
{
	t_env_node	*current;

	if (!key || !env || !new_value)
		return ;
	current = env->head;
	while (current)
	{
		if (ft_strcmp(key, current->key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(new_value);
			return ;
		}
		current = current->next;
	}
}

static int	extarct_key_value(char *args, char **key, char **value)
{
	char	*break_point;

	break_point = ft_strchr(args, '=');
	if (break_point)
	{
		*key = ft_substr(args, 0, break_point - args);
		*value = ft_strdup(break_point + 1);
	}
	else
	{
		*key = ft_strdup(args);
		*value = ft_strdup("");
	}
	if (!*key || !*value)
	{
		free(*key);
		free(*value);
		return (0);
	}
	return (1);
}

void	add_change_export(char *args, t_env_list *env)
{
	char	*key;
	char	*value;

	if (!extarct_key_value(args, &key, &value))
		return ;
	if (!check_if_var_exist(key, env))
		add_new_var(key, value, env);
	else
		change_var_value(key, value, env);
	free(key);
	free(value);
}
