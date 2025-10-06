/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 21:57:51 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/06 01:40:34 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../libft/libft.h"

static void	remove_node(t_env_node *node_to_remove,
		t_env_node *prev, t_env_list *env)
{
	if (prev == NULL)
		env->head = node_to_remove->next;
	else
		prev->next = node_to_remove->next;
	if (env->tail == node_to_remove)
		env->tail = prev;
	free(node_to_remove->key);
	free(node_to_remove->value);
	free(node_to_remove);
	env->size--;
}

void	remove_env_var(char *key, t_env_list *env)
{
	t_env_node	*current;
	t_env_node	*prev;
	t_env_node	*to_remove;

	if (!key || !env || !env->head)
		return ;
	current = env->head;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			to_remove = current;
			current = current->next;
			remove_node(to_remove, prev, env);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

int	ft_unset(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	int	i;
	int	exit_code;

	exit_code = 0;
	if (!ctx || !ctx->env)
		return (1);
	if (!cmd->cmd[1])
		return (0);
	i = 1;
	while (cmd->cmd[i])
	{
		if (!is_valid_identifier(cmd->cmd[i]))
			exit_code = 0;
		else
			remove_env_var(cmd->cmd[i], ctx->env);
		i++;
	}
	return (exit_code);
}
