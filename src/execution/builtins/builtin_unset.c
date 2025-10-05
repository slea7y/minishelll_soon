/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 21:57:51 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 05:07:44 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../libft/libft.h"

static void	remove_node(t_env_node *current, t_env_node *prev, t_env_list *env)
{
	if (prev == NULL)
		env->head = current->next;
	else
		prev->next = current->next;
	if (env->tail == current)
		env->tail = prev;
	free(current->key);
	free(current->value);
	free(current);
	env->size--;
}

void	remove_env_var(char *key, t_env_list *env)
{
	t_env_node	*current;
	t_env_node	*prev;

	if (!key || !env || !env->head)
		return ;
	current = env->head;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			remove_node(current, prev, env);
			return ;
		}
		prev = current;
		current = current->next;
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
		{
			ft_putstr_fd(cmd->cmd[i], 2);
			ft_putstr_fd(" : not a valid identifier\n", 2);
			exit_code = 1;
		}
		else
			remove_env_var(cmd->cmd[i], ctx->env);
		i++;
	}
	return (exit_code);
}
