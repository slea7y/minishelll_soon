/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 01:31:21 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/06 01:33:08 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../libft/libft.h"

static void	print_environment(t_env_list *env)
{
	t_env_node	*current;

	if (!env)
		return ;
	current = env->head;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		current = current->next;
	}
}

static int	handle_env_arguments(char *arg)
{
	ft_putstr_fd("env: '", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': No such file or directory\n", STDERR_FILENO);
	return (127);
}

int	ft_env(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	int	exit_code;

	exit_code = 0;
	if (!ctx || !ctx->env)
		return (1);
	if (cmd->cmd[1])
		return (handle_env_arguments(cmd->cmd[1]));
	print_environment(ctx->env);
	return (exit_code);
}
